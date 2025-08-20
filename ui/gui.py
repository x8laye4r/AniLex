'''

'''
import sys
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QStackedWidget, QPushButton, QSizePolicy, QScrollArea, QDialog, QCheckBox, QSpacerItem
)
from PySide6.QtCore import Qt, QPropertyAnimation, QEasingCurve, Property, QSize, QPoint
from PySide6.QtGui import QEnterEvent, QPixmap, QIcon
from utils.helpers import get_cache_path, load_settings
from ui.card import Card
from ui.tab import CustomTabWidget, TabButton
from os import path
from config.settings import save_card_settings

'''
    To Do:
        1) Make QScrollArea arrangement from top left to bottom right
        2) Make scroll bar better
        3) Make a drop down for the filter button
        4) Make tabs color in a better colors
'''

class Discover(QWidget):
    def __init__(self):
        super().__init__()

        main_layout = QVBoxLayout(self)
        scroll_area = QScrollArea()
        scroll_area.setWidgetResizable(True)
        scroll_area.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        main_layout.addWidget(scroll_area)

        container = QWidget()
        container_layout = QVBoxLayout(container)

        # Dictionary für einfachen Zugriff
        self.sections = {}

        def add_section(title):
            section_layout = QVBoxLayout()
            label = QLabel(title)
            label.setStyleSheet("font-size: 20px; font-weight: bold;")
            inner_scroll = QScrollArea()
            inner_scroll.setWidgetResizable(True)
            inner_scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
            inner_scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
            inner_scroll.setFixedHeight(150)
            inner_scroll.setStyleSheet("background: transparent; border: 0;")

            content = QWidget()

            inner_scroll.setWidget(content)

            section_layout.addWidget(label)
            section_layout.addWidget(inner_scroll)

            container_layout.addLayout(section_layout)

            self.sections[title] = inner_scroll

        add_section("Airing Anime")
        add_section("Trending Now")
        add_section("Releasing this season")
        add_section("Trending Manga")
        add_section("Next Season")

        scroll_area.setWidget(container)

class HoverButton(QPushButton):
    def __init__(self, icon, parent=None):
        super().__init__(parent)
        self.default_size = QSize(30, 30)
        self.hover_size = QSize(35, 35)
        self.setIcon(icon)
        self.setIconSize(self.default_size)
        self.setStyleSheet("background: transparent; border: 0;")

    def enterEvent(self, event):
        self.setIconSize(self.hover_size)
        super().enterEvent(event)

    def leaveEvent(self, event):
        self.setIconSize(self.default_size)
        super().leaveEvent(event)

class FilterPopUp(QDialog):
    def __init__(self):
        super().__init__()
        check_boxes = []
        self.setWindowTitle("Filter")
        self.setFixedSize(200, 100)
        layout = QVBoxLayout(self)

        self.checkbox1 = QCheckBox("Anime")
        self.checkbox2 = QCheckBox("Manga")
        check_boxes.append(self.checkbox1)
        check_boxes.append(self.checkbox2)
        layout.addWidget(self.checkbox1)
        layout.addWidget(self.checkbox2)

        btn = QPushButton("Apply")
        btn.clicked.connect(self.accept)
        layout.addWidget(btn)

        def get_filters(self):
            checked = []
            if self.checkbox1.isChecked():
                checked.append(self.checkbox1.text())
            if self.checkbox2.isChecked():
                checked.append(self.checkbox2.text())
            return checked if checked else None

