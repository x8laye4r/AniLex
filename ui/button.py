from PySide6.QtWidgets import QPushButton, QSizePolicy
from PySide6.QtCore import Property, Qt, QPropertyAnimation, QEasingCurve
from PySide6.QtGui import QEnterEvent

# Custom Button with hover animation
class AnimatedButton(QPushButton):
    def __init__(self, text, base_font_pt=14, parent=None):
        super().__init__(text, parent)
        self._scale = 1.0
        self.base_font_pt = base_font_pt
        self.setCursor(Qt.PointingHandCursor)
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        self._update_style()

    def getScale(self):
        return self._scale

    def setScale(self, v):
        self._scale = v
        self._update_style()

    scale = Property(float, getScale, setScale)

    def _update_style(self):
        max_scale = 1.2
        scale = min(self._scale, max_scale)
        font_pt = max(1, int(self.base_font_pt * scale))
        fixed_height = int(self.base_font_pt * 2.2 * scale)  # Dynamische Höhe

        style = f"""
            QPushButton {{
                background: #0078d7;
                color: white;
                font-weight: bold;
                font-size: {font_pt}pt;
                border: none;
                border-radius: 5px;
                padding: 10px 20px;
            }}
            QPushButton:hover {{
                background: #005a9e;
            }}
            QPushButton:pressed {{
                background: #004578;
            }}
        """
        self.setStyleSheet(style)
        self.setFixedHeight(fixed_height)

    def enterEvent(self, event: QEnterEvent):
        self._start_animation(target=1.1)
        super().enterEvent(event)

    def leaveEvent(self, event):
        self._start_animation(target=1.0)
        super().leaveEvent(event)

    def _start_animation(self, target):
        self.animation = QPropertyAnimation(self, b"scale")
        self.animation.setDuration(80)
        self.animation.setEasingCurve(QEasingCurve.OutQuad)
        self.animation.setEndValue(target)
        self.animation.start()

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        # Optional: Add a quick scale down effect on press
        self._start_animation(target=0.95)

    def mouseReleaseEvent(self, event):
        super().mouseReleaseEvent(event)
        # Restore scale on release
        self._start_animation(target=1.0)

if __name__ == "__main__":
    from PySide6.QtWidgets import QApplication, QVBoxLayout, QWidget

    app = QApplication([])

    window = QWidget()
    layout = QVBoxLayout(window)

    button1 = AnimatedButton("Click Me", base_font_pt=16)
    button2 = AnimatedButton("Another Button", base_font_pt=16)

    layout.addWidget(button1)
    layout.addWidget(button2)

    window.show()
    app.exec()