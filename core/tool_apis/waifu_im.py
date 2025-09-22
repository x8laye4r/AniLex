import json
import os
import typing

import requests
from waifuim import WaifuAioClient

from utils.anilex_helper import get_cache_path

PATH = os.path.join(get_cache_path(), "api_data", "waifuimapi.json")
os.makedirs(os.path.dirname(PATH), exist_ok=True)

if not os.path.exists(PATH):
    with open(PATH, "w", encoding="utf-8") as f:
        json.dump([], f)

class WaifuImAPI:
    def __init__(self, is_gif: typing.Union[bool, None] = None, tags: typing.Union[list[str], None] = None,
                 excl_tags: typing.Union[list[str], None] = None, is_nsfw: bool = None,
                 count: typing.Union[int, None] = None):
        self.is_gif = is_gif
        self.selected_tags = tags
        self.excluded_tags = excl_tags
        self.is_nsfw = is_nsfw
        self.limit = count
        self.image_bytes: list[bytes] = []
        self.image_links: list[str] = []
        self.client = WaifuAioClient()

    async def get_image_and_set_data(self):
        async with self.client as wf:
            result = await wf.search(
                included_tags=self.selected_tags,
                excluded_tags=self.excluded_tags,
                gif=self.is_gif,
                is_nsfw=self.is_nsfw,
                limit=self.limit
            )
            if isinstance(result, list):
                for data in result:
                    self.image_links.append(data)
            else:
                self.image_links = [result]

    def get_image_bytes(self):
        self.image_bytes = []
        for link in self.image_links:
            res = requests.get(link)
            self.image_bytes.append(res.content)

    def get_current_tags(self):
        url = "https://api.waifu.im/tags"
        res = requests.get(url)
        return res.json()

