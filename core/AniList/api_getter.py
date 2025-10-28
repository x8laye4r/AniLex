from models.AniListAPI import AniListAPI
from models.status import MediaListStatus
from models.type import MediaType
from utils.anilex_helpers_v2 import load_graphql_query

api = AniListAPI()

def get_auth_user_info():
    query = load_graphql_query("get_auth_profile")
    resp = api.get_response(query=query)
    return resp.get("Viewer", {})

def get_public_user_info(username: str):
    query = load_graphql_query("get_public_profile")
    variables = {"username": username}
    resp = api.get_response(query=query, variables=variables)
    return resp.get("User", {})

def search_user(username: str):
    query = load_graphql_query("search_user")
    variables = {"username": username}
    resp = api.get_response(query=query, variables=variables)
    return resp

from typing import Iterable

def get_user_list(
    user_id: int | None = None,
    username: str | None = None,
    media_type: "MediaType" = MediaType.ANIME,
    status: Iterable["MediaListStatus"] | None = None,
    status_not: Iterable["MediaListStatus"] | None = None,
    sort: Iterable["MediaListSort"] | None = None,
    force_single: bool = False,
    notes_like: str | None = None,
    started_at: str | None = None,
    ended_at: str | None = None,
    date_type: str = "started",
) -> dict:
    status = list(status) if status is not None else [MediaListStatus.COMPLETED]
    sort = list(sort) if sort is not None else None
    status_not = list(status_not) if status_not is not None else None

    query = load_graphql_query("media_list_collection")

    variables = {
        "userId": user_id,
        "userName": username,
        "type": media_type,
        "status": status,
        "statusNot": status_not,
        "sort": sort,
        "notesLike": notes_like,
        "startedAt": started_at,
        "endedAt": ended_at,
        "dateType": date_type,
        "forceSingleCompletedList": force_single,
    }
    resp = api.get_response(query=query, variables=variables)

    mlc = resp.get("data", {})
    if force_single:
        lists = mlc.get("lists", [])
        return lists[0] if lists else {}
    return mlc

def