'''
    Helper functions
'''

import json
import keyring

PATH = r"C:\Users\eisel\PycharmProjects\UNOFICIAL-ANILEX\cache\user_data.json"

def get_id():
    with open(PATH, "r") as file:
        data = json.load(file)
    return data["auth"]["user_id"]

def get_username():
    with open(PATH, "r") as file:
        data = json.load(file)
    return data["auth"]["username"]

def get_service_name():
    with open(PATH, "r") as file:
        data = json.load(file)
    return data["auth_key"]["service_name"]

def get_user_token_key():
    with open(PATH, "r") as file:
        data = json.load(file)
    return data["auth_key"]["user_token_key"]

def get_auth_key():
    token = keyring.get_password(get_service_name(), get_user_token_key())
    return token

