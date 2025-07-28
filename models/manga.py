'''
    Class for a manga object for easier accessing everything
'''

class Manga:
    def __init__(self, data):
        self.id = data["id"]
        self.title_romaji = data["title"]["romaji"]
        self.title_english = data["title"]["english"]
        self.title_native = data["title"]["native"]
        self.chapters = data["chapters"]
        self.volumes = data["volumes"]
        self.status = data["status"]
        self.score = data["averageScore"]
        self.genres = data["genres"]
        self.description = data["description"]
        self.cover_url = data["coverImage"]["large"]

    def __str__(self):
        return f"Title: {self.title_english}\nId: {self.id}"

    def ongoing(self):
        return str(self.status).lower() == "releasing"

    def finished(self):
        return str(self.status).lower() == "finished"
