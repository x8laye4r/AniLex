import asyncio
import datetime
import json
import os
import sqlite3

import httpx
import keyring
import requests
import yaml
from keyring.errors import PasswordSetError, PasswordDeleteError
from tinydb import TinyDB, Query


def get_cache_path():
    aktuell = os.path.abspath(os.path.dirname(__file__))
    parent = os.path.dirname(aktuell)
    cache_pfad = os.path.join(parent, "cache")
    return cache_pfad

STANDARD_PATH = get_cache_path()

PRIORITY_PATH = os.path.join(STANDARD_PATH, "anime_data", "custom_priority_db.json")
NOTIFICATIONS_PATH = os.path.join(STANDARD_PATH, "anime_data", "scheduled_notifications.db")
USER_DATA_PATH = os.path.join(STANDARD_PATH, "user_data.json")
SETTINGS_PATH = os.path.join(STANDARD_PATH, "settings.yaml")
TYPE_ANIME = "Anime"
TYPE_MANGA = "Manga"
QUERY = Query()

try:
    db = TinyDB(PRIORITY_PATH)
except Exception as e:
    raise RuntimeError(f"Error to initialize database: {e}")

def _load_user_data():
    if not os.path.exists(USER_DATA_PATH):
        raise FileNotFoundError(f"User data not found {USER_DATA_PATH}")
    try:
        with open(USER_DATA_PATH, "r") as file:
            return json.load(file)
    except json.JSONDecodeError:
        raise ValueError("User data are corrupt or in a not valid JSON format.")


class NotificationDatabaseHandler:
    def __init__(self):
        if not os.path.exists(NOTIFICATIONS_PATH):
            with open(NOTIFICATIONS_PATH, "w"):
                pass

        self.conn = sqlite3.connect(NOTIFICATIONS_PATH)
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
            response = requests.get(url)
            response.raise_for_status()
            return response.content
        except requests.exceptions.RequestException as e:
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
        except requests.exceptions.RequestException as e:
            raise RuntimeError(f"Error while fetching image: {e}")
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

handler = NotificationDatabaseHandler()
handler.delete_all_notifications()
handler.insert_notification(2, "Test", 1, 1,"2025-10-08", "19:06",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/cover/small/bx1-GCsPm7waJ4kS.png",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/banner/1-OquNCNB6srGe.jpg")
handler.insert_notification(3, "Test", 1, 1,"2025-10-08", "19:06",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/cover/small/bx1-GCsPm7waJ4kS.png",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/banner/1-OquNCNB6srGe.jpg")
handler.insert_notification(4, "Test", 1, 1,"2025-10-08", "19:08",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/cover/small/bx1-GCsPm7waJ4kS.png",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/banner/1-OquNCNB6srGe.jpg")
handler.insert_notification(5, "Test", 1, 1,"2025-10-08", "19:07",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/cover/small/bx1-GCsPm7waJ4kS.png",
                            "https://s4.anilist.co/file/anilistcdn/media/anime/banner/1-OquNCNB6srGe.jpg")

def get_mal_id(id, type_):
    URL = "https://graphql.anilist.co"
    query = f"""
    query {{
        Media(id: {id}, type: {type_}) {{
            idMal
        }}
    }}
    """
    response = requests.post(URL, json={"query": query})
    data = response.json()

    if "errors" in data:
        print("Error:", data["errors"])
        return None

    return data["data"]["Media"]["idMal"]

def get_mutation_path():
    return os.path.join(os.getcwd(), "GraphQL", "mutations")

def get_query_path():
    return os.path.join(os.getcwd(), "GraphQL", "queries")

def load_settings():
    try:
        with open(SETTINGS_PATH, "r", encoding="utf-8") as file:
            return yaml.safe_load(file) or None
    except FileNotFoundError:
        return {}

def get_card_settings():
    settings = load_settings()
    if settings is None:
        raise ValueError("Settings file is empty or not valid.")
    try:
        return settings["CARD_OPTIONS"]
    except KeyError:
        raise KeyError("Key 'CARD_OPTIONS' is missing in the settings.")

def get_id():
    data = _load_user_data()
    try:
        return data["auth"]["user_id"]
    except KeyError:
        raise KeyError("Key 'auth.user_id' is missing in the user data.")

def get_username():
    data = _load_user_data()
    try:
        return data["auth"]["username"]
    except KeyError:
        raise KeyError("Key 'auth.username' is missing in user data.")

def get_service_name():
    data = _load_user_data()
    try:
        return data["auth_key"]["service_name"]
    except KeyError:
        raise KeyError("Key 'auth_key.service_name' is missing in user data.")

