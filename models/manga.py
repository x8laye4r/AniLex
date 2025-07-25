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