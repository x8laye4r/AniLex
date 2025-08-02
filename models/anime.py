'''
    Class for an Anime object for easier accessing everything
'''


class AnimeWhole:
    def __init__(self, data):
        self.id = data.get("id")
        titles = data.get("title", {})
        self.title_romaji = titles.get("romaji")
        self.title_english = titles.get("english")
        self.title_native = titles.get("native")
        self.episodes = data.get("episodes")
        self.season = data.get("season")
        self.season_year = data.get("seasonYear")

        next_ep = data.get("nextAiringEpisode")
        self.airing_in = next_ep.get("airingAt") if next_ep else None

        self.status = data.get("status")
        self.score = data.get("averageScore")
        self.genres = data.get("genres", [])
        self.description = data.get("description")

        cover = data.get("coverImage", {})
        self.cover_url = cover.get("large")
        self.banner_url = data.get("bannerImage")

        self.streaming_episodes = data.get("streamingEpisodes", [])
        self.external_links = data.get("externalLinks", [])
        self.trailer = data.get("trailer")
        self.tags = data.get("tags", [])

        self.studios = [edge["node"]["name"] for edge in data.get("studios", {}).get("edges", [])]

        self.relations = [{
            "id": edge["node"]["id"],
            "title_romaji": edge["node"]["title"].get("romaji"),
            "title_english": edge["node"]["title"].get("english"),
            "status": edge["node"]["status"],
            "relationType": edge.get("relationType")
        } for edge in data.get("relations", {}).get("edges", [])]

        self.characters = [{
            "id": edge["node"]["id"],
            "name": edge["node"]["name"]["full"],
            "role": edge.get("role")
        } for edge in data.get("characters", {}).get("edges", [])]

        self.staff = [{
            "id": edge["node"]["id"],
            "name": edge["node"]["name"]["full"],
            "role": edge.get("role")
        } for edge in data.get("staff", {}).get("edges", [])]

        self.recommendations = [{
            "id": edge["node"]["mediaRecommendation"]["id"],
            "title_romaji": edge["node"]["mediaRecommendation"]["title"].get("romaji"),
            "rating": edge["node"].get("rating")
        } for edge in data.get("recommendations", {}).get("edges", [])]

        self.rankings = data.get("rankings", [])
        self.threads = data.get("threads", [])
        self.statistics = data.get("statistics", {})

    def __str__(self):
        return f"Title: {self.title_english or self.title_romaji}\nId: {self.id}"

    def releasing(self):
        return str(self.status).lower() == "releasing" if self.status else False

    def finished(self):
        return str(self.status).lower() == "finished" if self.status else False


class AnimeShort:
    def __init__(self, data):
        self.id = data.get["id"]
        self.title_romaji = data["title"]["romaji"]
        self.title_english = data["title"]["english"]
        self.title_native = data["title"]["native"]
        self.cover_url = data["coverImage"]["large"]
        self.status = data.get("status")