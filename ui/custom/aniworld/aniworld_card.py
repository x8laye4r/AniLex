from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel, QFrame
from PySide6.QtCore import Qt
from PySide6.QtGui import QPixmap, QPainter, QPainterPath
import sqlite3
import os
from utils.anilex_helper import get_cache_path
from ui.custom.outlineslabel import OutlinedLabel

class AniWorldCard(QWidget):
    """
    A widget to display an anime card with an image, title, and optional season/episode info.
    """

    def __init__(self, anime_id, title, image_data, season=None, episode=None):
        super().__init__()
        self.anime_id = anime_id
        self.title_text = title
        self.image_data = image_data
        self.season = season
        self.episode = episode

        self._init_ui()

    @staticmethod
    def rounded_pixmap(image_data, radius, size=(220, 312)):
        """
        Creates a rounded QPixmap from image data.
        """
        pixmap = QPixmap()
        pixmap.loadFromData(image_data)
        pixmap = pixmap.scaled(size[0], size[1], Qt.AspectRatioMode.KeepAspectRatioByExpanding,
                               Qt.TransformationMode.SmoothTransformation)

        # Crop the pixmap to the target size from the center
        crop_x = (pixmap.width() - size[0]) / 2
        crop_y = (pixmap.height() - size[1]) / 2
        pixmap = pixmap.copy(crop_x, crop_y, size[0], size[1])

        rounded = QPixmap(pixmap.size())
        rounded.fill(Qt.GlobalColor.transparent)

        painter = QPainter(rounded)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)

        path = QPainterPath()
        path.addRoundedRect(0, 0, pixmap.width(), pixmap.height(), radius, radius)

        painter.setClipPath(path)
        painter.drawPixmap(0, 0, pixmap)
        painter.end()

        return rounded

    def _init_ui(self):
        """
        Initializes the user interface of the card.
        """
        main_layout = QVBoxLayout(self)
        main_layout.setContentsMargins(0, 0, 0, 0)
        main_layout.setSpacing(0)

        # --- Image Container ---
        image_frame = QFrame(self)
        image_frame.setObjectName("imageFrame")
        image_frame.setStyleSheet("""
            #imageFrame {
                background-color: #2c2f33;
                border-top-left-radius: 10px;
                border-top-right-radius: 10px;
                border-bottom-left-radius: 10px;   
                border-bottom-right-radius: 10px;
            }
        """)
        image_frame.setFixedSize(220, 312)
        image_layout = QVBoxLayout(image_frame)
        image_layout.setContentsMargins(0, 0, 0, 0)

        image_label = QLabel()
        pix = self.rounded_pixmap(self.image_data, 10)
        image_label.setPixmap(pix)
        image_label.setFixedSize(pix.size())
        image_label.setScaledContents(True)

        image_layout.addWidget(image_label)

        # --- Info Label (Season/Episode) ---
        if self.season is not None and self.episode is not None:
            info_label = QLabel(f"S{self.season:02d} E{self.episode:02d}", image_label)
            info_label.setAlignment(Qt.AlignmentFlag.AlignRight)
            info_label.setStyleSheet("""
                QLabel {
                    background: rgba(0, 0, 0, 0.7);
                    color: white;
                    font-size: 12px;
                    font-weight: bold;
                    padding: 2px 5px;
                    border-bottom-left-radius: 5px;
                    border-top-right-radius: 10px;
                }
            """)
            # Position it at the top-right corner of the image_label
            info_label.adjustSize()
            info_label.move(image_label.width() - info_label.width(), 0)
        elif self.season is not None and self.episode is None:
            info_label = QLabel(f"Season {self.season}", image_label)
            info_label.setAlignment(Qt.AlignmentFlag.AlignRight)
            info_label.setStyleSheet("""
                QLabel {
                    background: rgba(0, 0, 0, 0.7);
                    color: white;
                    font-size: 12px;
                    font-weight: bold;
                    padding: 2px 5px;
                    border-bottom-left-radius: 5px;
                    border-top-right-radius: 10px;
                }
            """)
            # Position it at the top-right corner of the image_label
            info_label.adjustSize()
            info_label.move(image_label.width() - info_label.width(), 0)

        # --- Title Container ---
        title = OutlinedLabel(self.title_text, 3)
        title.setContentsMargins(0, 0, 0, 0)
        font_size = max(1, len(self.title_text))
        font_size = max(18, int(36 - (font_size * 0.8)))
        title.setStyleSheet(f"""
            QLabel {{
                color: white;
                font-size: {font_size}px;
                font-weight: bold;
                padding-bottom: 5px;
                background: qlineargradient(
                    x1: 0, y1: 1,  
                    x2: 0, y2: 0, 
                    stop: 0 rgba(0, 0, 0, 200),   
                    stop: 1 rgba(0, 0, 0, 0)     
                );
                border-bottom-left-radius: 10px;
                border-bottom-right-radius: 10px;
            }}
        """)

        image_layout.addWidget(title)
        main_layout.addWidget(image_frame)
        self.setLayout(main_layout)