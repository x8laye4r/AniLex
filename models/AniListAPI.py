import logging
import os
from logging.handlers import RotatingFileHandler
from time import sleep

import httpx
from requests.exceptions import HTTPError, Timeout, RequestException

from utils.anilex_helpers_v2 import get_auth_key, get_cache_dir


class AniListAPI:
    def __init__(self):
        self.base_url = "https://graphql.anilist.co"
        self.headers = {
            "Authorization": f"Bearer {get_auth_key("x8laye4r")}",
            "Content-Type": "application/json"
        }
        self.requests_left = 30

        log_dir = os.path.join(get_cache_dir(), "logs")
        os.makedirs(log_dir, exist_ok=True)
        path = os.path.join(log_dir, "AniListAPI.log")

        self.logger = logging.getLogger("AniListAPI")
        self.logger.setLevel(logging.DEBUG)

        if not self.logger.handlers:
            fh = RotatingFileHandler(path, maxBytes=1_000_000, backupCount=2)
            fh.setLevel(logging.DEBUG)
            fmt = logging.Formatter(
                "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
            )
            fh.setFormatter(fmt)
            self.logger.addHandler(fh)

        self.logger.info("Logging initialized")

    def _update_headers(self):
        self.headers["Authorization"] = f"Bearer {get_auth_key("")}"

    def get_response(self, *, query: str, variables: dict | None = None) -> dict:
        """
        Get response from AniList GraphQL API.
        :param query: query string
        :param variables: variables for the query
        :return: json of the response
        """
        if self.requests_left <= 0:
            self.logger.warning("Rate limit exceeded.")
            sleep(60)
        try:
            response = httpx.post(self.base_url, json={"query": query, "variables": variables or {}}, headers=self.headers, timeout=10.0)
            response.raise_for_status()
        except (HTTPError, Timeout, RequestException) as e:
            self.logger.error(f"Request failed: {e}")
            return {}
        except Exception as e:
            self.logger.error(f"An unexpected error occurred: {e}")
            return {}
        if response.status_code != 200:
            self.logger.error(f"Non-200 response: {response.status_code} - {response.text}")
            return {}
        self.requests_left = int(response.headers.get("X-RateLimit-Remaining", 30))
        return response.json()

    async def async_get_response(self, query: str, variables: dict | None = None) -> dict:
        if self.requests_left <= 0:
            self.logger.warning("Rate limit exceeded.")
            sleep(60)
        async with httpx.AsyncClient() as client:
            try:
                response = await client.post(self.base_url, json={"query": query, "variables": variables or {}}, headers=self.headers, timeout=10.0)
                response.raise_for_status()
            except (HTTPError, Timeout, RequestException) as e:
                self.logger.error(f"Request failed: {e}")
                return {}
            except Exception as e:
                self.logger.error(f"An unexpected error occurred: {e}")
                return {}
            if response.status_code != 200:
                self.logger.error(f"Non-200 response: {response.status_code} - {response.text}")
                return {}
            self.requests_left = int(response.headers.get("X-RateLimit-Remaining", 30))
            return response.json()



