import json
import keyring
from keyring.errors import PasswordSetError, PasswordDeleteError
import os
from tinydb import TinyDB, Query
import yaml
import requests

def get_cache_path():
    aktuell = os.path.abspath(os.path.dirname(__file__))
    parent = os.path.dirname(aktuell)
    cache_pfad = os.path.join(parent, "cache")
    return cache_pfad

STANDARD_PATH = get_cache_path()

STATUS_PATH = os.path.join(STANDARD_PATH, "anime_data", "custom_status_db.json")
PRIORITY_PATH = os.path.join(STANDARD_PATH, "anime_data", "custom_priority_db.json")
USER_DATA_PATH = os.path.join(STANDARD_PATH, "user_data.json")
SETTINGS_PATH = os.path.join(STANDARD_PATH, "settings.yaml")
TYPE_ANIME = "Anime"
TYPE_MANGA = "Manga"
QUERY = Query()


try:
    db = TinyDB(STATUS_PATH)
    db2 = TinyDB(PRIORITY_PATH)
except TinyDBException as e:
    raise RuntimeError(f"Error to initialize database: {e}")

def _load_user_data():
    if not os.path.exists(USER_DATA_PATH):
        raise FileNotFoundError(f"User data not found {USER_DATA_PATH}")
    try:
        with open(USER_DATA_PATH, "r") as file:
            return json.load(file)
    except json.JSONDecodeError:
        raise ValueError("User data are corrupt or in a not valid JSON format.")


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
    except TinyDBException as e:
        raise RuntimeError(f"Error reading all anime: {e}")

def db_get_all_manga():
    try:
        return db.search(QUERY.type == TYPE_MANGA)
    except TinyDBException as e:
        raise RuntimeError(f"Error reading all manga: {e}")

def db_get_anime_by_id(id: int):
    try:
        found = db.search((QUERY.id == id) & (QUERY.type == TYPE_ANIME))
        return found[0] if found else None
    except TinyDBException as e:
        raise RuntimeError(f"Error reading anime with id {id}: {e}")

def db_get_manga_by_id(id: int):
    try:
        found = db.search((QUERY.id == id) & (QUERY.type == TYPE_MANGA))
        return found[0] if found else None
    except TinyDBException as e:
        raise RuntimeError(f"Error reading manga with id {id}: {e}")

def db_add(id: int, title: str, type: str):
    try:
        if db.search((QUERY.id == id) & (QUERY.type == type)):
            raise ValueError(f"Entry with ID {id} and type '{type}' exists already.")
        db.insert({'id': id, 'title': title, 'type': type})
    except TinyDBException as e:
        raise RuntimeError(f"Error while appending data: {e}")

def db_remove_anime(id: int):
    try:
        db.remove((QUERY.id == id) & (QUERY.type == TYPE_ANIME))
    except TinyDBException as e:
        raise RuntimeError(f"Error while deleting anime with id {id}: {e}")

def db_remove_manga(id: int):
    try:
        db.remove((QUERY.id == id) & (QUERY.type == TYPE_MANGA))
    except TinyDBException as e:
        raise RuntimeError(f"Error while deleting manga with id {id}: {e}")

def db_delete_all():
    try:
        db.truncate()
    except TinyDBException as e:
        raise RuntimeError(f"Error while deleting databases data: {e}")

def set_priority(id: int, title: str, status: str, priority: str, type: str):
    if status.lower() != "planning":
        return
    try:
        db2.insert({'id': id, 'title': title, 'priority': priority, 'type': type})
    except TinyDBException as e:
        raise RuntimeError(f"Error while setting priority: {e}")

def update_priority(id: int, priority: str):
    try:
        if not db2.update({'priority': priority}, QUERY.id == id):
            raise ValueError(f"No entry with id {id} found to update.")
    except TinyDBException as e:
        raise RuntimeError(f"Error while updating priority: {e}")

def remove_priority(id: int):
    try:
        db2.remove(QUERY.id == id)
    except TinyDBException as e:
        raise RuntimeError(f"Error while removing priority with id {id}: {e}")

def get_priority(id: int):
    try:
        result = db2.search(QUERY.id == id)
        return result[0] if result else None
    except TinyDBException as e:
        raise RuntimeError(f"Error while reading the id {id}: {e}")

def clear_all_priorities():
    try:
        db2.truncate()
    except TinyDBException as e:
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