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
from ui.card import CutCornerCardLeft, CutCornerCardRight
from os import path

'''
    To Do:
        1) Make QScrollArea arrangement from top left to bottom right
        2) Make scroll bar better
        3) Make a drop down for the filter button
        4) Make tabs color in a better colors
'''

# Custom TabButton which scales automatically and has animations
class TabButton(QPushButton):
    def __init__(self, text, base_font_pt=20, parent=None):
        super().__init__(text, parent)
        self._scale = 1.0
        self.base_font_pt = base_font_pt
        self.setCheckable(True)
        self.setCursor(Qt.PointingHandCursor)

        # Fixed height so the TabBar isn't moving by when hovered over a button
        fixed_height = int(self.base_font_pt * 2.6) # some extra place for bigger text
        self.setFixedHeight(fixed_height)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        self._update_style()

    def getScale(self):
        return self._scale

    def setScale(self, v):
        self._scale = v
        self._update_style()

    scale = Property(float, getScale, setScale)

    def _update_style(self):
        max_scale = 1.25
        scale = min(self._scale, max_scale)
        font_pt = max(1, int(self.base_font_pt * scale))

        style = f"""
            QPushButton {{
                background: transparent;
                font-weight: bold;
                font-size: {font_pt}pt;
                border: 0;
                border-radius: 3px;
            }}
            QPushButton:checked {{
                background: #1f6fb5;
                color: white;
                border-radius: 3px;
            }}
            QPushButton:hover {{
                background: black;
                border-radius: 3px;
            }}
        """
        self.setStyleSheet(style)

    def enterEvent(self, event: QEnterEvent):
        # Bigger Text
        self._start_animation(target=1.15)
        super().enterEvent(event)

    # Reset
    def leaveEvent(self, event):
        self._start_animation(target=1.0)
        super().leaveEvent(event)

    def _start_animation(self, target=1.0, duration=180):
        anim = QPropertyAnimation(self, b"scale", self)
        anim.setDuration(duration)
        anim.setEasingCurve(QEasingCurve.OutCubic)
        anim.setStartValue(self._scale)
        anim.setEndValue(target)
        anim.start()

# Custom Tab Widget
class CustomTabWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self._buttons = []
        self._pages = QStackedWidget()
        self._tabbar = QWidget()
        tab_layout = QHBoxLayout(self._tabbar)
        tab_layout.setContentsMargins(6, 6, 6, 6)
        tab_layout.setSpacing(6)
        self._tab_layout = tab_layout

        main = QVBoxLayout(self)
        main.setContentsMargins(0, 0, 0, 0)
        main.setSpacing(0)
        main.addWidget(self._tabbar)
        main.addWidget(self._pages)

    def addTab(self, widget: QWidget, title: str):
        index = self._pages.addWidget(widget)

        btn = TabButton(title, base_font_pt=18, parent=self._tabbar)
        btn.clicked.connect(lambda checked, i=index: self.setCurrentIndex(i))
        if index == 0:
            btn.setChecked(True)

        self._buttons.append(btn)
        self._tab_layout.addWidget(btn)
        return index

    def setCurrentIndex(self, index: int):
        self._pages.setCurrentIndex(index)
        for i, b in enumerate(self._buttons):
            b.setChecked(i == index)

    def currentIndex(self):
        return self._pages.currentIndex()

    def count(self):
        return self._pages.count()

    def widget(self, index):
        return self._pages.widget(index)

    @property
    def tabbar(self):
        return self._tabbar

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

        # Testeinträge
        self.add_card("Test L", "Test", "Test")
        self.add_card("Test R", "Test", "Test")
        self.add_card("Test L 2", "Test", "Test")
        self.add_card("Test R 2", "Test", "Test")

    def add_card(self, title: str, img_path: str, desc: str):
        if self.right:
            self.temp_left_card = CutCornerCardLeft()
            self.temp_left_card.set_title(title)
            self.temp_left_card.set_description(desc)
            self.temp_left_card.set_image(img_path)
            self.right = False
        else:
            right_card = CutCornerCardRight()
            right_card.set_title(title)
            right_card.set_description(desc)
            right_card.set_image(img_path)

            row_widget = QWidget()
            row_layout = QHBoxLayout(row_widget)
            row_layout.setContentsMargins(0, 0, 0, 0)
            row_layout.setSpacing(0)

            row_layout.addWidget(self.temp_left_card)
            row_layout.addWidget(right_card)

            self.card_layout.addWidget(row_widget)
            self.right = True


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