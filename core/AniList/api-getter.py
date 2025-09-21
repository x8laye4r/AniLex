'''
    Here will be all API getter Calls for AniLib
'''
import httpx
import asyncio
from utils.anilex_helper import get_username, get_id, get_auth_key, get_mal_id, convert_time, filter_today

URL = "https://graphql.anilist.co"
username = get_username()
user_id = get_id()

async def fetch(query: str, variables: dict = None):
    async with httpx.AsyncClient() as client:
        if variables is not None:
            response = await client.post(URL, json={'query': query, 'variables': variables})
        else:
            response = await client.post(URL, params={'query': query})
        return response.json()


async def get_full_card_anime(anime_id: int):
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
    task = asyncio.create_task(fetch(query))
    await asyncio.sleep(2)
    task.cancel()

    try:
        data = await task
    except asyncio.CancelledError:
        return None

    data = data['data']['Media']

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
            "startDate": f"{data['startDate']['day']}.{data['startDate']['month']}.{data['startDate']['year']}" if data['startDate']['day'] else None,
            "endDate": f"{data['endDate']['day']}.{data['endDate']['month']}.{data['endDate']['year']}" if data['endDate']['day'] else None,
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

async def get_all_airing_today(page: int = 1):
    query = '''
    query($page: Int, $perPage: Int) {
        Page(page: $page, perPage: $perPage) {
            media(type: ANIME, status: RELEASING, sort: POPULARITY_DESC) {
                id title {
                    romaji
                    english
                    native
                }
                nextAiringEpisode {
                    episode
                    timeUntilAiring
                    airingAt
                }
                coverImage {
                    medium
                }
            }
        }
    }
    '''
    variables = {
        "page": page,
        "perPage": 50,
    }

    task = asyncio.create_task(fetch(query, variables))

    data = await task
    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        data = filter_today(data)
        return data

async def get_current_season_anime(page: int = 1):
    query = '''
    query ($page: Int, $perPage: Int, $season: MediaSeason, $seasonYear: Int) {
      Page(page: $page, perPage: $perPage) {
        media(type: ANIME, season: $season, seasonYear: $seasonYear, sort: POPULARITY_DESC) {
          id
          title {
            romaji
            english
            native
          }
          nextAiringEpisode {
            episode
            airingAt
            timeUntilAiring
          }
          coverImage {
            medium
          }
        }
      }
    }
    '''
    from datetime import datetime
    now = datetime.now()
    month = now.month
    year = now.year

    if month in [12, 1, 2]:
        season = "WINTER"
        if month == 12:
            year += 1
    elif month in [3, 4, 5]:
        season = "SPRING"
    elif month in [6, 7, 8]:
        season = "SUMMER"
    else:
        season = "FALL"

    variables = {
        "page": page,
        "perPage": 50,
        "season": season,
        "seasonYear": year
    }

    task = asyncio.create_task(fetch(query, variables))

    data = await task
    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        return data

async def get_next_season_anime(page: int = 1):
    query = '''
    query ($page: Int, $perPage: Int, $season: MediaSeason, $seasonYear: Int) {
      Page(page: $page, perPage: $perPage) {
        media(type: ANIME, season: $season, seasonYear: $seasonYear, sort: POPULARITY_DESC) {
          id
          title {
            romaji
            english
            native
          }
          nextAiringEpisode {
            episode
            airingAt
            timeUntilAiring
          }
          coverImage {
            medium
          }
        }
      }
    }
    '''
    from datetime import datetime
    now = datetime.now()
    month = now.month
    year = now.year

    if month in [12, 1, 2]:
        season = "SPRING"
        if month == 12:
            year += 1
    elif month in [3, 4, 5]:
        season = "SUMMER"
    elif month in [6, 7, 8]:
        season = "FALL"
    else:
        season = "WINTER"
        year += 1

    variables = {
        "page": page,
        "perPage": 50,
        "season": season,
        "seasonYear": year
    }

    task = asyncio.create_task(fetch(query, variables))

    data = await task
    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        return data

async def get_trending_anime(page: int = 1):
    query = '''
    query ($page: Int, $perPage: Int) {
      Page(page: $page, perPage: $perPage) {
        media(type: ANIME, sort: TRENDING_DESC) {
          id
          title {
            romaji
            english
            native
          }
          nextAiringEpisode {
            episode
            airingAt
            timeUntilAiring
          }
          coverImage {
            medium
          }
        }
      }
    }
    '''
    variables = {
        "page": page,
        "perPage": 50
    }

    task = asyncio.create_task(fetch(query, variables))

    data = await task
    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        return data

async def get_trending_manga(page: int = 1):
    query = '''
    query ($page: Int, $perPage: Int) {
      Page(page: $page, perPage: $perPage) {
        media(type: MANGA, sort: TRENDING_DESC) {
          id
          title {
            romaji
            english
            native
          }
          coverImage {
            medium
          }
        }
      }
    }
    '''
    variables = {
        "page": page,
        "perPage": 50
    }

    task = asyncio.create_task(fetch(query, variables))

    data = await task
    if data.get('errors'):
        return None
    else:
        return data

async def get_user_anime_list(sort: str, page: int = 1, status: str = "CURRENT"):
    auth_key = get_auth_key()
    if auth_key is None:
        return None

    query = f"""
    query ($userId: Int, $page: Int, $perPage: Int, $status: MediaListStatus) {{
      Page(page: $page, perPage: $perPage) {{
        mediaList(userId: $userId, type: ANIME, status: $status: {sort}) {{
          id
          status
          score
          progress
          media {{
            id
            title {{
              romaji
              english
              native
            }}
            coverImage {{
              medium
            }}
            nextAiringEpisode {{
              episode
              airingAt
              timeUntilAiring
            }}
          }}
        }}
      }}
    }}
    """
    variables = {
        "userId": user_id,
        "page": page,
        "perPage": 50,
        "status": status
    }

    headers = {
        "Authorization": f"Bearer {auth_key}"
    }

    async with httpx.AsyncClient() as client:
        response = await client.post(URL, json={'query': query, 'variables': variables}, headers=headers)
        data = response.json()
    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        return data

async def get_user_manga_list(sort: str, page: int = 1, status: str = "CURRENT"):
    auth_key = get_auth_key()
    if auth_key is None:
        return None

    query = f"""
    query ($userId: Int, $page: Int, $perPage: Int, $status: MediaListStatus, $sort: [MediaListSort]) {{
      Page(page: $page, perPage: $perPage) {{
        mediaList(userId: $userId, type: MANGA, status: $status, sort: $sort) {{
          id
          status
          score
          progress
          media {{
            id
            title {{
              romaji
              english
              native
            }}
            coverImage {{
              medium
            }}
            chapters
            volumes
          }}
        }}
      }}
    }}
    """

    variables = {
        "userId": user_id,
        "page": page,
        "perPage": 50,
        "status": status
    }

    headers = {
        "Authorization": f"Bearer {auth_key}"
    }

    async with httpx.AsyncClient() as client:
        response = await client.post(URL, json={'query': query, 'variables': variables}, headers=headers)
        data = response.json()

    if data.get('errors'):
        return None
    else:
        data = convert_time(data)
        return data
