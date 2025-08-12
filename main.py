from PySide6.QtWidgets import QApplication
from ui.gui import MainWindow
import sys
if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.showMaximized()
    sys.exit(app.exec())
