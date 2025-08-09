'''
    Here will be UI made with Pyside6
'''

import sys
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout,
    QLabel, QTabWidget, QPushButton, QSpacerItem, QSizePolicy
)
from PySide6.QtCore import Qt

class Discover(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 1", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

class ListAnime(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 2", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

class ListManga(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 3", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

class Profile(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 4", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

class ChatBot(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 5", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

class Recommendations(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 6", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        self.setLayout(layout)

# Hauptfenster
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("AniLex mit großen Tabs")
        self.setMinimumSize(800, 600)

        tabs = QTabWidget()
        tabs.setStyleSheet("""
            QTabBar::tab {
                min-width: 200px;
                display: flex;
                height: 40px;
                font-size: 18pt;
                padding: 8px;
            }
            QTabBar::tab:selected {
                background: #0078d7;
                color: white;
                font-weight: bold;
            }
            QTabBar::tab:hover {
                background: #3399ff;
            }
        """)

        self.setCentralWidget(tabs)

        tabs.addTab(Discover(), "Discover")
        tabs.addTab(ListAnime(), "List Anime")
        tabs.addTab(ListManga(), "List Manga")
        tabs.addTab(ChatBot(), "ChatBot")
        tabs.addTab(Recommendations(), "Recom")
        tabs.addTab(Profile(), "Profile")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())
