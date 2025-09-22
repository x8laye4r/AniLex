import sys
from os import path

from PySide6.QtCore import Qt, QSize
from PySide6.QtGui import QIcon
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QStackedWidget, QPushButton, QScrollArea, QDialog, QCheckBox
)

from ui.apis.animechangui import AnimeChanGUI
from ui.apis.aniworldgui import AniWorldGUI
from ui.apis.hianimezgui import HiAnimezGUI
from ui.apis.nekos_images import NekoImageGUI
from ui.apis.trace_moe_searchV2 import TraceMoeGUI
from ui.apis.waifu_images import WaifuPicturesGUI
from ui.custom.tab import TabBar
from ui.custom.vertical_tab import AutoHideVerticalTabs
from utils.anilex_helper import get_cache_path


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

# Implement new Custom QDialog for Options
class Options(QDialog):
    def __init__(self):
        super().__init__()


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


class Tools(QWidget):
    def __init__(self):
        super().__init__()

        # Create a horizontal layout
        self.layout = QHBoxLayout(self)
        self.layout.setContentsMargins(0, 0, 0, 0)
        self.layout.setSpacing(0)

        # Tab names
        self.tab_names = [
            "Nekos Images",
            "WaifuIm Images",
            "Trace.moe",
            "SauceNAO",
            "HiAnimez",
            "AniWorld",
            "Animechan"
        ]

        # Create vertical tab bar
        self.vertical_tab = AutoHideVerticalTabs(
            expanded_width=200,
            collapsed_width=70,
            collapse_delay=1500,
            background_color="#1e2329",
            font_size=12
        )
        self.vertical_tab.setup_tabs(self.tab_names)

        # Create content stack
        self.content_stack = QStackedWidget()

        # Create and add widget instances for each tab
        self.tab_widgets = [
            NekoImageGUI(),
            WaifuPicturesGUI(),
            TraceMoeGUI(),
            HiAnimezGUI(),
            AniWorldGUI(),
            AnimeChanGUI(),
        ]

        # Add widgets to stack
        for widget in self.tab_widgets:
            self.content_stack.addWidget(widget)

        # Connect signals
        self.vertical_tab.currentChanged.connect(self.on_tab_changed)

        # Add widgets to main layout
        self.layout.addWidget(self.vertical_tab)
        self.layout.addWidget(self.content_stack)

    def on_tab_changed(self, index):
        self.content_stack.setCurrentIndex(index)

    def add_new_section(self, name, widget_class=None):
        # Add new tab
        new_index = self.vertical_tab.add_tab(name)

        # Create widget for the new tab
        if widget_class:
            widget = widget_class()
        else:
            # Default widget if no class provided
            widget = QWidget()
            layout = QVBoxLayout(widget)
            layout.addWidget(QLabel(f"Content for {name}", alignment=Qt.AlignCenter))

        # Add to content stack and tab list
        self.content_stack.addWidget(widget)
        self.tab_widgets.append(widget)

        return new_index


# ---- Main Window ----
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("AniLex")
        self.setMinimumSize(800, 600)

        self.tab_bar = TabBar(["Discover", "Anime", "Manga", "ChatBot", "Tools", "Profile"])
        self.tab_bar.currentChanged.connect(self.on_tab_changed)

        self.stack = QStackedWidget()
        self.stack.addWidget(Discover())
        self.stack.addWidget(ListAnime())
        self.stack.addWidget(ListManga())
        self.stack.addWidget(ChatBot())
        self.stack.addWidget(Tools())
        self.stack.addWidget(Profile())

        container = QWidget()
        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(0)
        layout.addWidget(self.tab_bar)
        layout.addWidget(self.stack)
        container.setLayout(layout)
        self.setCentralWidget(container)

        self.stack.setCurrentIndex(0)

    def on_tab_changed(self, index):
        self.stack.setCurrentIndex(index)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())