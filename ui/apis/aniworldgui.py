from PySide6.QtWidgets import QWidget, QScrollArea, QVBoxLayout, QLabel, QApplication, QHBoxLayout
from PySide6.QtCore import Qt, Signal, QThread
import sys
from core.tool_apis.aniworldapi import AniWorldAPI
from ui.custom.aniworld.aniworld_card import AniWorldCard
import logging

def create_many_cards(data_list):
    widgets = []
    for title, data in data_list.items():
        card = create_card_widget(title, data)
        widgets.append(card)
    return widgets

def create_card_widget(title, data):
    card = AniWorldCard(
        anime_id=data.get("id") if data.get("id") is not None else -1,
        title=title if title is not None else "No Title",
        image_data=data.get("poster") if data.get("poster") is not None else None,
        season=data.get("season") if data.get("season") is not None else None,
        episode=data.get("episode") if data.get("episode") is not None else None,
    )
    return card

def create_table_items(data_list):
    widgets = []



class HomeDataFetcher(QThread):
    signal = Signal(object)
    def __init__(self, api: AniWorldAPI):
        super().__init__()
        self.api = api
    def run(self):
        data = self.api.get_home()
        self.signal.emit(data)

class AniWorldGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.api = AniWorldAPI()
        self.scroll_views = []

        self.init_ui()

        # Logging setup
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s- %(levelname)s - %(message)s'
        )
        self.logger = logging.getLogger(__name__)

    def init_ui(self):
        main_layout = QVBoxLayout(self)

        main_title = QLabel("AniWorld")
        main_title.setStyleSheet("font-size: 46px; font-weight: bold;")
        main_title.setAlignment(Qt.AlignCenter)
        main_layout.addWidget(main_title)


        # --- Outer scroll area ---
        main_scroll_area = QScrollArea()
        main_scroll_area.setWidgetResizable(True)
        main_scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        main_scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

        main_container = QWidget()
        main_container_layout = QVBoxLayout(main_container)

        # --- Example horizontal scroll section ---
        def create_scroll_section(title, height=312, scroll_type="horizontal"):
            label = QLabel(title)
            label.setStyleSheet("font-size: 18px; font-weight: bold;")
            scroll = QScrollArea()
            scroll.setWidgetResizable(True)
            scroll.setFixedHeight(height)
            container = None
            if scroll_type == "horizontal":
                scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
                scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)

                container = QWidget()
                container_layout = QHBoxLayout(container)
            elif scroll_type == "vertical":
                scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
                scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)

                container = QWidget()
                container_layout = QVBoxLayout(container)

            scroll.setStyleSheet("background: transparent; border: none;")
            container.setContentsMargins(5, 5, 5, 5)
            scroll.setWidget(container)
            return label, scroll

        # Abschnitte hinzufügen
        sections = [
            ("Animes", 340, "horizontal"),
            ("Popular Animes", 340, "horizontal"),
            ("New Episodes", 600, "vertical"),
            ("New Animes", 340, "horizontal"),
            ("Animes Calendar", 600, "vertical"),
            ("Currently Popular Animes", 340, "horizontal"),
            ("Users' Favorite Animes", 340, "horizontal")
        ]

        for title, h, scroll in sections:
            lbl, scr = create_scroll_section(title, h, scroll)
            main_container_layout.addWidget(lbl)
            main_container_layout.addWidget(scr)
            self.scroll_views.append(scr)

        main_scroll_area.setWidget(main_container)
        main_layout.addWidget(main_scroll_area)
        self.setLayout(main_layout)

        self.add_home()

    def add_home(self):
        self.fetcher = HomeDataFetcher(self.api)
        self.fetcher.signal.connect(self.process_home_data)
        self.fetcher.start()

    def add_widgets_to_section(self, section_index, widgets):
        scroll_area = self.scroll_views[section_index]  # Zugriff über Index
        container = scroll_area.widget()
        layout = container.layout()
        for widget in widgets:
            layout.addWidget(widget)

    def process_home_data(self, data):
        if data is None:
            self.logger.error("No data was fetched from the API.")
            return

        print(data.keys())

        keys = ['slider', 'popular', 'new_episodes', 'new_animes', 'today_anime_calendar', 'currently_popular', 'users_favorite']
        card_keys = ['slider', 'popular', 'new_animes', 'currently_popular', 'users_favorite']
        table_keys = ['new_episodes', 'today_anime_calendar']

        for key, dat in data.items():
            if key in card_keys:
                slider_widgets = create_many_cards(dat)
                self.add_widgets_to_section(keys.index(key), slider_widgets)
            elif key in table_keys:
                pass


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = AniWorldGUI()
    window.showMaximized()
    sys.exit(app.exec())