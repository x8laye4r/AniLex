'''
    Here will be all API calls for sending data
'''
import httpx 
import asyncio 
from utils.helpers import get_username, get_auth_key, get_id

username = get_username()
user_id = get_id()
URL = "https://api.example.com/graphql"

async def create_custom_list(list_id: int):
    query = f'''
        mutation($id: Int) {{
            DeleteCustomList(id: $id) {{
                deleted
            }}
        }}
    '''
    variables = {
        "id": list_id
    }
    headers = {
        "Authorization": f"Bearer {get_auth_key()}",
        "Content-Type": "application/json"
    }
    # further implementation of httpx

def create_custom_list(list_name: str):
    query = f'''
        mutation($mediaId: Int, $customLists: [String]) {
            SaveMediaListEntry(mediaId: $mediaId, customLists: $customLists){
                id
                customLists
            }
        }
    '''
    variables = {
        "mediaId": 1,
        "customLists": [list_name]
    }
    headers = {
        "Authorization": f"Bearer {get_auth_key()}",
        "Content-Type": "application/json"
    }
    async with httpx.AsyncClient() as client:
        response = await client.post(URL, json={"query": query, "variables": variables}, headers=headers)
        data = response.json()
    query = f'''
        mutation ($id: Int) {{
            DeleteMediaListEntry(id: $id) {{
                deleted
            }}
        }}
    '''

    variables = {
        "id": 1
    }

    async with httpx.AsyncClient() as client:
        response = await client.post(URL, json={"query": query, "variables": variables}, headers=headers)
        data2 = response.json()

    # See Output to return right data
    print(data, data2)