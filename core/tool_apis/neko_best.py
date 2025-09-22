import requests


def get_tags():
    resp = requests.get("https://nekos.best/api/v2/endpoints")
    data = resp.json()
    tags_img = [tag for tag, info in data.items() if info.get("format") == "png" or info.get("format") == "jpg"]
    tags_gif = [tag for tag, info in data.items() if info.get("format") == "gif"]
    return tags_img, tags_gif


class NekoBestAPI:
    def __init__(self, tag: str = "neko"):
        self.tags_img, self.tags_gif = get_tags()
        self.tag = tag
        self.base_url = "https://nekos.best/api/v2/"


    def fetch_image(self):
        res = requests.get(f"{self.base_url}{self.tag}")
        return res.json()

    def fetch_many_images(self, amount: int = 1):
        res = requests.get(f"{self.base_url}{self.tag}", params={"amount": amount})
        return res.json()

    def get_data(self):
        return self.data

    def get_url(self):
        return self.url


if __name__ == "__main__":
    api = NekoBestAPI()
    data = api.fetch_many_images(5)
    data = data.get("results", [])
    for item in data:
        print(item.get("url"))

