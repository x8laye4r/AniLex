from PySide6.QtCore import Qt
from PySide6.QtWidgets import QApplication, QWidget, QLabel, QVBoxLayout, QHBoxLayout


class AnilexTableItem(QWidget):
    def __init__(self, title, lang, season, episode, time = None,parent = None):
        super().__init__(parent)
        self.title = title
        self.lang = lang
        self.season = season
        self.episode = episode
        self.time = time

        self.setStyleSheet("background-color: gray; border-radius: 4px;") 
        self.setAttribute(Qt.WA_StyledBackground, True)

        self._init_ui()


    def _init_ui(self):
        main_layout = QHBoxLayout()



        title_label = QLabel("")
        if len(self.title) > 40: 
            title_label.setText(self.title[:40] + "...")
            title_label.setToolTip(self.title)
            title_label.setStyleSheet("""
                QLabel {
                    font-size: 18px;
                    font-weight: bold;
                    background: transparent;
                }
                QToolTip {
                    font-size: 14px;
                    padding: 4px;
                    border-radius: 4px;
                    background-color: #333333;
                    color: white;   
                }
            """)
        else:
            title_label.setText(self.title)
            title_label.setStyleSheet("""
                QLabel {
                    font-size: 18px;
                    font-weight: bold;
                    background: transparent;
                }
            """)
        title_label.setAlignment(Qt.AlignmentFlag.AlignTop | Qt.AlignmentFlag.AlignLeft | Qt.AlignmentFlag.AlignVCenter)

        season_episode_label = QLabel(f"S{self.season} E{self.episode}")
        season_episode_label.setStyleSheet("""
            QLabel {
                font-size: 16px;
                font-weight: bold; 
                color: black;
                background-color: #e0e0e0;
                border-radius: 4px; 
                margin-left: 4px;
                margin-right: 4px;
            }
        """)
        season_episode_label.setAlignment(Qt.AlignmentFlag.AlignRight | Qt.AlignmentFlag.AlignVCenter | Qt.AlignmentFlag.AlignTop)

        main_layout.addWidget(title_label)
        main_layout.addStretch()
        main_layout.addWidget(season_episode_label)

        self.setLayout(main_layout)


if __name__ == "__main__":
    app = QApplication([])


    layout = QVBoxLayout()
    window = QWidget()
    window.setLayout(layout)
    test_item = AnilexTableItem("Sample Title Test Test ", "japanese-german", "1", "1", "12:00 PM")
    layout.addWidget(test_item)
    window.show()

    app.exec() 