'''
    Here will be UI made with Pyside6
'''

import sys
from PySide6.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout,
    QLabel, QTabWidget, QPushButton
)
from PySide6.QtCore import Qt

class Discover(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 1", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Klick mich in Tab 1")
        button.clicked.connect(lambda: label.setText("Tab 1 Button geklickt!"))
        layout.addWidget(button)

        self.setLayout(layout)

class ListAnime(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 2", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Klick mich in Tab 2")
        button.clicked.connect(lambda: label.setText("Tab 2 Button geklickt!"))
        layout.addWidget(button)

        self.setLayout(layout)

class ListManga(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 3", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Klick mich in Tab 3")
        button.clicked.connect(lambda: label.setText("Tab 3 Button geklickt!"))
        layout.addWidget(button)

        self.setLayout(layout)

class Profile(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 4", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Click mich in Tab 4")
        button.clicked.connect(lambda: label.setText("Tab 4 Button geklickt!"))
        layout.addWidget(button)

        self.setLayout(layout)

class ChatBot(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 5", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Click mich in Tab 5")
        button.clicked.connect(lambda: label.setText("Tab 5 Button geklickt!"))
        layout.addWidget(button)

        self.setLayout(layout)

class Recommendations(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        label = QLabel("Das ist Tab 6", alignment=Qt.AlignCenter)
        layout.addWidget(label)

        button = QPushButton("Click mich in Tab 6")
        button.clicked.connect(lambda: label.setText("Tab 6 Button geklickt!"))
        layout.addWidget(button)

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
                min-width: 150px;
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
        tabs.addTab(Profile(), "Profile")
        tabs.addTab(ChatBot(), "ChatBot")
        tabs.addTab(Recommendations(), "Recommendations")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())
