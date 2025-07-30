'''
    Helper functions
'''

import json
import keyring
from tinydb import TinyDB, Query

STATUS_PATH = r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache/anime_data/custom_status_db.json"
PRIORITY_PATH = r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache/anime_data/priority_db.json"
USER_DATA_PATH = r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache\user_data.json"
TYPE_ANIME = "Anime"
TYPE_MANGA = "Manga"
db = TinyDB(STATUS_PATH)
db2 = TinyDB(PRIORITY_PATH)
QUERY = Query()

def get_id():
    with open(USER_DATA_PATH, "r") as file:
        data = json.load(file)
    return data["auth"]["user_id"]

def get_username():
    with open(USER_DATA_PATH, "r") as file:
        data = json.load(file)
    return data["auth"]["username"]

def get_service_name():
    with open(USER_DATA_PATH, "r") as file:
        data = json.load(file)
    return data["auth_key"]["service_name"]

def get_user_token_key():
    with open(USER_DATA_PATH, "r") as file:
        data = json.load(file)
    return data["auth_key"]["user_token_key"]

def get_auth_key():
    token = keyring.get_password(get_service_name(), get_user_token_key())
    return token

def db_get_all_anime_id():
    return db.search(QUERY.type == TYPE_ANIME)

def db_get_anime_by_id(id):
    found = db.search((QUERY.id == id) & (QUERY.type == TYPE_ANIME))
    return found[0] if found else None


def db_add_anime(id, title, type):
    query = {'id': id, 'title': title, 'type': type}
    db.insert(query)

def db_remove_anime(id):
    db.remove((QUERY.id == id) & (QUERY.type == TYPE_ANIME))

def db_delete_all():
    db.truncate()

def db_get_all_manga_id():
    return db.search(QUERY.type == TYPE_MANGA)

def db_get_manga_by_id(id):
    found = db.search((QUERY.id == id) & (QUERY.type == TYPE_MANGA))
    return found[0] if found else None

def db_add_manga(id, title, type):
    query = {'id': id, 'title': title, 'type': type}
    db.insert(query)

def db_remove_manga(id):
    db.remove((QUERY.id == id) & (QUERY.type == TYPE_MANGA))

def anime_set_priority(id: int, title: str, status: str, priority: str):
    if status.lower() != "planning":
        return
    else:
        query = {'id': id, 'title': title, 'priority': priority, 'type': TYPE_ANIME}
        db2.insert(query)

def anime_update_priority(id: int, priority: str):
    db2.update({'priority': priority}, QUERY.id == id)

def anime_remove_priority(id: int):
    db2.remove(QUERY.id == id)
