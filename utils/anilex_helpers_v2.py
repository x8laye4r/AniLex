import json
import logging
import os
from logging.handlers import RotatingFileHandler
from pathlib import Path

import keyring

from utils import SETTINGS


def build_structure_dir(path: str) -> None:
    # create main path
    os.makedirs(path, exist_ok=True)

    # build structure
    os.makedirs(os.path.join(path, "anime_data"), exist_ok=True)
    os.makedirs(os.path.join(path, "api_data"), exist_ok=True)
    os.makedirs(os.path.join(path, "custom"), exist_ok=True)
    os.makedirs(os.path.join(path, "logs"), exist_ok=True)

    # create files
    with open(os.path.join(path, "settings.json"), "w") as f:
        json.dump(SETTINGS, f, indent=4)
    with open(os.path.join(path, "user_data.json"), "w") as f:
        f.write("{}") # prevent json errors
    with open(os.path.join(path, "api_data", "aniworld_data.db"), "w"):
        pass


def get_cache_dir() -> str:
    path = os.path.join(os.getenv('LOCALAPPDATA') or Path.home() / "Documents", "Anilex")
    if not os.path.exists(path):
        build_structure_dir(path)

    return path

def get_graphql_dir() -> str:
    current_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(current_dir)
    graphql_path = os.path.join(project_root, "GraphQL")
    return graphql_path

if not os.path.exists(os.path.join(get_cache_dir(), "logs", "helpers.log")):
    with open(os.path.join(get_cache_dir(), "logs", "helpers.log"), "w") as f:
        pass
logging.basicConfig(level=logging.DEBUG,
                    format='%(asctime)s - %(levelname)s - %(message)s',
                    handlers=[RotatingFileHandler(os.path.join(get_cache_dir(), "logs", "helpers.log"), maxBytes=5*1024*1024, backupCount=2)]
)
logger = logging.getLogger("anilex_helpers")


def get_query_path() -> str:
    current_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(current_dir)

    return os.path.join(project_root, "GraphQL", "query")

def get_mutation_path() -> str:
    current_dir = os.path.dirname(os.path.abspath(__file__))
    project_root = os.path.dirname(current_dir)
    return os.path.join(project_root, "GraphQL", "mutations")

def _load_user_data() -> dict:
    if not os.path.exists(os.path.join(get_cache_dir(), "user_data.json")):
        logger.warning("Cache Path does not exist!")
        return {}
    with open(os.path.join(get_cache_dir(), "user_data.json"), "r") as f:
        return json.load(f)

def _load_settings() -> dict:
    if not os.path.exists(os.path.join(get_cache_dir(), "settings.json")):
        logger.warning("Settings Path does not exist!")
        return {}
    with open(os.path.join(get_cache_dir(), "settings.json"), "r") as f:
        return json.load(f)

def load_graphql_query(file_name: str, type: str = "queries") -> str:
    path = os.path.join(get_graphql_dir(), type, file_name + ".graphql")
    with open(path, "r") as f:
        return f.read()

def get_service_name(username: str) -> str:
    data = _load_user_data()
    try:
        return data[username]["auth_key"]["service_name"]
    except KeyError:
        logger.error("Service Name not found!")
        return ""

def get_user_totken_key(username: str) -> str:
    data = _load_user_data()

    try:
        return data[username]["auth_key"]["user_token_key"]
    except KeyError:
        logger.error("User Token Key not found!")
        return ""

def get_auth_key(username: str) -> str:
    service_name = get_service_name(username)
    user_token_key = get_user_totken_key(username)
    if service_name and user_token_key:
        try:
            return keyring.get_password(service_name, user_token_key)
        except Exception as e:
            logger.error(f"Error retrieving auth key: {e}")
            return ""
    else:
        logger.error("Service Name or User Token Key is empty!")
        return ""