import sys
from datetime import datetime

from PySide6.QtWidgets import QApplication
from old.anilex_helper import _load_user_data
from old.ui.gui import MainWindow

from core.AniList.auth import AniListAuthenticator
from core.notifications.notification_handler import NotificationHandler

if __name__ == "__main__":
    user_data = _load_user_data()
    if user_data.get("auth", {}).get("user_token", "unsaved") == "unsaved":
        auth = AniListAuthenticator()
        auth.authenticate()
    elif datetime.fromisoformat(user_data.get("auth", {}).get("renew_token", None)) < datetime.now():
        auth = AniListAuthenticator()
        auth.authenticate(True)


    CURRENT_USER = ""

    app = QApplication(sys.argv)
    app.setApplicationName("UNOFICIAL-ANILEX")
    app.setApplicationVersion("0.1.0")
    app.setApplicationDisplayName("UNOFICIAL-ANILEX")

    window = MainWindow()
    window.showMaximized()

    handler = NotificationHandler()

    sys.exit(app.exec())
