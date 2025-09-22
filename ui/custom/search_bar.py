import os

from PySide6.QtCore import QRect
from PySide6.QtGui import QPainter, QIcon
from PySide6.QtWidgets import QLineEdit, QApplication

from utils.anilex_helper import get_cache_path


class SearchBar(QLineEdit):
    def __init__(self):
        super().__init__()
        self.setObjectName("anilexSearchBar")
        self.setStyleSheet("""
            QLineEdit {
                padding-left: 50px;  /* Increased padding for larger icon */
                font-size: 16px;
                min-height: 60px;
                min-width: 1000px;
                max-width: 1000px;
                max-height: 60px;
                border-radius: 30px;
                margin: 10px;
                color: #E0E0E0;
            }
        """)
        self.icon = QIcon(os.path.join(get_cache_path(), "images", "search.png"))
        self.icon_size = 30  # Adjust size as needed
        self.setPlaceholderText("Search Animes...")

    def paintEvent(self, event):
        super().paintEvent(event)
        painter = QPainter(self)
        icon_rect = QRect(20, (self.height() - self.icon_size) // 2,
                          self.icon_size, self.icon_size)
        self.icon.paint(painter, icon_rect)




if __name__ == "__main__":
    app = QApplication([])
    widget = SearchBar()
    widget.show()
    app.exec()