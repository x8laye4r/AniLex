'''

'''
import sys
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLabel, QStackedWidget, QPushButton, QSizePolicy, QScrollArea, QDialog, QCheckBox
)
from PySide6.QtCore import Qt, QPropertyAnimation, QEasingCurve, Property, QSize
from PySide6.QtGui import QEnterEvent, QPixmap, QIcon
from utils.helpers import get_cache_path
from ui.card import Card
from ui.tab import CustomTabWidget
from os import path

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
'''
    class FilterDropDown(QDialog):
        print("Will be added in the future!")
'''


class ListAnime(QWidget):
    def __init__(self):
        super().__init__()

        self.right = True
        self.temp_left_card = None

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
        layout = QVBoxLayout(self)
        label = QLabel("Das ist Tab 3", alignment=Qt.AlignCenter)
        layout.addWidget(label)

class Profile(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("Das ist Tab 4", alignment=Qt.AlignCenter)
        layout.addWidget(label)

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