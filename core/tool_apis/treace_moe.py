import os
import typing
import urllib.parse
from pathlib import Path

import requests
from gql import gql, Client
from gql.transport.requests import RequestsHTTPTransport

from models.TraceMoe import TraceMoe
from utils.anilex_helper import get_trace_moe_key, get_auth_key
from utils.anilex_helper import load_settings, get_query_path

transport = RequestsHTTPTransport(
    url="https://graphql.anilist.co",
    verify=True,
    retries=2,
    headers={
        "Authorization": f"Bearer {get_auth_key()}",
        "Content-Type": "application/json"
    }
)

URL = "https://api.trace.moe/search"


def read_graphql_query(file_path: str) -> str:
    with open(file_path, 'r', encoding='utf-8') as file:
        return file.read()


def get_user_data_tile(data):
    title = data['USER_SETTINGS']['title']
    true_title = next((k for k, v in title.items() if v), None)
    return true_title


class TraceMoeAPI:
    def __init__(self, mute: bool = False, use_key: bool = False):
        self.mute: bool = mute
        self.data: typing.Union[TraceMoe, None] = None
        response = requests.get("https://api.trace.moe/me").json()
        self.limit = int(response.get("quota", 1000)) - int(response.get("quotaUsed", 0))
        self.use_key = use_key
        self.api_key = get_trace_moe_key()

    @staticmethod
    def get_file_data(path: Path):
        with open(path, "rb") as f:
            return f.read()

    @staticmethod
    def convert_seconds(seconds: float) -> str:
        total_seconds = int(round(seconds))
        minutes = total_seconds // 60
        secs = total_seconds % 60
        return f"{minutes:02}:{secs:02}"

    def get_anilist_data(self, anilist_id: int):
        """Fetch detailed data from AniList API for a specific anime"""
        query = read_graphql_query(os.path.join(get_query_path(), "trace_moe_title.graphql"))
        client = Client(transport=transport, fetch_schema_from_transport=False)
        params = {"id": anilist_id}
        try:
            result = client.execute(gql(query), variable_values=params)
            return result["Media"]
        except Exception as e:
            print(f"Error fetching data from Anilist: {e}")
            return None

    def populate_anime_details(self, anime: TraceMoe):
        """Populate detailed information for a specific anime object"""
        if anime.data_fetched:
            print(f"Data already fetched for anime ID {anime.id}")
            return True

        if not anime.id:
            print("No AniList ID available to fetch details")
            return False

        try:
            anilist_data = self.get_anilist_data(anime.id)
            if not anilist_data:
                print(f"Failed to get AniList data for ID {anime.id}")
                return False

            anime.title = anilist_data.get("title", {}).get("romaji") or anilist_data.get("title", {}).get(
                "english") or "Unknown Title"

            cover_image = anilist_data.get("coverImage", {})
            anime.anilist_image = cover_image.get("extraLarge") or cover_image.get("large") or cover_image.get("large")

            anime.data_fetched = True
            return True
        except Exception as e:
            print(f"Error populating anime details: {str(e)}")
            return False

    @staticmethod
    def parse_data(data):
        print(data)
        if data["error"] != "":
            return TraceMoe(data["error"])
        data_list = []
        user_data = load_settings()
        for dat in data["result"]:
            temp = TraceMoe()
            temp.id = dat["anilist"]
            temp.duration = int(dat["duration"]) / 60
            temp.episode = dat["episode"]
            temp.from_ = TraceMoeAPI.convert_seconds(dat["from"])
            temp.to = TraceMoeAPI.convert_seconds(dat["to"])
            temp.similarity = round(float(dat["similarity"]) * 100, 2)
            temp.video = dat["video"]
            temp.image = dat["image"]
            data_list.append(temp)

        return data_list

    @staticmethod
    def is_url(string):
        from urllib.parse import urlparse
        try:
            result = urlparse(string)
            return all([result.scheme in ('https', 'https', 'ftp"'), result.netloc])
        except Exception:
            return False

    @staticmethod
    def is_file(string):
        return os.path.exists(string) or os.path.isabs(string)

    def get_data(self, string):
        if self.limit == 0:
            return "Limit exceeded!"
        if TraceMoeAPI.is_url(string):
            params = []
            if self.use_key and self.api_key:
                params.append(f"key={self.api_key}")
            params.append(f"url={urllib.parse.quote_plus(string)}")
            if self.mute:
                params.append("mute")
            url = f"{URL}?{'&'.join(params)}"
            data = requests.get(url).json()
            return TraceMoeAPI.parse_data(data)
        elif TraceMoeAPI.is_file(string):
            files = {"image": TraceMoeAPI.get_file_data(Path(string))}
            params = []
            if self.use_key and self.api_key:
                params.append(f"key={self.api_key}")
            if self.mute:
                params.append("mute")
            post_url = f"{URL}?{'&'.join(params)}"
            data = requests.post(post_url, files=files).json()
            return TraceMoeAPI.parse_data(data)
        else:
            return "Invalid input!"

    def check_limit(self):
        return self.limit


if __name__ == "__main__":
    api = TraceMoeAPI()
    data = api.get_data("demo.png")
    for d in data:
        print(d)