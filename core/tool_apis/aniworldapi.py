import concurrent.futures
import logging
import os
import re
import sqlite3
import threading
from typing import Optional
from urllib.parse import urljoin

import cloudscraper
from bs4 import BeautifulSoup
from rapidfuzz import fuzz, process

from utils.anilex_helper import get_cache_path

DATA_DB = os.path.join(get_cache_path(), "api_data", "aniworld_data.db")


class AniWorldAPI:
    '''
        Class for scraping data from Aniworld.to
        Methods:
            - search_anime(anime): Search for an anime by name
            - scrape_anime(anime): Scrape info, max episodes, and max seasons for an anime by id
            - get_redirect_links(episode, season): Get streaming redirect links for a specific episode and season
            - get_streaming_link(data): Get the final streaming link by following redirects
            - max_episodes(): Get the maximum number of episodes for the current anime season
            - max_seasons(): Get the maximum number of seasons for the current anime
            - current_season(): Get the current season number
    '''

    def __init__(self):
        self.base_url = "https://aniworld.to"
        self.scraper = cloudscraper.create_scraper(
            browser={
                'browser': 'chrome',
                'platform': 'windows',
                'mobile': False
            }
        )

        # Data for the anime
        self.has_movie = False
        self.max_seasons = 0
        self.anime: Optional[str] = None
        self.info = {
            'title': "",
            'description': "",
            'genres': [],
            'poster': "",
        }
        self.home: Optional[dict] = None

        # Logging setup
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s- %(levelname)s - %(message)s'
        )
        self.logger = logging.getLogger(__name__)
        self.create_tables()

    # ---------------------------
    # Getter methods
    # ---------------------------
    def current_max_seasons(self):
        return self.max_seasons

    def get_info(self):
        return self.info

    # ---------------------------
    # Helper methods
    # ---------------------------

    @staticmethod
    def convert_to_id(data: list[str]):
        converted = {}
        for dat in data:
            text = dat.lower()
            text = re.sub(r'[^a-z0-9\s-]', '', text)
            text = re.sub(r'[\s-]+', '-', text)
            converted[dat] = text
        return converted

    @staticmethod
    def convert_sting_to_id(text: str):
        text = text.lower()
        text = re.sub(r'[^a-z0-9\s-]', '', text)
        text = re.sub(r'[\s-]+', '-', text)
        return text

    @staticmethod
    def create_tables():
        if not os.path.exists(os.path.dirname(DATA_DB)):
            os.makedirs(os.path.dirname(DATA_DB))
        if not os.path.exists(DATA_DB):
            with open(DATA_DB, 'w') as file:
                pass
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        # Create DB titles if not exists
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS titles (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title VARCHAR(255),
                anime_id VARCHAR(255)
            )
        ''')

        # Create DB home if not exists
        cursor.execute("PRAGMA foreign_keys = ON")
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS home ( 
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                anime INTEGER,
                poster BLOB,
                season INTEGER,
                episode INTEGER,
                lang TEXT,
                last_update TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                next_update TEXT,
                type TEXT,
                FOREIGN KEY(anime) REFERENCES titles(id)
            )
        ''')

        # Create DB anime_data if not exists
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS anime_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                anime INTEGER,
                has_movie INTEGER DEFAULT 0,
                poster BLOB,
                anime_description VARCHAR(2000),
                anime_genres VARCHAR(500),
                last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY(anime) REFERENCES titles(id)
            )
        ''')
        conn.commit()
        conn.close()

    @staticmethod
    def insert_title(title, anime_id):
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        cursor.execute("SELECT 1 FROM titles WHERE title = ? AND anime_id = ?", (title, anime_id))
        exists = cursor.fetchone()
        if exists:
            conn.close()
            return

        cursor.execute('''
            INSERT INTO titles (title, anime_id) VALUES (?, ?)
        ''', (title, anime_id))
        conn.commit()
        conn.close()

    def scrape_episode_and_titles(self, anime_id, season_num):
        contents = [None] * season_num
        local_scraper = threading.local()

        def get_scraper():
            if not hasattr(local_scraper, "scraper"):
                local_scraper.scraper = cloudscraper.create_scraper()
            return local_scraper.scraper

        def scrape_season(season_index):
            url = urljoin(self.base_url, f"/anime/stream/{anime_id}/staffel-{season_index}")
            self.logger.info(f"Scraping {url}")

            try:
                scraper = get_scraper()
                response = scraper.get(url)
                contents[season_index - 1] = response.text
            except Exception as e:
                self.logger.error(f"Failed to scrape {url}: {e}")

        with concurrent.futures.ThreadPoolExecutor(max_workers=5) as executor:
            futures = [executor.submit(scrape_season, i) for i in range(1, season_num + 1)]
            concurrent.futures.wait(futures)

        results = {}

        for season_index, content in enumerate(contents, start=1):
            if not content:
                self.logger.error(f"No content for season {season_index}")
                continue
            soup = BeautifulSoup(content, 'html.parser')
            tbody = soup.find('tbody')
            if not tbody:
                self.logger.error(f"No tbody found for season {season_index}")
                continue
            table_elements = tbody.find_all('tr')
            results[season_index] = []

            for table_element in table_elements:
                episode_title = table_element.find('strong').get_text(strip=True)
                if not episode_title:
                    episode_title = table_element.find('span').get_text(strip=True)

                lang_container = table_element.find('td', class_='editFunctions').find_all('img')
                langs = []
                for lang in lang_container:
                    src = lang.get('src', '')
                    langs.append(src.split("/")[-1].removesuffix(".svg"))

                results[season_index].append({
                    'episode': int(table_element.find('meta', itemprop='episodeNumber')['content']),
                    'title': episode_title,
                    'german-dub': int('german' in langs),
                    'english-sub': int('japanese-english' in langs),
                    'german-sub': int('japanese-german' in langs)
                })

        return results

    def seasons_episodes_table(self, anime_id, season_num):
        self.logger.info("Seasons episodes table creation")
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        try:
            cursor.execute(f'''
                CREATE TABLE IF NOT EXISTS "{anime_id}" (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    season INTEGER,
                    episode INTEGER,
                    episode_title VARCHAR(2000),
                    seen INTEGER DEFAULT 0,
                    last_stop TIMESTAMP DEFAULT NULL,
                    german_dub INTEGER DEFAULT 0,
                    german_sub INTEGER DEFAULT 0,
                    english_sub INTEGER DEFAULT 0
                )
            ''')
        except sqlite3.OperationalError as e:
            self.logger.error(e)

        results = self.scrape_episode_and_titles(anime_id, season_num)
        for season, episodes in results.items():
            for ep in episodes:
                try:
                    cursor.execute(f'''
                        INSERT INTO "{anime_id}" 
                            (season, episode, episode_title, german_dub, german_sub, english_sub)
                        VALUES (?, ?, ?, ?, ?, ?)
                    ''', (
                        season, ep.get('episode'), ep.get('title'), ep.get('german-dub'), ep.get('german-sub'),
                        ep.get('english-sub')
                    ))
                except sqlite3.OperationalError as e:
                    self.logger.error(e)

        self.logger.info("Seasons episodes table creation finished")
        conn.commit()
        conn.close()

    def update_anime(self, anime_id):
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        cursor.execute(f'''
            
        ''')

    def insert_anime_data(self, anime_id, anime_description="", anime_genres=[""], anime_poster_data="", max_seasons=0):
        self.logger.info("Inserting anime data into table")

        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        cursor.execute("SELECT 1 FROM anime_data WHERE anime = (SELECT id FROM titles WHERE anime_id = ?)", (anime_id,))
        exists = cursor.fetchone()

        if exists:
            self.logger.info("Anime data already exists. Skipping insert.")
            conn.close()
            self.seasons_episodes_table(anime_id, max_seasons)
            return

        try:
            cursor.execute("""
                INSERT INTO anime_data (anime, poster, has_movie, anime_description, anime_genres)
                VALUES ((SELECT id FROM titles WHERE anime_id = ?), ?, ?, ?, ?)
            """, (anime_id, anime_poster_data, int(self.has_movie), anime_description, ",".join(anime_genres)))
        except sqlite3.OperationalError as e:
            self.logger.error(e)

        conn.commit()
        conn.close()
        self.seasons_episodes_table(anime_id, max_seasons)

    @staticmethod
    def extract_id(link: str) -> Optional[str]:
        m = re.search(r"/anime/stream/([^/]+)", link)
        if m:
            return m.group(1)
        return None

    # ---------------------------
    # Core methods
    # ---------------------------

    def _download_poster(self, poster: Optional[str]) -> Optional[bytes]:
        """
            Download pictures helper function
        """
        if not poster or not isinstance(poster, str):
            return None
        try:
            poster_url = urljoin(self.base_url, poster) if poster.startswith("/") else poster
            resp = self.scraper.get(poster_url, timeout=10)
            if resp and getattr(resp, "status_code", 0) == 200:
                return resp.content
        except Exception:
            return None

    def add_home_to_database(self):
        """
            Fügt die Home-Daten in die Datenbank ein, angepasst an das neue Layout.
        """
        self.logger.info("Lösche alte Home-Daten.")
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()
        cursor.execute("DELETE FROM home;")
        conn.commit()
        conn.close()

        self.logger.info("Füge neue Home-Daten ein.")
        if not getattr(self, "home", None):
            self.logger.info("Keine Home-Daten vorhanden.")
            return

        tasks = []
        for section, items in self.home.items():
            if not isinstance(items, dict):
                continue
            for title, val in items.items():
                if not title:
                    continue
                anime_id = val.get("id") or val.get("anime_id")
                self.insert_title(title, anime_id)
                poster = val.get("poster")
                season = val.get("season")
                episode = val.get("episode")
                lang = val.get("lang")
                _time = val.get("time")
                entry_type = section
                tasks.append((anime_id, poster, season, episode, lang, _time, entry_type))

        if not tasks:
            self.logger.info("Keine Einträge zum Einfügen.")
            return

        inserted = 0
        with concurrent.futures.ThreadPoolExecutor(max_workers=5) as ex:
            future_to_task = {ex.submit(self._download_poster, t[1]): t for t in tasks}
            for future in concurrent.futures.as_completed(future_to_task):
                anime_id, poster, season, episode, lang, time, entry_type = future_to_task[future]
                try:
                    poster_blob = future.result()
                except Exception:
                    poster_blob = None

                try:
                    conn = sqlite3.connect(DATA_DB, timeout=30)
                    cursor = conn.cursor()
                    cursor.execute('''
                        INSERT INTO home (anime, poster, season, episode, lang, next_update, type)
                        VALUES ((SELECT id FROM titles WHERE anime_id = ?), ?, ?, ?, ?, ?, ?)
                    ''', (anime_id, poster_blob, season, episode, lang, time, entry_type))
                    conn.commit()
                    inserted += 1
                except Exception as e:
                    self.logger.error(f"DB Insert-Fehler für Anime-ID {anime_id}: {e}")
                finally:
                    try:
                        conn.close()
                    except Exception:
                        pass

        self.logger.info(f"{inserted} neue Home-Einträge wurden eingefügt.")

    def scrape_home_screen(self):
        resp = self.scraper.get(self.base_url)
        content = resp.text
        soup = BeautifulSoup(content, 'html.parser')

        result = {
            'slider': {},
            'popular': {},
            'new_episodes': {},
            'new_animes': {},
            'today_anime_calendar': {},
            'currently_popular': {},
            'users_favorite': {}
        }

        seen_titles = set()

        # --- Slider Animes ---
        slider_views = soup.find_all('div', class_="homeSliderView row")
        for view in slider_views:
            items = view.find_all("div", class_="homeContentPromotionBoxPicture")
            for item in items:
                h3 = item.find("h3")
                if h3.get_text() == "Animeverschiebungen":
                    continue
                h4 = item.find("h4")

                title = h3.get_text(strip=True) if h3 else ""
                subtitle = h4.get_text(strip=True) if h4 else ""
                season = None
                episode = None
                if subtitle:
                    if not re.search(r"Season\s*\d+|St\.\s*\d+", subtitle, re.I):
                        title = f"{title} {subtitle}"

                title_key = title.lower()
                if title_key in seen_titles:
                    continue
                seen_titles.add(title_key)

                link = item.find_parent("a")["href"]
                season = None
                episode = None
                m = re.search(r"Season\s*(\d+)|St\.\s*(\d+)", subtitle or "", re.I)
                if m:
                    season = int(m.group(1) or m.group(2))
                else:
                    m2 = re.search(r"/staffel-(\d+)", link, re.I)
                    season = int(m2.group(1)) if m2 else 1

                match = re.search(r'\bEp\. (\d+)\b', subtitle)
                if match:
                    episode = match.group(1)

                img_tag = item.find("img")
                poster = img_tag.get("data-src") or img_tag.get("src") if img_tag else None
                anime_id = self.extract_id(link)

                result["slider"][title] = {
                    "id": anime_id,
                    "season": season,
                    "episode": episode,
                    "poster": poster
                }

        # --- Popular Animes ---
        popular_container = None
        for carousel in soup.find_all("div", class_="carousel"):
            header = carousel.find("h2")
            if header and "Derzeit beliebt" in header.get_text():
                popular_container = carousel.find("div", class_="previews")
                break

        if popular_container:
            for item in popular_container.find_all("div", class_="coverListItem"):
                a_tag = item.find("a")
                if not a_tag:
                    continue

                anime_id = self.extract_id(a_tag["href"])
                title_tag = item.find("h3")
                if not title_tag:
                    continue
                title = title_tag.get_text(strip=True)

                img_tag = item.find("img")
                poster = img_tag.get("data-src") if img_tag else None

                season_match = re.search(r"/season-(\d+)", a_tag["href"])
                season = int(season_match.group(1)) if season_match else 1

                if any(title.lower() == t.lower() for t in result["popular"]):
                    continue

                result["popular"][title] = {
                    "id": anime_id,
                    "season": season,
                    "poster": poster
                }

        # --- New Animes (drittes Carousel) ---
        new_animes_container = None
        for carousel in soup.find_all("div", class_="carousel"):
            header = carousel.find("h2")
            if header and "Neue Animes" in header.get_text():
                new_animes_container = carousel.find("div", class_="previews")
                break

        if new_animes_container:
            for item in new_animes_container.find_all("div", class_="coverListItem"):
                a_tag = item.find("a")
                if not a_tag:
                    continue

                anime_id = self.extract_id(a_tag["href"])
                title_tag = item.find("h3")
                if not title_tag:
                    continue
                title = title_tag.get_text(strip=True)

                # Season aus 'title'-Attribut extrahieren
                full_title = a_tag.get("title", "")
                season = None
                m = re.search(r"Season\s*(\d+)|St\.\s*(\d+)", full_title, re.I)
                if m:
                    season = int(m.group(1) or m.group(2))
                else:
                    season_match = re.search(r"/staffel-(\d+)", a_tag["href"], re.I)
                    season = int(season_match.group(1)) if season_match else 1

                img_tag = item.find("img")
                poster = img_tag.get("data-src") if img_tag else None

                if any(title.lower() == t.lower() for t in result["new_animes"]):
                    continue

                result["new_animes"][title] = {
                    "id": anime_id,
                    "season": season,
                    "poster": poster,
                }

        # --- New Episodes ---
        new_episode_element = soup.find('div', class_='newEpisodeList')
        if new_episode_element:
            elements = new_episode_element.find_all('div', class_='row')
            for element in elements:
                try:
                    strong_tag = element.find('strong')
                    title = strong_tag.get_text() if strong_tag else None
                    if not title:
                        continue

                    a_tag = element.find('a')
                    if not a_tag or not a_tag.get("href"):
                        continue
                    anime_id = self.extract_id(a_tag["href"])

                    span_tag = element.find('span', class_='listTag bigListTag blue2')
                    if span_tag:
                        text = span_tag.get_text(strip=True)
                        pattern = r"S(\d{2})\s*E(\d{2})"
                        text = re.sub(pattern, lambda m: f"Staffel {int(m.group(1))} Episode {int(m.group(2))}", text)
                        text_parts = text.split(" ")
                        episode = text_parts[-1] if len(text_parts) > 0 else None
                        season = text_parts[-3] if len(text_parts) >= 3 else None
                    else:
                        episode = None
                        season = None

                    img_tag = element.find('img')
                    lang = None
                    if img_tag:
                        data_src = img_tag.get('data-src')
                        if data_src:
                            lang = data_src.removesuffix('.svg').split('/')[-1]

                    result['new_episodes'][title] = {
                        "id": anime_id,
                        "episode": episode,
                        "season": season,
                        "lang": lang,
                    }
                except Exception as e:
                    self.logger.warning(f"Error processing new episode item: {e}")
                    continue

        # --- Anime Calendar ---
        calendar_elements = soup.find_all('div', id=re.compile(r'animekalender-\d+'))
        for element in calendar_elements:
            if element.get('style') and 'display: none' in element['style']:
                continue

            a_tags = element.find_all('a')
            for a_tag in a_tags:
                try:
                    anime_id = self.extract_id(a_tag["href"]) if a_tag.get("href") else None
                    if not anime_id:
                        continue

                    title_tag = a_tag.find('h3')
                    title = title_tag.get_text(strip=True) if title_tag else None
                    if not title:
                        continue

                    h_tags = a_tag.find_all('h4')
                    if len(h_tags) < 3:
                        continue

                    time = h_tags[0].get_text().replace('~ ', '').replace("Uhr", "").strip()
                    season = h_tags[1].get_text().replace("Staffel", "").strip()
                    episode = h_tags[2].get_text().replace("Episode", "").strip()

                    img_tag = a_tag.find('img')
                    lang = None
                    if img_tag:
                        src = img_tag.get('src') or img_tag.get('data-src')
                        if src:
                            lang = src.removesuffix('.svg').split('/')[-1]

                    result['today_anime_calendar'][title] = {
                        "id": anime_id,
                        "season": season,
                        "episode": episode,
                        "lang": lang,
                        "time": time,
                    }
                except Exception as e:
                    self.logger.warning(f"Error processing calendar item: {e}")
                    continue

        # currently popular
        currently_popular = soup.find_all('div', class_='carousel')
        if currently_popular:
            carousel = currently_popular[-1]
            containers = carousel.find_all('div', class_='preview')
            for container in containers:
                items = container.find_all('div', class_='coverListItem')
                for element in items:
                    try:
                        a_tag = element.find('a')
                        if not a_tag or not a_tag.get("href"):
                            continue
                        anime_id = self.extract_id(a_tag["href"])

                        title_tag = element.find('h3')
                        title = title_tag.get_text(strip=True) if title_tag else None
                        if not title:
                            continue

                        img_tag = element.find('img')
                        poster = img_tag.get('data-src') if img_tag else None

                        result['currently_popular'][title] = {
                            "id": anime_id,
                            "poster": poster,
                        }
                    except Exception as e:
                        self.logger.warning(f"Error processing currently popular item: {e}")
                        continue

        # users favourite
        all_sliders = soup.select('.homeSliderView')

        for slider in all_sliders:
            anime_boxes = slider.select('.homeContentPromotionBoxPicture')

            for box in anime_boxes:
                try:
                    a_tag = box.find_parent("a")
                    if not a_tag or not a_tag.get("href"):
                        continue

                    img = box.find("img")
                    title_tag = box.find("h3")

                    anime_id = self.extract_id(a_tag["href"])
                    poster = img.get("data-src") or img.get("src") if img else None
                    title = title_tag.get_text(strip=True) if title_tag else None

                    if not title:
                        continue

                    result['users_favorite'][title] = {
                        "id": anime_id,
                        "poster": poster,
                    }
                except Exception as e:
                    self.logger.warning(f"Error processing users favorite item: {e}")
                    continue
        self.home = result
        db_thread = threading.Thread(target=self.add_home_to_database, name="add_home_to_db")
        db_thread.start()
        return result, db_thread

    # Search for an anime by name
    def search_anime(self, anime):
        try:
            self.logger.info(f"Searching for anime {anime}")
            resp = self.scraper.get(urljoin(self.base_url, "animes"))
            soup = BeautifulSoup(resp.text, 'html.parser')

            # get all animes of aniworld.to
            animes = []
            for element in soup.find_all("div", class_="genre"):
                ul = element.find("ul")
                if not ul:
                    continue
                for li in ul.find_all("li"):
                    a = li.find("a")
                    if a:
                        animes.append(a.get_text(strip=True))
                        self.insert_title(a.get_text(strip=True), a['href'].split('/')[-1])

            # convert to lower case for better matching
            anime_lower = anime.lower()
            animes_lower = list(map(str.lower, animes))

            # Improved matching strategy to reduce false positives
            matches = []

            # 1. Exact matches
            exact_matches = [anime for anime in animes if anime.lower() == anime_lower]
            matches.extend(exact_matches)

            # 2. Substring matches
            substring_matches = [anime for anime in animes if anime_lower in anime.lower()]
            matches.extend(substring_matches)

            # 3. Fuzzy matching with better threshold to reduce false positives
            already_found = {anime.lower() for anime in matches}
            fuzzy_results = process.extract(anime_lower, animes_lower, scorer=fuzz.ratio, limit=20)

            for match_text, score, index in fuzzy_results:
                if score > 70 and match_text not in already_found:
                    matches.append(animes[index])

            # 4. Token-based matching for compound titles (like "re:zero")
            # Split search term and check if all tokens appear in the title
            search_tokens = re.split(r'[:\s\-_]+', anime_lower.strip())
            search_tokens = [token for token in search_tokens if len(token) > 1]  # should ignore single chars like that

            if len(search_tokens) > 1:
                token_matches = []
                for anime in animes:
                    anime_lower_check = anime.lower()
                    if anime_lower_check not in already_found:
                        if all(token in anime_lower_check for token in search_tokens):
                            token_matches.append(anime)
                matches.extend(token_matches)

            # Remove dups
            unique_matches = list(dict.fromkeys(matches))
            return self.convert_to_id(unique_matches)

        except Exception as e:
            self.logger.error(f"Error while searching for anime: {e}")
            return []

    # scrape info of an anime
    def scrape_info(self, content):
        '''
        Scrape info of an anime from the given HTML content
        1. Title
        2. Description
        3. Genres
        4. Poster URL
        :param content: HTML content of the anime page
        :return: Nothing
        '''
        try:
            self.logger.info("Start getting info about anime")
            soup = BeautifulSoup(content, 'html.parser')

            # get title
            title_tag = soup.find('h1')
            self.info['title'] = title_tag.get_text(strip=True) if title_tag else "N/A"
            self.insert_title(self.info['title'], self.anime)

            # get description
            desc_tag = soup.find('p', class_='seri_des')
            self.info['description'] = desc_tag.get_text(strip=True) if desc_tag else "N/A"

            # get genres
            genres_tag = soup.find('div', class_='genres')
            elements = genres_tag.find('ul').find_all('li') if genres_tag else []
            genres = [el.get_text(strip=True) for el in elements]
            for genre in genres:
                pattern = r'^\s*\+\s*\d+\s*$'
                if re.match(pattern, genre):
                    genres.remove(genre)
            self.info['genres'] = genres

            # get poster
            poster = soup.find('div', class_='seriesCoverBox')
            img_tag = poster.find('img') if poster else None
            self.info['poster'] = img_tag['data-src'] if img_tag and 'data-src' in img_tag.attrs else "N/A"

            # get max seasons
            season_links = soup.select('#stream ul li a')
            regular_seasons = [a for a in season_links if "staffel-" in a['href'] and "/episode-" not in a['href']]
            self.max_seasons = len(regular_seasons)

            # has movie
            self.has_movie = any("filme" in a['href'].lower() for a in season_links)

            self.logger.info(f"Finished getting info")
        except Exception as e:
            self.logger.error(f"Error while scraping info: {e}")

    # Get redirect links for a specific episode and season
    def get_redirect_links(self, episdoe, season):
        '''
        Get redirect links for a specific episode and season
        1. Check if anime is set
        2. Update episode count if season changed
        3. Validate season and episode numbers
        4. Scrape the streaming links
        5. Return the links in a structured format
        :param episdoe: episode number
        :param season: season number
        :return: redirect links of the episode in a dict
        '''
        # Check if anime is set
        if self.anime is None:
            self.logger.error("Anime not set. Please set the anime before fetching episode streams.")
            return None

        # Validate season and episode numbers
        if season > int(self.max_seasons):
            self.logger.error("Requested season or episode exceeds available limits.")
            return None
        try:
            resp = self.scraper.get(
                urljoin(self.base_url, f"/anime/stream/{self.anime}/staffel-{season}/episode-{episdoe}"))
            content = resp.text

            # Language keys mapping
            lang_keys = {
                '1': 'de',
                '2': 'en-sub',
                '3': 'de-sub'
            }
            soup = BeautifulSoup(content, 'html.parser')
            # get the streaming element list
            ul_element = soup.find('ul', class_="row")
            elements = ul_element.find_all('li') if ul_element else []

            # streams dict
            streams = {
                'de': [],
                'en-sub': [],
                'de-sub': []
            }

            for element in elements:
                # extract data attributes
                lang = element.attrs['data-lang-key']
                if lang in lang_keys:
                    lang_code = lang_keys[lang]
                    redirect = element.attrs['data-link-target']
                    server_name = element.find('h4').get_text(strip=True) if element.find('h4') else "N/A"
                    streams[lang_code].append({
                        'server': server_name,
                        'url': redirect
                    })
            return streams
        except Exception as e:
            self.logger.error(f"Error while fetching episode streams: {e}")
            return None

    # follows redirects to get the final streaming link
    def follow_redirect(self, url):
        '''
        Follow redirects to get the final streaming link
        :param url: Redirect URL
        :return: Final URL
        '''
        resp = self.scraper.get(url)
        return resp.url

    # get the streaming link
    def get_streaming_link(self, data):
        '''
        Get the streaming link by following redirects
        1. Check if server is Vidmoly, if yes follow redirect directly and return the link
        2. For other servers, use Selenium to load the page and capture all requests
        3. Filter requests to find m3u8 links
        4. Return the list of m3u8 links
        :param data: the data of the stream
        :return: m3u8 links or None
        '''
        # get server and url
        server = data.get('server')
        url = data.get('url')

        self.logger.info(f"Processing {server} server")

        # if server is vidmoly, follow redirect directly and return it
        if server == "Vidmoly":
            # Because Vidmoly uses Embeded HTML Video its enough to only get the link of the website
            redirect_url = self.follow_redirect(urljoin(self.base_url, url))
            self.logger.info(f"Vidmoly redirect URL: {redirect_url}")
            return redirect_url

        # Special handling for VOE servers
        if "VOE" in server or "voe" in server.lower():
            return self._handle_voe_server(url)

        # For Filmoon and other servers
        return self._handle_generic_server(url, server)

    def _handle_generic_server(self, url, server):
        '''
        Handle generic servers like Filmoon using Selenium
        :param url: Redirect URL
        :param server: Server name
        :return: m3u8 links list or None
        '''
        try:
            # Use Selenium to capture requests
            self.logger.info(f"Following redirects for {server}: {url}")
            from seleniumwire import webdriver
            from selenium.webdriver.chrome.service import Service
            from webdriver_manager.chrome import ChromeDriverManager
            from selenium.webdriver.common.by import By
            from selenium.webdriver.common.action_chains import ActionChains
            import time

            # setup selenium options
            options = webdriver.ChromeOptions()
            options.add_argument('--disable-blink-features=AutomationControlled')
            options.add_argument('--headless=new')
            options.add_argument("--disable-gpu")
            options.add_argument("--no-sandbox")
            options.add_argument("--disable-dev-shm-usage")

            # Initialize the webdriver
            driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)

            # Clear previous requests
            del driver.requests

            redirect_url = self.follow_redirect(f"{self.base_url}{url}")
            self.logger.info(f"{server} redirect URL: {redirect_url}")

            driver.get(redirect_url)
            time.sleep(3)  # Wait for page to load

            # get the m3u8 links from the requests
            m3u8_links = []
            for request in driver.requests:
                if '.m3u8' in request.url:
                    m3u8_links.append(request.url)
                    self.logger.info(f"Found m3u8 link: {request.url}")

            driver.quit()
            return m3u8_links if m3u8_links else None
        except Exception as e:
            self.logger.error(f"Error while getting streaming link for {server}: {e}")
            return None

    def _handle_voe_server(self, url):
        '''
        Optimized VOE server handling - faster stream detection
        :param url: Redirect URL
        :return: m3u8 links list or None
        '''
        try:
            self.logger.info(f"Handling VOE server: {url}")
            from seleniumwire import webdriver
            from selenium.webdriver.chrome.service import Service
            from webdriver_manager.chrome import ChromeDriverManager
            from selenium.webdriver.common.by import By
            import time

            # Optimized selenium options for speed
            options = webdriver.ChromeOptions()
            options.add_argument('--disable-blink-features=AutomationControlled')
            options.add_argument('--headless=new')
            options.add_argument("--disable-gpu")
            options.add_argument("--no-sandbox")
            options.add_argument("--disable-dev-shm-usage")
            options.add_argument("--disable-logging")
            options.add_argument("--log-level=3")
            options.add_argument("--silent")
            options.add_argument("--disable-extensions")
            options.add_argument("--disable-plugins")
            options.add_argument("--disable-images")  # Don't load images for speed

            # Initialize the webdriver
            driver = webdriver.Chrome(service=Service(ChromeDriverManager().install()), options=options)

            # Clear previous requests
            del driver.requests

            redirect_url = self.follow_redirect(f"{self.base_url}{url}")
            self.logger.info(f"VOE redirect URL: {redirect_url}")

            driver.get(redirect_url)
            time.sleep(3)  # Reduced wait time

            def find_and_click_play_button():
                """Find and click play button quickly"""
                # Reduced selector list - most common VOE selectors only
                play_selectors = [
                    'div[class*="play"]',
                    'button[class*="play"]',
                    '.vjs-big-play-button',
                    '.jw-display-icon-container'
                ]

                for selector in play_selectors:
                    try:
                        elements = driver.find_elements(By.CSS_SELECTOR, selector)
                        for element in elements:
                            if element.is_displayed():
                                try:
                                    element.click()
                                    self.logger.info(f"Clicked play button: {selector}")
                                    return True
                                except:
                                    try:
                                        driver.execute_script("arguments[0].click();", element)
                                        return True
                                    except:
                                        continue
                    except:
                        continue
                return False

            # Optimized clicking sequence
            self.logger.info("Starting VOE click sequence...")
            max_clicks = 5  # Reduced from 7

            for attempt in range(max_clicks):
                if find_and_click_play_button():
                    self.logger.info(f"Click {attempt + 1}/{max_clicks}")

                    # Shorter wait between clicks
                    time.sleep(2)

                    # Early check for streams after 2 clicks
                    if attempt >= 1:
                        current_m3u8 = [r for r in driver.requests if '.m3u8' in r.url.lower()]
                        if current_m3u8:
                            self.logger.info(f"Found m3u8 after {attempt + 1} clicks, stopping...")
                            break
                else:
                    time.sleep(1)  # Reduced wait

            # Shorter final wait
            time.sleep(4)

            # Quick scan for m3u8 links in network requests
            m3u8_links = []

            for request in driver.requests:
                url_lower = request.url.lower()

                # Focus only on m3u8 links for speed
                if '.m3u8' in url_lower:
                    m3u8_links.append(request.url)
                    self.logger.info(f"Found VOE m3u8 link: {request.url}")

                # Quick check for JWPlayer
                elif 'jwplayer' in url_lower:
                    self.logger.info(f"Found JWPlayer request: {request.url}")

            driver.quit()

            if m3u8_links:
                return list(set(m3u8_links))  # Remove duplicates
            else:
                self.logger.warning("No m3u8 streams found for VOE")
                return None

        except Exception as e:
            self.logger.error(f"Error while handling VOE server: {e}")
            return None

    # Main method to scrape an anime by id
    def scrape_anime(self, anime):
        try:
            self.logger.info(f"Starte Scraping für Anime: {anime}")
            self.anime = anime
            response = self.scraper.get(urljoin(self.base_url, f"/anime/stream/{anime}/staffel-1"))
            content = response.text

            self.scrape_info(content)
            poster_data = self.scraper.get(urljoin(self.base_url, self.info['poster'])).content if self.info[
                                                                                                       'poster'] != "N/A" else "N/A"
            self.insert_anime_data(anime, self.info['description'], self.info['genres'], poster_data, self.max_seasons)

            return self.info, self.max_seasons
        except Exception as e:
            self.logger.error(f"Fehler beim Scrapen des Animes: {e}")


if __name__ == "__main__":
    import time

    api = AniWorldAPI()

    from pprint import pprint
    pprint(api.scrape_home_screen(), sort_dicts=False)
    """
    # Search for anime
    print("=== Anime Search ===")
    data = api.search_anime("Re:Zero")
    print(f"Found anime matches: {data[:5] if data else 'None'}")  # Show first 5 matches

    # Scrape anime info
    print("\n=== Anime Info ===")
    result = api.scrape_anime("dan-da-dan")
    if result:
        info, max_episodes, max_seasons = result
        print(f"Title: {info.get('title', 'N/A')}")
        print(f"Max Episodes: {max_episodes}, Max Seasons: {max_seasons}")
    else:
        print("Failed to scrape anime info")
        exit()

    # Get streaming links
    print("\n=== Episode Streaming Links ===")
    episode_data = api.get_redirect_links(1, 2)  # Episode 1, Season 1 ? I guess
    if not episode_data:
        print("Failed to get episode data")
        exit()

    print("Available streaming servers:")
    for lang, servers in episode_data.items():
        if servers:
            print(f"\n{lang.upper()} ({len(servers)} servers):")
            for i, server in enumerate(servers):
                print(f"  {i + 1}. {server['server']}")

    # Test different streaming services
    print("\n=== Testing Streaming Services ===")

    for lang, servers in episode_data.items():
        if not servers:
            continue

        print(f"\n--- {lang.upper()} Language ---")
        for server_data in servers:
            server_name = server_data['server']
            print(f"\nTesting {server_name}...")

            try:
                stream_result = api.get_streaming_link(server_data)

                if stream_result:
                    if isinstance(stream_result, str):
                        # Vidmoly returns a direct URL
                        print(f"  ✓ {server_name}: {stream_result}")
                    elif isinstance(stream_result, list):
                        # Other servers return m3u8 links list
                        print(f"  ✓ {server_name}: Found {len(stream_result)} m3u8 link(s)")
                        for i, link in enumerate(stream_result, 1):
                            print(f"    {i}. {link}")
                    else:
                        print(f"  ✓ {server_name}: {stream_result}")
                else:
                    print(f"  ✗ {server_name}: No streaming links found")

            except Exception as e:
                print(f"  ✗ {server_name}: Error - {e}")

            # Add delay between requests to avoid rate limiting
            time.sleep(2)

"""