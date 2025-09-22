import os
import typing

from gql import gql, Client
from gql.transport.requests import RequestsHTTPTransport

from utils.anilex_helper import get_auth_key
from utils.anilex_helper import get_mutation_path

transport = RequestsHTTPTransport(
    url="https://graphql.anilist.co",
    verify=True,
    retries=2,
    headers={
        "Authorization": f"Bearer {get_auth_key()}",
        "Content-Type": "application/json"
    }
)


class TraceMoe:
    def __init__(self, error: typing.Union[None, False] = False):
        self.__error: typing.Union[False, str] = error
        self.__title: typing.Union[None, str] = None
        self.__id: typing.Union[None, str] = None
        self.__anilist_image: typing.Union[None, str] = None
        self.__episode: typing.Union[None, str] = None
        self.__duration: typing.Union[None, int] = None
        self.__from: typing.Union[None, str] = None
        self.__at: typing.Union[None, str] = None
        self.__to: typing.Union[None, str] = None
        self.__similarity: typing.Union[None, float] = None
        self.__video: typing.Union[None, str] = None
        self.__image: typing.Union[None, str] = None
        self.__data_fetched: bool = False

    def __str__(self):
        if self.error:
            return (
                f"TraceMoe(\n"
                f"  error={self.error},\n"
                f")"
            )
        else:
            return (
                f"TraceMoe(\n"
                f"  error={self.error},\n"
                f"  title={self.title},\n"
                f"  id={self.id},\n"
                f"  anilist_image={self.anilist_image},\n"
                f"  duration={self.duration},\n"
                f"  episode={self.episode},\n"
                f"  from={self.from_},\n"
                f"  at={self.at},\n"
                f"  to={self.to},\n"
                f"  similarity={self.similarity},\n"
                f"  video={self.video},\n"
                f"  image={self.image}\n"
                f")"
            )

    '''
        AniList Functions
    '''

    @staticmethod
    def verify_result(result, custom_list=None):
        if "errors" in result:
            return False
        entry = result.get("SaveMediaListEntry", {})
        customLists = entry.get("customLists", {})

        if custom_list is not None:
            return customLists.get(custom_list, False)

        return True

    def add_to_list(self, status: str = "PLANNING"):
        query: typing.Union[str, None] = None
        with open(os.path.join(get_mutation_path(), "add_to_list.graphql"), "r") as file:
            query = file.read()
        variables = {
            "mediaId": self.id,
            "status": status
        }
        client = Client(transport=transport, fetch_schema_from_transport=False)
        with client as session:
            result = session.execute(gql(query), variable_values=variables)
        return self.verify_result(result)

    def add_to_custom_list(self, list_to_add: str, status: str = "PLANNING"):
        with open(os.path.join(get_mutation_path(), "add_to_custom_list.graphql"), "r") as file:
            query = file.read()
        variables = {
            "mediaId": self.id,
            "status": status,
            "customLists": list_to_add
        }
        client = Client(transport=transport, fetch_schema_from_transport=False)
        with client as seasion:
            result = seasion.execute(gql(query), variable_values=variables)
        return self.verify_result(result)

    '''
        Define all setter and getter
    '''

    # data_fetched flag
    @property
    def data_fetched(self):
        return self.__data_fetched

    @data_fetched.setter
    def data_fetched(self, value):
        self.__data_fetched = value

    # anilist_image
    @property
    def anilist_image(self):
        return self.__anilist_image

    @anilist_image.setter
    def anilist_image(self, value):
        self.__anilist_image = value

    # error
    @property
    def error(self):
        return self.__error

    @error.setter
    def error(self, value):
        self.__error = value

    # title
    @property
    def title(self):
        return self.__title

    @title.setter
    def title(self, value):
        self.__title = value

    # id
    @property
    def id(self):
        return self.__id

    @id.setter
    def id(self, value):
        self.__id = value

    # episode
    @property
    def episode(self):
        return self.__episode

    @episode.setter
    def episode(self, value):
        self.__episode = value

    # duration
    @property
    def duration(self):
        return self.__duration

    @duration.setter
    def duration(self, value):
        self.__duration = value

    # from
    @property
    def from_(self):
        return self.__from

    @from_.setter
    def from_(self, value):
        self.__from = value

    # at
    @property
    def at(self):
        return self.__at

    @at.setter
    def at(self, value):
        self.__at = value

    # to
    @property
    def to(self):
        return self.__to

    @to.setter
    def to(self, value):
        self.__to = value

    # similarity
    @property
    def similarity(self):
        return self.__similarity

    @similarity.setter
    def similarity(self, value):
        self.__similarity = value

    # video
    @property
    def video(self):
        return self.__video

    @video.setter
    def video(self, value):
        self.__video = value

    # image
    @property
    def image(self):
        return self.__image

    @image.setter
    def image(self, value):
        self.__image = value