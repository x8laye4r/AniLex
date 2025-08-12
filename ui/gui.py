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
from os import path

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
                border: 0;
                font-weight: bold;
                font-size: {font_pt}pt;
            }}
            QPushButton:checked {{
                background: #1f6fb5;
                color: white;
                border-radius: 3px;
                border: 2px solid black;
            }}
            QPushButton: hover {{
                background: #1c65a5;
                border: 2px solid #1a609e;
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

class Card(QWidget):
    def __init__(self, image_path, title, description, parent=None):
        super().__init__(parent)

        self.image_label = QPixmap(image_path)
        self.image_label.setPixmap(pixmap.scaled(200, 300, aspectRatioMode=1))

        self.title = QLabel(title)
        self.title.styleSheet = "font-size: 20px; font-weight: bold;"

        self.description = QLabel(description)
        self.description_label.setWordWrap(True)
        self.description_label.setStyleSheet("font-size: 12px;")

        layout = QVBoxLayout(self)
        layout.addWidget(self.image_label)
        layout.addWidget(self.title)
        layout.addWidget(self.description)
        self.setLayout(layout)

    def resize_event(self, event):
        w = self.width()
        h = int(w * 1.5)
        pixmap = self.image_label.pixmap()
        if pixmap:
            self.image_label.setPixmap(pixmap.scaled(w, h, aspectRatioMode=1))
        super().resizeEvent(event)



class Discover(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout(self)
        label = QLabel("Das ist Tab 1", alignment=Qt.AlignCenter)
        layout.addWidget(label)

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

        # Main Window
        layout = QVBoxLayout(self)

        # Container
        container = QWidget()
        container.setFixedHeight(35)
        container.setStyleSheet("background: transparent; border: 0;")

        # Container Layout
        container_layout = QHBoxLayout(container)
        container_layout.setContentsMargins(0, 0, 0, 0)

        # Label
        label = QLabel("Anime: ", alignment=Qt.AlignVCenter | Qt.AlignLeft)
        label.setStyleSheet("font-size: 20px; font-weight: bold;")

        # Button with Animation
        icon = QIcon(path.join(get_cache_path(), "images", "filter.png"))
        button = HoverButton(icon)
        button.setStyleSheet("background: transparent; border: 0;")
        button.clicked.connect(lambda: FilterPopUp().exec())


        # ScrollArea
        scroll = QScrollArea()
        scroll.setStyleSheet("background: transparent; border: 0;")
        scroll.setWidget(QWidget())
        scroll.setWidgetResizable(False)

        container_layout.addWidget(label)
        container_layout.addStretch(1)
        container_layout.addWidget(button)

        # Add to the Layout
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
