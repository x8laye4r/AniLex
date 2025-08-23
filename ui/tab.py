from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QPushButton, QSizePolicy, QStackedWidget, QHBoxLayout, QVBoxLayout, QTabBar
from PySide6.QtCore import Qt, Signal, QPropertyAnimation, QEasingCurve, Property, QSize
from PySide6.QtGui import QEnterEvent

class TabButton(QPushButton):
    def __init__(self, text, base_font_pt=20, parent=None):
        super().__init__(text, parent)
        self._scale = 1.0
        self.base_font_pt = base_font_pt
        self.setCheckable(True)
        self.setCursor(Qt.PointingHandCursor)
        fixed_height = int(self.base_font_pt * 2.6)
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
            QPushButton:checked:hover {{
                background: #1f6fb5;  /* Gleich wie checked, kein Hover-Effekt */
                color: white;
            }}
        """
        self.setStyleSheet(style)

    def enterEvent(self, event: QEnterEvent):
        if self.isChecked():
            return
        self._start_animation(target=1.15)
        super().enterEvent(event)

    # Reset
    def leaveEvent(self, event):
        if self.isChecked():
            return
        self._start_animation(target=1.0)
        super().leaveEvent(event)

    def _start_animation(self, target, duration=80):
        self.animation = QPropertyAnimation(self, b"scale")
        self.animation.setDuration(duration)
        self.animation.setEasingCurve(QEasingCurve.OutCubic)
        self.animation.setEndValue(target)
        self.animation.start()

    def mousePressEvent(self, event):
        if self.isChecked():
            return
        self._start_animation(target=0.95)
        super().mousePressEvent(event)

    def mouseReleaseEvent(self, e):
        if self.isChecked():
            return
        self._start_animation(target=1.0)
        super().mouseReleaseEvent(e)

    def isChecked(self):
        return super().isChecked()

    def setChecked(self, checked):
        super().setChecked(checked)
        if checked:
            self._start_animation(target=1.25)
        else:
            self._start_animation(target=1.0)

class TabBar(QWidget):
    currentChanged = Signal(int)

    def __init__(self, tabs: list[str], parent=None):
        super().__init__(parent)

        self._buttons: list[TabButton] = []
        self._current_index: int = -1

        layout = QHBoxLayout(self)
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(0)

        for i, text in enumerate(tabs):
            btn = TabButton(text)
            btn.clicked.connect(lambda checked, idx=i: self._on_tab_clicked(idx))
            layout.addWidget(btn)
            self._buttons.append(btn)

        # optional: ersten Tab aktivieren
        if self._buttons:
            self.setCurrentIndex(0)

    def _on_tab_clicked(self, index: int):
        self.setCurrentIndex(index)

    def setCurrentIndex(self, index: int):
        if index == self._current_index:
            return
        # Reset alle Buttons
        for i, btn in enumerate(self._buttons):
            btn.setChecked(i == index)
        self._current_index = index
        self.currentChanged.emit(index)

    def currentIndex(self) -> int:
        return self._current_index

    def buttons(self) -> list[TabButton]:
        return self._buttons