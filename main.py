import sys

from PySide6.QtWidgets import QApplication

from core.notifications.notification_handler import NotificationHandler
from ui.gui import MainWindow

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setApplicationName("UNOFICIAL-ANILEX")
    app.setApplicationVersion("0.1.0")
    app.setApplicationDisplayName("UNOFICIAL-ANILEX")

    window = MainWindow()
    window.showMaximized()

    handler = NotificationHandler()

    sys.exit(app.exec())
