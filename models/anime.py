'''
    Class for an Anime object for easier accessing everything
'''

class AnimeWhole:
    def __init__(self, data):
        self.id = data["id"]
        self.title_romaji = data["title"]["romaji"]
        self.title_english = data["title"]["english"]
        self.title_native = data["title"]["native"]
        self.episodes = data.get("episodes")
        self.season = data.get("season")
        self.season_year = data.get("seasonYear")
        self.airing_in = data["nextAiringEpisode"]["airingAt"]
        self.status = data["status"]
        self.score = data.get("averageScore")
        self.genres = data["genres"]
        self.description = data["description"]
        self.cover_url = data["coverImage"]["large"]
        self.banner_url = data.get("bannerImage")

    def __str__(self):
        return f"Title: {self.title_english or self.title_romaji}\nId: {self.id}"

    def releasing(self):
        return str(self.status).lower() == "releasing"

    def finished(self):
        return str(self.status).lower() == "finished"

class AnimeShort:
    def __init__(self, data):
        self.id = data["id"]
        self.title_romaji = data["title"]["romaji"]
        self.title_english = data["title"]["english"]
        self.title_native = data["title"]["native"]
        self.cover_url = data["coverImage"]["large"]