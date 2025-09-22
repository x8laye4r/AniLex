import json
import os

import requests

from utils.anilex_helper import get_cache_path

PATH = os.path.join(get_cache_path(), "api_data", "nekoimageapi.json")
os.makedirs(os.path.dirname(PATH), exist_ok=True)

if not os.path.exists(PATH):
    with open(PATH, "w", encoding="utf-8") as f:
        json.dump([], f)

def read_nekos_json():
    with open(PATH, "r", encoding="utf-8") as f:
        data = json.load(f)
        print(data)
        return data

class NekoImageAPI:
    def __init__(self, tags: str = "", exc_tags: str = "", rating: str = "safe"):
        self.rating = rating
        self.selected_tags = tags
        self.exclude_tags = exc_tags
        self.url_file = "https://api.nekosapi.com/v4/images/random/file"

    def fetch_image(self):
        query = {
            "rating": self.rating,
            "tags": self.selected_tags,
            "without_tags": self.exclude_tags
        }
        try:
            res = requests.get(self.url_file, params=query)
            res.raise_for_status()
            return res.content
        except Exception as e:
            print("Fehler beim Laden des Bildes:", e)
            return None

    def fetch_images(self, count):
        images = []
        for _ in range(count):
            img = self.fetch_image()
            images.append(img)
        return images