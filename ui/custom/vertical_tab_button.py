from PySide6.QtCore import Property, QPropertyAnimation, QEasingCurve, Qt
from PySide6.QtGui import QEnterEvent, QFont
from PySide6.QtWidgets import QPushButton, QSizePolicy


class VerticalTabButton(QPushButton):
    """
    Custom button designed specifically for vertical tab panels with
    auto-hiding functionality and smooth animations.
    """

    def __init__(self, text, base_font_pt=10, parent=None):
        super().__init__(text, parent)

        # Button properties
        self._scale = 1.0
        self.base_font_pt = base_font_pt
        self.full_text = text
        self.short_text = text[:2] if len(text) > 1 else text

        # Start with short text by default
        super().setText(self.short_text)

        # Button configuration
        self.setCheckable(True)
        self.setCursor(Qt.PointingHandCursor)
        self.setFixedHeight(int(self.base_font_pt * 3.5))
        self.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)

        # Font configuration
        self.font = QFont()
        self.font.setBold(True)
        self.setFont(self.font)

        # Initial style
        self._update_style()

    def getScale(self):
        return self._scale

    def setScale(self, value):
        self._scale = value
        self._update_style()

    # Property for animations
    scale = Property(float, getScale, setScale)

    def _update_style(self):
        max_scale = 1.25
        scale = min(self._scale, max_scale)
        font_pt = max(1, int(self.base_font_pt * scale))

        style = f"""
            QPushButton {{
                background-color: transparent;
                color: #e0e0e0;
                font-size: {font_pt}pt;
                text-align: center;
                border: none;
                border-radius: 4px;
                padding: 10px;
                margin: 2px 3px;
            }}
            QPushButton:hover {{
                background-color: rgba(255, 255, 255, 0.1);
                color: white;
            }}
            QPushButton:checked {{
                background-color: #1f6fb5;
                color: white;
            }}
            QPushButton:pressed {{
                background-color: #164e82;
            }}
        """
        self.setStyleSheet(style)

    def enterEvent(self, event: QEnterEvent):
        if not self.isChecked():
            self._animate_scale(target=1.1)
        super().enterEvent(event)

    def leaveEvent(self, event):
        if not self.isChecked():
            self._animate_scale(target=1.0)
        super().leaveEvent(event)

    def mousePressEvent(self, event):
        if not self.isChecked():
            self._animate_scale(target=0.95)
        super().mousePressEvent(event)

    def mouseReleaseEvent(self, event):
        if not self.isChecked():
            self._animate_scale(target=1.0)
        super().mouseReleaseEvent(event)

    def _animate_scale(self, target, duration=100):
        self.anim = QPropertyAnimation(self, b"scale")
        self.anim.setDuration(duration)
        self.anim.setStartValue(self._scale)
        self.anim.setEndValue(target)
        self.anim.setEasingCurve(QEasingCurve.OutCubic)
        self.anim.start()

    def setChecked(self, checked):
        super().setChecked(checked)
        if checked:
            self._animate_scale(target=1.15)
        else:
            self._animate_scale(target=1.0)

    def setText(self, text):
        """Override setText to store the full text value"""
        self.full_text = text
        super().setText(text)

    def setShortText(self):
        """Set abbreviated text for collapsed mode"""
        super().setText(self.short_text)

    def setFullText(self):
        """Set full text for expanded mode"""
        super().setText(self.full_text)