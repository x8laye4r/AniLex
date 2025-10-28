import json
import os
import time

import requests
from old.anilex_helper import get_cache_path

PATH = os.path.join(get_cache_path(), "api_data")
os.makedirs(PATH, exist_ok=True)
FILE = os.path.join(PATH, "animechanapi.json")

if not os.path.exists(FILE):
    with open(FILE, "w", encoding="utf-8") as f:
        json.dump({"limit": 0}, f, indent=4)

def get_limit_data() -> int:
    try:
        with open(FILE, "r", encoding="utf-8") as f:
            data = json.load(f)
        val = data.get("limit", 0)
        return int(val)
    except (FileNotFoundError, ValueError, TypeError):
        return 0

class AnimeChanAPI:
    def __init__(self, request_limit: int = 20):
        self.base_uri = "https://api.animechan.io/v1/"
        self.max_limit = int(request_limit)
        used = get_limit_data()
        self.limit = max(self.max_limit - used, 0)
        print("Initial remaining limit:", self.limit)

    @staticmethod
    def parse_data(data):
        if not data:
            return None

        if data.get("status") == "success":
            data_dict = data.get("data")
            return {
                "Character": data_dict["character"]["name"],
                "Anime": data_dict["anime"]["name"],
                "Quote": data_dict["content"]
            }
        else:
            return None

    def check_api_limit(self):
        try:
            mtime = os.path.getmtime(FILE)
            age = time.time() - mtime
        except OSError:
            age = float("inf")

        if age > 3600:
            with open(FILE, "w", encoding="utf-8") as f:
                json.dump({"limit": 0}, f, indent=4)
            self.limit = self.max_limit
            return True

        used = self.max_limit - self.limit
        try:
            with open(FILE, "w", encoding="utf-8") as f:
                json.dump({"limit": int(used)}, f, indent=4)
        except OSError:
            return False
        return True

    def get_random_quote(self):
        if self.limit <= 0:
            print("API-Limit erreicht (in-memory).")
            return None
        uri = f"{self.base_uri}quotes/random"
        try:
            res = requests.get(uri, timeout=10)
            res.raise_for_status()
            payload = res.json()
        except (requests.RequestException, ValueError) as e:
            print("HTTP/JSON error:", e)
            return None

        data = self.parse_data(payload)
        if data is not None:
            self.limit -= 1
            self.check_api_limit()
        return data

    def get_quote_by_anime(self, anime: str = ""):
        if not anime:
            return None
        if self.limit <= 0:
            print("API-Limit reached (in-memory).")
            return None

        uri = f"{self.base_uri}quotes/anime?title={requests.utils.quote(anime)}"
        try:
            res = requests.get(uri, timeout=10)
            res.raise_for_status()
            payload = res.json()
        except (requests.RequestException, ValueError) as e:
            print("HTTP/JSON error:", e)
            return None

        data = self.parse_data(payload)
        if data is not None:
            self.limit -= 1
            self.check_api_limit()
        return data

# --- Beispiel
if __name__ == "__main__":
    api = AnimeChanAPI()
    print(api.get_random_quote())
