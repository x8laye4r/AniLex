'''
    Here will be all API getter Calls for AniLib
'''
import requests
from utils.helpers import get_username, get_id, get_auth_key, get_mal_id

URL = "https://graphql.anilist.co"
username = get_username()
user_id = get_id()

# First try in getting some data
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

    response = requests.post(URL, json={"query": query, "variables": variables})
    data = response.json()

    for anime in data["data"]["Page"]["media"]:
        print(anime["title"]["romaji"])



def get_full_card_anime(anime_id: int):
    query = f'''
    query {{
        Media(id: {anime_id}, type: ANIME) {{
            # ==== BASIC INFO ====
            id
            title {{ english romaji native }}
            description
            episodes
            duration
            status
            genres
            averageScore
            meanScore
            season
            hashtag
            startDate {{ day month year }}
            endDate {{ day month year }}

            # ==== IMAGES ====
            coverImage {{ large }}
            bannerImage

            # ==== AIRING ====
            nextAiringEpisode {{ episode airingAt timeUntilAiring }}

            # ==== ADDITIONAL INFO ====
            format
            popularity
            favourites
            source

            # ==== STAFF ====
            staff {{
                edges {{
                    role
                    node {{
                        id
                        name {{ full }}
                        image {{ medium }}
                    }}
                }}
            }}

            # ==== CHARACTERS ====
            characters {{
                edges {{
                    role
                    node {{
                        id
                        name {{ full }}
                        image {{ medium }}
                    }}
                }}
            }}

            # ==== RELATIONS ====
            relations {{
                edges {{
                    relationType
                    node {{
                        id
                        title {{ romaji english native }}
                        coverImage {{ medium }}
                    }}
                }}
            }}

            # ==== RANKINGS & STATS ====
            rankings {{ context rank year allTime }}
            stats {{
                statusDistribution {{ status amount }}
                scoreDistribution {{ score amount }}
            }}

            # ==== STUDIOS, TAGS, TRAILER ====
            studios {{
                edges {{
                    isMain
                    node {{ id name }}
                }}
            }}
            tags {{ category name }}
            trailer {{ site id thumbnail }}
        }}
    }}
    '''
    response = requests.post(URL, json={'query': query})
    data = response.json()['data']['Media']

    # Build Card structure
    card = {
        "basic": {
            "id": data['id'],
            "title": data['title'],
            "description": data['description'],
            "episodes": data['episodes'],
            "duration": data['duration'],
            "status": data['status'],
            "genres": data['genres'],
            "scores": {
                "average": data['averageScore'],
                "mean": data['meanScore']
            },
            "season": data['season'],
            "startDate": data['startDate'],
            "endDate": data['endDate'],
            "format": data['format'],
            "popularity": data['popularity'],
            "favourites": data['favourites'],
            "source": data['source'],
            "hashtag": data['hashtag']
        },
        "images": {
            "cover": data['coverImage']['large'],
            "banner": data['bannerImage']
        },
        "airing": data.get('nextAiringEpisode', None),
        "staff": [
            {
                "role": edge['role'],
                "id": edge['node']['id'],
                "name": edge['node']['name']['full'],
                "image": edge['node'].get('image', {}).get('medium')
            } for edge in data['staff']['edges']
        ],
        "characters": [
            {
                "role": edge['role'],
                "id": edge['node']['id'],
                "name": edge['node']['name']['full'],
                "image": edge['node'].get('image', {}).get('medium')
            } for edge in data['characters']['edges']
        ],
        "relations": [
            {
                "relationType": edge['relationType'],
                "id": edge['node']['id'],
                "title": edge['node']['title'],
                "coverImage": edge['node']['coverImage']['medium']
            } for edge in data['relations']['edges']
        ],
        "rankings": data.get('rankings', []),
        "stats": data.get('stats', {}),
        "studios": [
            {
                "isMain": edge['isMain'],
                "id": edge['node']['id'],
                "name": edge['node']['name']
            } for edge in data['studios']['edges']
        ],
        "tags": data.get('tags', []),
        "trailer": data.get('trailer', None)
    }

    return card

def get_user_anime_list_with_own_info(info: list):
    USERNAME = get_username()
    OAUTH_KEY = get_auth_key()