'''
    Settings Handler
'''

import yaml
import os
from utils.helpers import load_settings, get_cache_path

PATH = os.path.join(get_cache_path(), "settings.yaml")

def save_card_settings(data_anime, data_manga):
    configure = load_settings()
    if configure is None:
        configure = {}

    configure.setdefault("CARD_OPTIONS", {})
    configure["CARD_OPTIONS"].setdefault("ANIME", {})
    configure["CARD_OPTIONS"].setdefault("MANGA", {})

    for cb in data_anime:
        name = cb.text().lower()
        configure["CARD_OPTIONS"]["ANIME"][name] = cb.isChecked()

    for cb in data_manga:
        name = cb.text().lower()
        configure["CARD_OPTIONS"]["MANGA"][name] = cb.isChecked()

    with open(PATH, "w", encoding="utf-8") as file:
        yaml.safe_dump(configure, file, default_flow_style=False, sort_keys=False, allow_unicode=True)
