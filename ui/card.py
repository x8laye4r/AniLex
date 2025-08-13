from PySide6.QtWidgets import QWidget, QHBoxLayout, QVBoxLayout, QLabel, QSizePolicy, QApplication
from PySide6.QtCore import Qt, Signal, QPropertyAnimation, QEasingCurve, QSize
from PySide6.QtGui import QPainter, QPainterPath, QColor, QPen, QFont, QPixmap


'''
    To-Do:
        1) Make the card colors better
        3) Fix cut corner 
'''
class CutCornerCardLeft(QWidget):
    clicked = Signal()
    def __init__(self, parent=None):
        super().__init__(parent)

        screen = QApplication.primaryScreen()
        screen_size = screen.size()
        width = screen_size.width() // 2
        height = 150

        self.normal_size = (width, height)
        self.hover_size = (width + 20, height + 10)

        self.resize(*self.normal_size)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.setMaximumSize(QSize(*self.normal_size))

        self.bg_color = QColor("#f0f0f0")
        self.border_color = QColor("#888")
        self.corner_cut_x = 150
        self.corner_cut_y = 150
        self.hover = False
        self._path = None

        self.size_anim = QPropertyAnimation(self, b"maximumSize")
        self.size_anim.setDuration(150)
        self.size_anim.setEasingCurve(QEasingCurve.OutCubic)

        main_layout = QHBoxLayout(self)
        main_layout.setContentsMargins(10, 10, 10, 10)
        main_layout.setSpacing(10)

        self.image_label = QLabel()
        img_size = height - 20
        self.image_label.setFixedSize(img_size, img_size)
        self.image_label.setAlignment(Qt.AlignCenter)

        text_layout = QVBoxLayout()
        self.title_label = QLabel("No Title Found...")
        self.title_label.setFont(QFont("Arial", 14, QFont.Bold))
        self.desc_label = QLabel("No Description Found...")
        self.desc_label.setWordWrap(True)

        text_layout.addWidget(self.title_label)
        text_layout.addWidget(self.desc_label)

        main_layout.addWidget(self.image_label)
        main_layout.addLayout(text_layout)

        self.setMouseTracking(True)

    def set_image(self, path: str):
        pixmap = QPixmap(path)
        if not pixmap.isNull():
            scaled = pixmap.scaled(self.image_label.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
            self.image_label.setPixmap(scaled)
        else:
            self.image_label.clear()

    def set_title(self, text: str):
        self.title_label.setText(text)

    def set_description(self, text: str):
        self.desc_label.setText(text)

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        rect = self.rect()

        path = QPainterPath()
        path.moveTo(rect.topLeft())
        path.lineTo(rect.topRight().x() - self.corner_cut_x, rect.top())
        path.lineTo(rect.topRight().x(), rect.top() + self.corner_cut_y)
        path.lineTo(rect.bottomRight())
        path.lineTo(rect.bottomLeft())
        path.closeSubpath()

        self._path = path

        fill_color = QColor("#dcdcdc") if self.hover else self.bg_color
        painter.fillPath(path, fill_color)

        pen = QPen(self.border_color)
        pen.setWidth(2)
        painter.setPen(pen)
        painter.drawPath(path)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton and self._path and self._path.contains(event.position()):
            self.clicked.emit()

    def enterEvent(self, event):
        self.hover = True
        self.update()
        self.size_anim.stop()
        self.size_anim.setStartValue(self.maximumSize())
        self.size_anim.setEndValue(QSize(*self.hover_size))
        self.size_anim.start()

    def leaveEvent(self, event):
        self.hover = False
        self.update()
        self.size_anim.stop()
        self.size_anim.setStartValue(self.maximumSize())
        self.size_anim.setEndValue(QSize(*self.normal_size))
        self.size_anim.start()

class CutCornerCardRight(QWidget):
    clicked = Signal()
    def __init__(self, parent=None):
        super().__init__(parent)

        screen = QApplication.primaryScreen()
        screen_size = screen.size()
        width = screen_size.width() // 2
        height = 150

        self.normal_size = (width, height)
        self.hover_size = (width + 20, height + 10)

        self.resize(*self.normal_size)
        self.setSizePolicy(QSizePolicy.Fixed, QSizePolicy.Fixed)
        self.setMaximumSize(QSize(*self.normal_size))

        self.bg_color = QColor("#f0f0f0")
        self.border_color = QColor("#888")
        self.corner_cut_x = 150
        self.corner_cut_y = 150
        self.hover = False
        self._path = None

        self.size_anim = QPropertyAnimation(self, b"maximumSize")
        self.size_anim.setDuration(150)
        self.size_anim.setEasingCurve(QEasingCurve.OutCubic)

        main_layout = QHBoxLayout(self)
        main_layout.setContentsMargins(10, 10, 10, 10)
        main_layout.setSpacing(10)

        text_layout = QVBoxLayout()
        self.title_label = QLabel("No Title Found...")
        self.title_label.setFont(QFont("Arial", 14, QFont.Bold))
        self.desc_label = QLabel("No Description Found...")
        self.desc_label.setWordWrap(True)

        self.image_label = QLabel()
        img_size = height - 20
        self.image_label.setFixedSize(img_size, img_size)
        self.image_label.setAlignment(Qt.AlignCenter)

        text_layout.addWidget(self.title_label)
        text_layout.addWidget(self.desc_label)

        main_layout.addLayout(text_layout)
        main_layout.addWidget(self.image_label)

        self.setMouseTracking(True)

    def set_image(self, path: str):
        pixmap = QPixmap(path)
        if not pixmap.isNull():
            scaled = pixmap.scaled(self.image_label.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
            self.image_label.setPixmap(scaled)
        else:
            self.image_label.clear()

    def set_title(self, text: str):
        self.title_label.setText(text)

    def set_description(self, text: str):
        self.desc_label.setText(text)

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        rect = self.rect()

        path = QPainterPath()
        path.moveTo(rect.topLeft())
        path.lineTo(rect.topRight())
        path.lineTo(rect.bottomRight())
        path.lineTo(rect.bottomLeft().x() + self.corner_cut_x, rect.bottom())
        path.lineTo(rect.bottomLeft().x(), rect.bottomLeft().y() - self.corner_cut_y)
        path.closeSubpath()

        self._path = path

        fill_color = QColor("#dcdcdc") if self.hover else self.bg_color
        painter.fillPath(path, fill_color)

        pen = QPen(self.border_color)
        pen.setWidth(2)
        painter.setPen(pen)
        painter.drawPath(path)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton and self._path and self._path.contains(event.position()):
            self.clicked.emit()

    def enterEvent(self, event):
        self.hover = True
        self.update()
        self.size_anim.stop()
        self.size_anim.setStartValue(self.maximumSize())
        self.size_anim.setEndValue(QSize(*self.hover_size))
        self.size_anim.start()

    def leaveEvent(self, event):
        self.hover = False
        self.update()
        self.size_anim.stop()
        self.size_anim.setStartValue(self.maximumSize())
        self.size_anim.setEndValue(QSize(*self.normal_size))
        self.size_anim.start()