class ListAnime(QWidget):
    def __init__(self):
        super().__init__()

        # Main Layout
        layout = QVBoxLayout(self)

        # Header Container
        container = QWidget()
        container.setFixedHeight(35)
        container.setStyleSheet("background: transparent; border: 0;")
        container_layout = QHBoxLayout(container)
        container_layout.setContentsMargins(0, 0, 0, 0)

        # Label
        label = QLabel("Anime: ", alignment=Qt.AlignVCenter | Qt.AlignLeft)
        label.setStyleSheet("font-size: 20px; font-weight: bold;")

        # Button mit Icon
        icon = QIcon(path.join(get_cache_path(), "images", "filter.png"))
        button = HoverButton(icon)
        button.setStyleSheet("background: transparent; border: 0;")
        button.clicked.connect(lambda: FilterPopUp().exec())

        container_layout.addWidget(label)
        container_layout.addStretch(1)
        container_layout.addWidget(button)

        # ScrollArea
        scroll = QScrollArea()
        scroll.setStyleSheet("background: transparent; border: 0;")
        scroll.setWidgetResizable(True)

        # Container für Karten
        card_container = QWidget()
        self.card_layout = QVBoxLayout(card_container)
        self.card_layout.setContentsMargins(0, 0, 0, 0)
        self.card_layout.setSpacing(10)

        scroll.setWidget(card_container)

        # Add to main layout
        layout.addWidget(container)
        layout.addWidget(scroll)

class ListManga(QWidget):
    def __init__(self):
        super().__init__()


