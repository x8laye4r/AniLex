from PySide6.QtWidgets import QApplication
from ui.gui import MainWindow
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setApplicationName(u"UNOFICIAL-ANILEX")
    app.setApplicationVersion(u"0.1.0")
    app.setApplicationDisplayName(u"UNOFICIAL-ANILEX")
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())
