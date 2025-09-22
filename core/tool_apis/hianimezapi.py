import re

import requests

from core.tool_apis.hianimez_setup import HianimezSetup


class HianimezAPI:
    def __init__(self):
        self.hz_setup = HianimezSetup()
        self.base_url = "http://localhost:4000"

    def setup_all(self):
        self.hz_setup.clone_repo()
        self.hz_setup.install_dependencies()
        self.hz_setup.downgrade_pino()
        self.hz_setup.build_api()

    def start_server(self):
        self.hz_setup.start_server()

    def stop_server(self):
        self.hz_setup.server_stop()

    def get_all_info_ids(self, data):
        animes = {}
        for key, ids in data.items():
            anime_info = []
            for id in ids:
                res = self.get_anime_info(id)
                print(id, res)
                anime_info.append(res)
            animes[key] = anime_info
        return animes

    def home_screen(self):
        if not self.hz_setup.server_running():
            print("Server not running")
            return []
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/home")
            return response.json()
            return data_list
        except Exception as e:
            print("Error in home_screen:", e)
            return []

    def get_anime_a_z(self, page: int = 1, sortoption: str = "0-9"):
        pattern = r'^(all|other|0-9|[a-zA-Z])$'
        if not re.match(pattern, sortoption):
            print("Invalid sortoption")
            return None
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/azlist/{sortoption}?page={page}")
            return response.json()
        except Exception as e:
            print("Error in get_anime_a_z:", e)
            return None

    def get_anime_qtip(self, id):
        if not self.hz_setup.server_running():
           print("Server not running")
           return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/qtip/{id}")
            return response.json()
        except Exception as e:
            print("Error in get_anime_qtip:", e)
            return None

    def get_anime_info(self, id):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/anime/{id}")
            return response.json()
        except Exception as e:
            print("Error in get_anime_info:", e)
            return None

    def search_anime_easy(self, anime, page: int = 1):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/search?q={anime}&page={page}")
            return response.json()
        except Exception as e:
            print("Error in search_anime_easy:", e)
            return None

    def search_anime_advanced(self, parameters: dict = None):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        url = f"{self.base_url}/api/v2/hianime/search?"
        for parameter, query in parameters.items():
            url += parameter + "=" + str(query)
            url += "&"
        print("Request URL:", url)
        try:
            response = requests.get(url)
            return response.json()
        except Exception as e:
            print("Error in search_anime_advanced:", e)
            return None

    def search_suggestion(self, anime):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/search/suggestion?q={anime}")
            return response.json()
        except Exception as e:
            print("Error in search_suggestion:", e)
            return None

    def search_producer(self, producer, page: int = 1):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/producer/{producer}?page={page}")
            print(response.json())
            return response.json()
        except Exception as e:
            print("Error in search_producer:", e)
            return None

    def get_category_anime(self, category, page: int = 1):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/category/{category}?page={page}")
            return response.json()
        except Exception as e:
            print("Error in get_category_anime:", e)
            return None

    def get_estimated_schedules(self, date):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/schedule?date={date}")
            return response.json()
        except Exception as e:
            print("Error in get_estimated_schedules:", e)
            return None

    def get_anime_episodes(self, anime_id):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/anime/{anime_id}/episodes")
            return response.json()
        except Exception as e:
            print("Error in get_anime_episodes:", e)
            return None

    def get_anime_next_episode(self, anime_id):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/anime/{anime_id}/next-episode-schedule")
            return response.json()
        except Exception as e:
            print("Error in get_anime_next_episode:", e)
            return None

    def get_anime_episode_server(self, episode_id):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(f"{self.base_url}/api/v2/hianime/episode/servers?animeEpisodeId={episode_id}")
            return response.json()
        except Exception as e:
            print("Error in get_anime_episode_server:", e)
            return None

    def get_streaming_link(self, episode_id, server_name="hd-1", category="sub"):
        if not self.hz_setup.server_running():
            print("Server not running")
            return None
        try:
            response = requests.get(
                f"{self.base_url}/api/v2/hianime/episode/sources?animeEpisodeId={episode_id}&server={server_name}&category={category}"
            )
            return response.json()
        except Exception as e:
            print("Error in get_streaming_link:", e)
            return None


# Beispiel-Nutzung
if __name__ == "__main__":
    api = HianimezAPI()
    api.start_server()
    from pprint import pprint as p
    data = api.home_screen()
    p(data)
    api.stop_server()