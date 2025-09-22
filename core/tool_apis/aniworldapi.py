import logging
import os
import re
import sqlite3
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
        self.current_max_episodes = 0
        self.current_season = 1
        self.max_seasons = 0
        self.anime: Optional[str] = None
        self.info = {
            'title': "",
            'description': "",
            'genres': [],
            'poster': "",
        }

        # Logging setup
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s- %(levelname)s - %(message)s'
        )
        self.logger = logging.getLogger(__name__)
        self.create_data_db()
        self.create_title_db()

    # ---------------------------
    # Getter methods
    # ---------------------------
    def max_episodes(self):
        return self.current_max_episodes

    def max_seasons(self):
        return self.max_seasons

    def current_season(self):
        return self.current_season

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
    def create_title_db():
        if not os.path.exists(DATA_DB):
            with open(DATA_DB, 'w') as file:
                pass
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS titles (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT,
                anime_id TEXT
            )
        ''')
        conn.commit()
        conn.close()

    @staticmethod
    def create_home_db():
        """
        not final db structure
        :return: Nothing
        """
        if not os.path.exists(DATA_DB):
            with open(DATA_DB, 'w') as file:
                pass
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS home (
                id TEXT PRIMARY KEY,
                title TEXT,
                season TEXT,
                episode TEXT, 
                poster BLOB,
                release_date TEXT,
                release_time TEXT,
                category TEXT,
            )
        ''')

    @staticmethod
    def create_data_db():
        if not os.path.exists(DATA_DB):
            with open(DATA_DB, 'w') as file:
                pass
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()
        cursor.execute('''
            CREATE TABLE IF NOT EXISTS anime_data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                anime_id TEXT,
                anime_title TEXT,
                anime_description TEXT,
                anime_genres TEXT,
                anime_poster_data BLOB,
                max_episodes INTEGER,
                max_seasons INTEGER,
                last_updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP
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

    @staticmethod
    def insert_anime_data(self, anime_id, anime_title, anime_description, anime_genres, anime_poster_data, max_episodes, max_seasons):
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

        cursor.execute("SELECT 1 FROM anime_data WHERE anime_title = ?", (anime_title,))
        exists = cursor.fetchone()
        if exists:
            conn.close()
            self.logger.info(f"Anime {anime_title} already exists in database. Skipping insert.")
            return

        cursor.execute('''
            INSERT INTO anime_data (anime_id, anime_title, anime_description, anime_genres, anime_poster_data, max_episodes, max_seasons)
            VALUES (?, ?, ?, ?, ?, ?, ?)
        ''', (anime_id, anime_title, anime_description, anime_genres, anime_poster_data, max_episodes, max_seasons))
        conn.commit()
        conn.close()


    @staticmethod
    def extract_id(link: str) -> str:
        m = re.search(r"/anime/stream/([^/]+)", link)
        if m:
            return m.group(1)
        return None

    @staticmethod
    def write_home_db(data: dict):
        """
        Write home data to database for faster retrieval of data and not calling the website to many times
        :param data: Home Screen scraped data
        :return: Nothing
        """
        conn = sqlite3.connect(DATA_DB)
        cursor = conn.cursor()

    # ---------------------------
    # Core methods
    # ---------------------------

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
                h4 = item.find("h4")

                title = h3.get_text(strip=True) if h3 else ""
                subtitle = h4.get_text(strip=True) if h4 else ""
                if subtitle:
                    if not re.search(r"Season\s*\d+|St\.\s*\d+", subtitle, re.I):
                        title = f"{title} {subtitle}"

                title_key = title.lower()
                if title_key in seen_titles:
                    continue
                seen_titles.add(title_key)

                link = item.find_parent("a")["href"]
                season = None
                m = re.search(r"Season\s*(\d+)|St\.\s*(\d+)", subtitle or "", re.I)
                if m:
                    season = int(m.group(1) or m.group(2))
                else:
                    m2 = re.search(r"/staffel-(\d+)", link, re.I)
                    season = int(m2.group(1)) if m2 else 1

                img_tag = item.find("img")
                poster = img_tag.get("data-src") or img_tag.get("src") if img_tag else None
                anime_id = self.extract_id(link)

                result["slider"][title] = {
                    "id": anime_id,
                    "season": season,
                    "poster": poster,
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
        elements = new_episode_element.find_all('div', class_='row')
        for element in elements:
            title = element.find('strong').get_text()
            a_tag = element.find('a')
            anime_id = self.extract_id(a_tag["href"])

            episode = element.find('span', class_='listTag bigListTag blue2').get_text(strip=True)
            episode.replace('S', 'Staffel')
            episode.replace('E', 'Episode')
            lang = element.find('img').get('data-src').removesuffix('.svg').split('/')[-1]

            result['new_animes'][title] = {
                "id": anime_id,
                "episode": episode,
                "lang": lang,
            }


        # --- Anime Calendar ---
        calendar_element = soup.find_all('div', id=re.compile(r'animekalender-\d+'))
        anime_id = None
        title = None
        time = None
        season = None
        episode = None
        lang = None
        for element in calendar_element:
            if element.has_attr('style'):
                continue
            a_tags = element.find_all('a')
            for a_tag in a_tags:
                anime_id = self.extract_id(a_tag["href"])
                title = a_tag.find('h3').get_text()
                h_tags = a_tag.find_all('h4')
                time = h_tags[0].get_text().replace('~ ', '').strip()
                season = h_tags[1].get_text()
                episode = h_tags[2].get_text()
                lang = a_tag.find('img').get('src').removesuffix('.svg').split('/')[-1].replace('-', ' ')
            result['today_anime_calendar'][title] = {
                "id": anime_id,
                "season": season,
                "episode": episode,
                "lang": lang,
                "time": time,
            }


        # currently popular
        currently_popular = soup.find_all('div', class_='carousel')
        carousel = currently_popular[-1]
        containers = carousel.find_all('div', class_='preview')
        for container in containers:
            items = container.find_all('div', class_='coverListItem')
            for element in items:
                a_tag = element.find('a')
                anime_id = self.extract_id(a_tag["href"])
                title = element.find('h3').get_text(strip=True)
                poster = element.find('img').get('data-src')

                result['currently_popular'][title] = {
                    "id": anime_id,
                    "poster": poster,
                }

        # users favourite
        all_sliders = soup.select('.homeSliderView')

        for slider in all_sliders:
            anime_boxes = slider.select('.homeContentPromotionBoxPicture')

            for box in anime_boxes:
                a_tag = box.find_parent("a")
                img = box.find("img")
                title_tag = box.find("h3")

                anime_id = self.extract_id(a_tag["href"])
                poster = img.get("data-src") or img.get("src") if img else None
                title = title_tag.get_text(strip=True) if title_tag else None

                result['users_favorite'][title] = {
                    "id": anime_id,
                    "poster": poster,
                }

        return result

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

            self.logger.info(f"Finished getting info")
        except Exception as e:
            self.logger.error(f"Error while scraping info: {e}")

    # Update episode count for a specific season
    def update_episode_count(self, season):
        '''
        Update the current max episodes and current season
        :param season: Current season number
        :return: Nothing
        '''
        try:
            self.logger.info(f"Updating episode count for season {season}")
            resp = self.scraper.get(urljoin(self.base_url, f"/anime/stream/{self.anime}/staffel-{season}"))
            content = resp.text
            soup = BeautifulSoup(content, 'html.parser')

            # scrape max episodes for the season
            nav_tag = soup.find('div', class_='hosterSiteDirectNav', id='stream').find_all('ul')
            episodes = nav_tag[1].find_all('li') if nav_tag else []

            # set the variables
            self.current_max_episodes = int(episodes[-1].get_text(strip=True)) if episodes else 0
            self.current_season = season
        except Exception as e:
            self.logger.error(f"Error while updating episode count: {e}")

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

        # Update episode count if season changed
        if season != int(self.current_season) and season <= int(self.max_seasons):
            self.update_episode_count(season)

        # Validate season and episode numbers
        if season > int(self.max_seasons) or int(self.current_max_episodes) < episdoe:
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
            elements = soup.find('ul', class_="row").find_all('li')

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

    # Scrape episodes and seasons count
    def scrape_episodes_seasons(self, content):
        '''
        Scrape max episodes and max seasons from the given HTML content
        1. Max Seasons
        2. Max Episodes for season 1
        3. Update class variables
        :param content: HTML content of the anime page
        :return: Nothing
        '''
        try:
            self.logger.info("Start getting episodes and seasons info")
            soup = BeautifulSoup(content, 'html.parser')

            # get max seasons
            nav_tag = soup.find('div', class_='hosterSiteDirectNav', id='stream').find_all('ul')
            seasons = nav_tag[0].find_all('li')
            self.max_seasons = seasons[-1].get_text(strip=True) if nav_tag else "N/A"
            # get max episodes for season 1
            episodes = nav_tag[1].find_all('li')
            self.current_max_episodes = episodes[-1].get_text(strip=True) if nav_tag else "N/A"

        except Exception as e:
            self.logger.error(f"Error while scraping episodes and seasons: {e}")

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
            self.scrape_episodes_seasons(content)
            poster_data = self.scraper.get(urljoin(self.base_url, self.info['poster'])).content if self.info['poster'] != "N/A" else "N/A"
            self.insert_anime_data(self, anime, self.info['title'], self.info['description'], ",".join(self.info['genres']), poster_data, self.current_max_episodes, self.max_seasons)

            return self.info, self.current_max_episodes, self.max_seasons
        except Exception as e:
            self.logger.error(f"Fehler beim Scrapen des Animes: {e}")


if __name__ == "__main__":
    import time

    api = AniWorldAPI()

    data = api.scrape_home_screen()
    print(data)
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