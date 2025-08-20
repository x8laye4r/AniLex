from PySide6.QtWidgets import QWidget, QHBoxLayout, QVBoxLayout, QLabel, QSizePolicy, QApplication
from PySide6.QtCore import Qt, Signal, QPropertyAnimation, QEasingCurve, QSize
from PySide6.QtGui import QPainter, QPainterPath, QColor, QPen, QFont, QPixmap
from utils.helpers import get_card_settings

'''
    To-Do:
        1) Make the card colors better
        3) Fix cut corner 
'''

class Card(QWidget):
    def __init__(self, id: int = None, image_url: str = "", title: str = "None...", description: str = "None..", parent=None):
        super().__init__(parent)
        self.id = id
        self.image_url = image_url
        self.title = title
        self.description = description

        layout = QHBoxLayout()
        layout.setContentsMargins(8, 8, 8, 8)
        layout.setSpacing(12)

        layout2 = QVBoxLayout()
        layout2.setContentsMargins(0, 0, 0, 0)
        layout2.setSpacing(2)

        # Title + Description
        label_title = QLabel(self.title)
        label_desc = QLabel(self.description)

        # Styling
        label_title.setStyleSheet('font-size: 20pt; font-weight: bold;')
        label_desc.setWordWrap(True)

        label_title.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        label_desc.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)

        label_title.setAlignment(Qt.AlignLeft | Qt.AlignVCenter)
        label_desc.setAlignment(Qt.AlignLeft | Qt.AlignTop)

        layout2.addWidget(label_title)
        layout2.addWidget(label_desc)

        self.label_pic = QLabel()
        self.label_pic.setFixedSize(120, 170)
        self.label_pic.setAlignment(Qt.AlignCenter)

        layout.addWidget(self.label_pic, 0, Qt.AlignLeft | Qt.AlignVCenter)
        layout.addLayout(layout2)
        layout.setAlignment(layout2, Qt.AlignVCenter)

        self.manager = QNetworkAccessManager()
        if self.image_url:
            request = QNetworkRequest(QUrl(self.image_url))
            reply = self.manager.get(request)
            reply.finished.connect(lambda r=reply: self.handle_reply(r))

        self.setLayout(layout)

    def handle_reply(self, reply):
        pixmap = QPixmap()
        pixmap.loadFromData(reply.readAll())
        self.label_pic.setPixmap(
            pixmap.scaled(
                self.label_pic.size(),
                Qt.KeepAspectRatio,
                Qt.SmoothTransformation
            )
        )

