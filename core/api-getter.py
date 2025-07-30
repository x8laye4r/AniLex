'''
    Here will be all API getter Calls for AniLib
'''
import requests
from utils.helpers import get_username, get_id, get_auth_key

url = "https://graphql.anilist.co"
username = get_username()
user_id = get_id()

def get_all_airing_anime():
    query = '''
    query ($page: Int, $perPage: Int) {
      Page(page: $page, perPage: $perPage) {
        media(type: ANIME, status: RELEASING, sort: POPULARITY_DESC) {
          id
          title {
            romaji
            english
            native
          }
          nextAiringEpisode {
            episode
            airingAt
          }
          coverImage {
            large
          }
        }
      }
    }
    '''

    variables = {
        "page": 1,
        "perPage": 50
    }

    response = requests.post(url, json={"query": query, "variables": variables})
    data = response.json()

    for anime in data["data"]["Page"]["media"]:
        print(anime["title"]["romaji"])
