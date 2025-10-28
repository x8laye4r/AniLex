import os

from PySide6.QtCore import QTimer
from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QApplication, QWidget, QPushButton, QStackedLayout, QVBoxLayout
)

dll_path = os.path.join(os.path.dirname(__file__))
os.environ['PATH'] = dll_path + os.pathsep + os.environ['PATH']
import mpv
import sys


class MPVPlayer(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setAttribute(Qt.WidgetAttribute.WA_NativeWindow)
        self.setAttribute(Qt.WidgetAttribute.WA_DontCreateNativeAncestors)

        self.player = mpv.MPV(
            wid=str(int(self.winId())),
            osc=False,
            ytdl=True
        )

    def play(self, path):
        self.player.play(path)

    def toggle_pause(self):
        self.player.pause = not self.player.pause


class MPVWindow(QWidget):
    def __init__(self, player_widget, parent=None):
        super().__init__(parent)
        self.setAttribute(Qt.WidgetAttribute.WA_TransparentForMouseEvents, False)
        self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)

        self.setStyleSheet("background: transparent;")

        layout = QVBoxLayout()

        btn = QPushButton("⏸")
        btn.setFixedSize(50, 50)
        btn.setStyleSheet("""
            QPushButton {
                background-color: rgba(0, 0, 0, 150);
                border: none;
                border-radius: 25px;
                color: white;
                font-size: 24px;
            }
        """)
        btn.clicked.connect(player_widget.toggle_pause)
        layout.addWidget(btn, alignment=Qt.AlignTop | Qt.AlignRight)

        self.hide_timer = QTimer()
        self.hide_timer.setInterval(3000)
        self.hide_timer.timeout.connect(self.hide)
        self.setLayout(layout)

        # Starten Sie den Timer-Mechanismus hier, anstatt den Timer direkt zu starten
        self.start_hide_timer()

    def start_hide_timer(self):
        self.show()
        self.raise_()
        self.hide_timer.start()

    def mouseMoveEvent(self, event, /):
        self.start_hide_timer()


class EmbeddedPlayer(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.player_widget = MPVPlayer(self)
        self.overlay = MPVWindow(self.player_widget, self)

        layout = QStackedLayout(self)
        layout.setStackingMode(QStackedLayout.StackingMode.StackAll)
        layout.addWidget(self.player_widget)
        layout.addWidget(self.overlay)

        self.setMouseTracking(True)
        self.overlay.setMouseTracking(True)

    def play(self, path):
        self.player_widget.play(path)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = EmbeddedPlayer()
    w.play("https://vidmoly.net/embed-nwu38hmvkq2n.html")  # Stellen Sie sicher, dass diese Videodatei existiert
    w.resize(900, 600)
    w.show()
    sys.exit(app.exec())