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



def get_full_card_info_anime(id):
    query = f'''
    query {{
        Media(id: {id}, type: ANIME) {{
            id
            title {{
                english
                romaji
                native
            }}
            description
            episodes
            duration
            status
            genres
            averageScore
            meanScore
            coverImage {{
                large
            }}
            bannerImage
            nextAiringEpisode {{
                episode
                airingAt
                timeUntilAiring
            }}
            format
            popularity
            favourites
            startDate {{
                day
                month
                year
            }}
            endDate {{
                day
                month
                year
            }}
            season
            source
            staff {{
                edges {{
                    role
                    node {{
                        id
                        name {{
                            full
                        }}
                        image {{
                            medium
                        }}
                    }}
                }}
            }}
            characters {{
                edges {{
                    role
                    node {{
                        id
                        name {{
                            full
                        }}
                        image {{
                            medium
                        }}
                    }}
                }}
            }}
            relations {{
                edges {{
                    relationType
                    node {{
                        id
                        title {{
                            romaji
                            english
                            native
                        }}
                        coverImage {{
                            medium
                        }}
                    }}
                }}
            }}
            rankings {{
                context
                rank
                year
                allTime
            }}
            stats {{
                statusDistribution {{
                    status
                    amount
                }}
                scoreDistribution {{
                    score
                    amount
                }}
            }}
            studios {{
                edges {{
                    isMain
                    node {{
                        id
                        name
                    }}
                }}
            }}
            tags {{
                category
                name
            }}
            trailer {{
                site
                id
                thumbnail
            }}
        }}
    }}
    '''
    response = requests.post(URL, json={"query": query})
    data = response.json()
    if "errors" in data:
        print("Fehler:", data["errors"])
        return None
    return data

# Beispiel
print(get_full_card_info_anime(179966))