'''
    Here will be all API calls for sending data
'''
import httpx 
import asyncio 
from utils.anilex_helper import get_username, get_auth_key, get_id

username = get_username()
user_id = get_id()
URL = "https://graphql.anilist.co"

async def delete_custom_list(list_id: int):
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


