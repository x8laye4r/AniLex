import asyncio
import datetime
import os
import sqlite3

import httpx

from utils.anilex_helpers_v2 import get_cache_dir


class NotificationDatabaseHandler:
    def __init__(self):
        self.NOTIFICATION_PATH = os.path.join(get_cache_dir(), "anime_data", "scheduled_notifications.db")
        if not os.path.exists(self.NOTIFICATION_PATH):
            with open(self.NOTIFICATION_PATH, "w"):
                pass

        self.conn = sqlite3.connect(self.NOTIFICATION_PATH)
        self.cursor = self.conn.cursor()
        self._create_database(self.conn, self.cursor)


    # ---------------
    # Static Methods
    # ---------------
    @staticmethod
    def _create_database(conn: sqlite3.Connection, cursor: sqlite3.Cursor) -> None:
        try:
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS notifications (
                    id INTEGER PRIMARY KEY,
                    title TEXT, 
                    season_number INTEGER, 
                    episode_number INTEGER, 
                    time TIMESTAMP NOT NULL,
                    date TIMESTAMP NOT NULL,
                    poster BLOB DEFAULT NULL,
                    banner BLOB DEFAULT NULL   
                )
            """)
            conn.commit()
        except sqlite3.Error as e:
            raise RuntimeError(f"Error while creating database: {e}")

    @staticmethod
    def _get_image_data(url: str) -> bytes:
        try:
            response = httpx.get(url)
            response.raise_for_status()
            return response.content
        except httpx.RequestError as e:
            raise RuntimeError(f"Error while fetching image from {url}: {e}")

    @staticmethod
    def convert_images_to_bytes(image_urls: list[str]) -> list[bytes]:
        async def fetch_image(client: httpx.AsyncClient, url: str) -> bytes:
            try:
                resp = await client.get(url)
                resp.raise_for_status()
                return resp.content
            except Exception:
                return b""

        async def fetch_images(urls: list[str]) -> list[bytes]:
            async with httpx.AsyncClient() as client:
                tasks = [fetch_image(client, url) for url in urls]
                return await asyncio.gather(*tasks)

        return asyncio.run(fetch_images(image_urls))

    # ---------------
    # Core Methods
    # ---------------

    def insert_notification(self, id: int, title: str, season_number: int, episode_number: int, date: str, time: str, poster: str, banner: str) -> None:
        converted_date = datetime.datetime.strptime(date, "%Y-%m-%d").strftime("%Y-%m-%d")
        try:
            self.cursor.execute("""
                INSERT INTO notifications (id, title, season_number, episode_number, date, time, poster, banner) 
                VALUES (?, ?, ?, ?, ?, ?, ?, ?)
            """, (id, title, season_number, episode_number, converted_date, time, self._get_image_data(poster) , self._get_image_data(banner)))
            self.conn.commit()

        except sqlite3.Error as e:
            raise RuntimeError(f"Error while inserting notification: {e}")
        except Exception as e:
            raise RuntimeError(f"Unexpected error: {e}")

    def delete_notification(self, id: int) -> None:
        try:
            self.cursor.execute("""
                DELETE FROM notifications WHERE id = ?
            """, (id,))
            self.conn.commit()
        except sqlite3.Error as e:
            raise RuntimeError(f"Error while deleting notification with id {id}: {e}")

    def delete_all_notifications(self) -> None:
        try:
            self.cursor.execute("""
                DELETE FROM notifications
            """)
            self.conn.commit()
        except sqlite3.Error as e:
            raise RuntimeError(f"Error while deleting all notifications: {e}")

    def edit_notification(self, id: int, columns: list, values: list) -> None:
        if len(columns) != len(values):
            return
        set_clause = ", ".join([f"{col} = ?" for col in columns])
        try:
            self.cursor.execute(f"""
                UPDATE notifications SET {set_clause} WHERE id = ?
            """, (*values, id))
            self.conn.commit()
        except sqlite3.Error as e:
            raise RuntimeError(f"Error while updating notification with id {id}: {e}")

    def get_todays_notifications(self) -> list:
        today = datetime.date.today()
        try:
            self.cursor.execute("""
                SELECT * FROM notifications WHERE date = ? ORDER BY time ASC;
            """, (today,))
            return self.cursor.fetchall()
        except sqlite3.Error as e:
            raise RuntimeError(f"Error while reading today's notifications: {e}")

    def create_many_notifications(self, titles: list[str], season_numbers: list[int], episode_numbers: list[int],
                                  dates: list[str], times: list[str], posters: list[str], banners: list[str]) -> None:
        if not (len(titles) == len(season_numbers) == len(episode_numbers) == len(dates) == len(times) == len(posters) == len(banners)):
            return

        poster_bytes = self.convert_images_to_bytes(posters)
        banner_bytes = self.convert_images_to_bytes(banners)
        for i in range(len(titles)):
            converted_date = datetime.datetime.strptime(dates[i], "%Y-%m-%d")
            try:
                self.cursor.execute("""
                    INSERT INTO notifications (title, season_number, episode_number, date, time, poster, banner) 
                    VALUES (?, ?, ?, ?, ?, ?, ?)
                """, (titles[i], season_numbers[i], episode_numbers[i], converted_date, times[i], poster_bytes[i], banner_bytes[i]))
                self.conn.commit()
            except sqlite3.Error as e:
                print(f"Error while inserting notification: {e}")
            except Exception as e:
                print(f"Unexpected error: {e}")