class Options(QDialog):
    # === Options for the Cards ===
    class CardOptions(QWidget):
        def __init__(self, parent=None):
            super().__init__(parent)
            self.main_layout = QHBoxLayout(self)
            self.main_layout.setContentsMargins(0, 0, 0, 0)
            self.main_layout.setSpacing(12)

            container_layout_anime = QVBoxLayout()
            container_layout_manga = QVBoxLayout()

            for l in (container_layout_anime, container_layout_manga):
                l.setContentsMargins(10, 0, 0, 0)
                l.setSpacing(4)
                l.setAlignment(Qt.AlignTop)

            anime_label = QLabel("Anime Card:")
            manga_label = QLabel("Manga Card:")
            anime_label.setStyleSheet("font-size:14pt; font-weight:bold; margin:0; padding:0;")
            manga_label.setStyleSheet("font-size:14pt; font-weight:bold; margin:0; padding:0;")

            container_layout_anime.addWidget(anime_label)
            container_layout_manga.addWidget(manga_label)

            # === Options ===
            options_anime = ["description", "episodes", "status", "score", "studios", "genres", "external_links",
                             "airing", "start_end", "popularity", "favorites", "source"]
            options_manga = ["description", "chapters", "volumes", "status", "score", "genres", "external_links",
                             "start_end", "popularity", "favorites", "source"]

            # Load Data from YAML
            configure = load_settings()
            card_options = configure.get("CARD_OPTIONS", {})
            anime_settings = card_options.get("ANIME", {})
            manga_settings = card_options.get("MANGA", {})

            self.checkboxes_anime = []
            self.checkboxes_manga = []

            for text in options_anime:
                cb = QCheckBox(text)
                cb.setStyleSheet("""
                    QCheckBox {
                        spacing: 10px;                  
                        font-size: 12pt;                
                    }
                    QCheckBox::indicator {
                        width: 20px;                     
                        height: 20px;
                    }
                """)
                cb.setChecked(anime_settings.get(text.lower(), False))
                cb.stateChanged.connect(self.check_max_anime)
                self.checkboxes_anime.append(cb)
                container_layout_anime.addWidget(cb)

            self.check_max_anime(0)

            for text in options_manga:
                cb = QCheckBox(text)
                cb.setStyleSheet("""
                    QCheckBox {
                        spacing: 10px;                  
                        font-size: 12pt;                
                    }
                    QCheckBox::indicator {
                        width: 20px;                     
                        height: 20px;
                    }
                """)
                cb.setChecked(manga_settings.get(text.lower(), False))  # Status aus YAML übernehmen
                cb.stateChanged.connect(self.check_max_manga)
                self.checkboxes_manga.append(cb)
                container_layout_manga.addWidget(cb)

            self.check_max_manga(0)

            btn = QPushButton("Apply")
            btn.clicked.connect(lambda: save_card_settings(self.checkboxes_anime, self.checkboxes_manga))

            self.main_layout.addLayout(container_layout_anime)
            self.main_layout.addLayout(container_layout_manga)
            self.main_layout.addWidget(btn)
            self.setLayout(self.main_layout)

        def check_max_anime(self, state):
            checked = sum(cb.isChecked() for cb in self.checkboxes_anime)
            for cb in self.checkboxes_anime:
                cb.setEnabled(cb.isChecked() or checked < 3)

        def check_max_manga(self, state):
            checked = sum(cb.isChecked() for cb in self.checkboxes_manga)
            for cb in self.checkboxes_manga:
                cb.setEnabled(cb.isChecked() or checked < 3)

    # === Options for the User Settings ===
    class UserSettings(QWidget):
        def __init__(self, parent=None):
            super().__init__(parent)

    # === Main Class ===
    def __init__(self):
        super().__init__()

        # === Make Window Frameless and Translucent ===
        self.setWindowFlags(Qt.FramelessWindowHint)
        self.setAttribute(Qt.WA_TranslucentBackground)

        # === Define Windows size ===
        width, height = 600, 600
        self.setFixedSize(width, height)

        # === Variable for dragging window ===
        self.dragPos = QPoint()

        # === Main container with rounded edges ===
        self.container = QWidget(self)
        self.container.setGeometry(0, 0, width, height)
        self.container.setStyleSheet("""
            background-color: #2c3e50;
            border-radius: 20px;
        """)

        # === Layout ===
        main_layout = QVBoxLayout(self.container)
        main_layout.setContentsMargins(20, 20, 20, 20)

        # === Create Header ===
        header_layout = QHBoxLayout()
        self.label = QLabel("AniLex Options")
        self.label.setStyleSheet("color: white; font-size: 14pt;")
        close_btn = QPushButton("X")
        close_btn.setStyleSheet("""
            QPushButton {
                background-color: #e74c3c;
                border: none;
                padding: 5px 12px;
                border-radius: 10px;
                color: white;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #c0392b;
            }
        """)
        close_btn.clicked.connect(self.close)

        spacer = QSpacerItem(20, 40, QSizePolicy.Expanding, QSizePolicy.Minimum)
        header_layout.addWidget(self.label)
        header_layout.addSpacerItem(spacer)
        header_layout.addWidget(close_btn)

        main_layout.addLayout(header_layout)
        # === Create Tabs ===
        tabs = CustomTabWidget()
        tabs.addTab(self.CardOptions(), "Card Options")
        tabs.addTab(self.UserSettings(), "User Settings")

        main_layout.addWidget(tabs)

    # ==== Fenster verschiebbar machen ====
    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.dragPos = event.globalPosition().toPoint() - self.frameGeometry().topLeft()
            event.accept()

    def mouseMoveEvent(self, event):
        if event.buttons() == Qt.LeftButton:
            self.move(event.globalPosition().toPoint() - self.dragPos)
            event.accept()

class Profile(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        btn = QPushButton("Options")
        btn.clicked.connect(lambda: Options().exec())
        layout.addWidget(btn)

class ChatBot(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("Das ist Tab 5", alignment=Qt.AlignCenter)
        layout.addWidget(label)

class Recommendations(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("Das ist Tab 6", alignment=Qt.AlignCenter)
        layout.addWidget(label)


# ---- Main Window ----
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("AniLex mit großen Tabs")
        self.setMinimumSize(800, 600)

        tabs = CustomTabWidget()
        tabs.addTab(Discover(), "Discover")
        tabs.addTab(ListAnime(), "List Anime")
        tabs.addTab(ListManga(), "List Manga")
        tabs.addTab(ChatBot(), "ChatBot")
        tabs.addTab(Recommendations(), "Recom")
        tabs.addTab(Profile(), "Profile")

        tabs.tabbar.setStyleSheet("background: transparent;")

        self.setCentralWidget(tabs)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())