def get_user_token_key():
    data = _load_user_data()
    try:
        return data["auth_key"]["user_token_key"]
    except KeyError:
        raise KeyError("Key 'auth_key.user_token_key' is missing in user data.")

def get_auth_key():
    service = get_service_name()
    user_key = get_user_token_key()
    token = keyring.get_password(service, user_key)
    if token is None:
        raise ValueError(f"No Auth-Token in keyring for Service '{service}' and User-Key '{user_key}' found.")
    return token

def db_get_all_anime():
    try:
        return db.search(QUERY.type == TYPE_ANIME)
    except Exception as e:
        raise RuntimeError(f"Error reading all anime: {e}")

def db_get_all_manga():
    try:
        return db.search(QUERY.type == TYPE_MANGA)
    except Exception as e:
        raise RuntimeError(f"Error reading all manga: {e}")

def db_get_anime_by_id(id: int):
    try:
        found = db.search((QUERY.id == id) & (QUERY.type == TYPE_ANIME))
        return found[0] if found else None
    except Exception as e:
        raise RuntimeError(f"Error reading anime with id {id}: {e}")

def db_get_manga_by_id(id: int):
    try:
        found = db.search((QUERY.id == id) & (QUERY.type == TYPE_MANGA))
        return found[0] if found else None
    except Exception as e:
        raise RuntimeError(f"Error reading manga with id {id}: {e}")

def db_add(id: int, title: str, type: str):
    try:
        if db.search((QUERY.id == id) & (QUERY.type == type)):
            raise ValueError(f"Entry with ID {id} and type '{type}' exists already.")
        db.insert({'id': id, 'title': title, 'type': type})
    except Exception as e:
        raise RuntimeError(f"Error while appending data: {e}")

def db_remove_anime(id: int):
    try:
        db.remove((QUERY.id == id) & (QUERY.type == TYPE_ANIME))
    except Exception as e:
        raise RuntimeError(f"Error while deleting anime with id {id}: {e}")

def db_remove_manga(id: int):
    try:
        db.remove((QUERY.id == id) & (QUERY.type == TYPE_MANGA))
    except Exception as e:
        raise RuntimeError(f"Error while deleting manga with id {id}: {e}")

def db_delete_all():
    try:
        db.truncate()
    except Exception as e:
        raise RuntimeError(f"Error while deleting databases data: {e}")

def set_priority(id: int, title: str, status: str, priority: str, type: str):
    if status.lower() != "planning":
        return
    try:
        db.insert({'id': id, 'title': title, 'priority': priority, 'type': type})
    except Exception as e:
        raise RuntimeError(f"Error while setting priority: {e}")

def update_priority(id: int, priority: str):
    try:
        if not db.update({'priority': priority}, QUERY.id == id):
            raise ValueError(f"No entry with id {id} found to update.")
    except Exception as e:
        raise RuntimeError(f"Error while updating priority: {e}")

def remove_priority(id: int):
    try:
        db.remove(QUERY.id == id)
    except Exception as e:
        raise RuntimeError(f"Error while removing priority with id {id}: {e}")

def get_priority(id: int):
    try:
        result = db.search(QUERY.id == id)
        return result[0] if result else None
    except Exception as e:
        raise RuntimeError(f"Error while reading the id {id}: {e}")

def clear_all_priorities():
    try:
        db.truncate()
    except Exception as e:
        raise RuntimeError(f"Error while deleting databases data: {e}")

def get_api_key():
    pw = keyring.get_password("OPENAI", get_username())
    if pw is None:
        print("No API-Key found in keyring. Please enter your API-Key.")
        key = input("Enter API Key: ")
        keyring.set_password("OPENAI", get_username(), key.strip())
        return key
    else:
        return pw

def save_trace_moe_key(key):
    try:
        keyring.set_password("Trace.moe", get_username(), key)
        return True
    except PasswordSetError:
        return False

def get_trace_moe_key():
    try:
        pw = keyring.get_password("Trace.moe", get_username())
        return pw
    except keyring.errors.KeyringError:
        return None

def delete_trace_moe_key():
    try:
        keyring.delete_password("Trace.moe", get_username())
        return True
    except PasswordDeleteError:
        return False

def save_sauce_noe_key(key):
    try:
        keyring.set_password("saucenoe", get_username(), key)
        return True
    except PasswordSetError:
        return False

def get_sauce_noe_key():
    try:
        pw = keyring.get_password("saucenoe", get_username())
        return pw
    except keyring.errors.KeyringError:
        return None

def delete_sauce_noe_key():
    try:
        keyring.delete_password("saucenoe", get_username())
        return True
    except PasswordDeleteError:
        return False