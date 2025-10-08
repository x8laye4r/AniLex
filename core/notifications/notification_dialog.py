from typing import Optional, Tuple, TYPE_CHECKING

from PySide6.QtCore import Qt, QEvent, QTimer
from PySide6.QtGui import QPixmap, QPainter, QImage, QPainterPath
from PySide6.QtWidgets import QLabel, QVBoxLayout, QHBoxLayout, QWidget, QDialog, QPushButton, QApplication

if TYPE_CHECKING:
    from core.notifications.notification_handler import NotificationHandler

class Notification(QDialog):
    """
    A custom notification toast dialog that displays information on screen
    This includes title, season and episode number, optional poster and banner images.
    The dialog auto-dismisses after a timeout unless hovered over.
    :param title: The title of the notification.
    :param season_num: The season number to display.
    :param episode_num: The episode number to display.
    :param poster: Optional bytes of the poster image.
    :param banner: Optional bytes of the banner image.
    :param handler: The NotificationHandler managing this notification.
    :param offset_top: Vertical offset from the top of the screen.
    :param offset_right: Horizontal offset from the right of the screen.
    :param fixed_width: Fixed width of the notification dialog.
    :param width_factor: Optional factor of screen width to determine dialog width.
    :param banner_max_height: Maximum height for the banner image.
    :param poster_height: Height for the poster image.
    :param corner_radius: Corner radius for rounded corners on images.
    :param compact_mode: Whether to use compact styling.
    :param dismiss_timeout: Time in milliseconds before auto-dismissal.
    :param parent: Optional parent widget.
    """
    def __init__(
        self,
        title: str,
        season_num: int,
        episode_num: int,
        poster: Optional[bytes] = None,
        banner: Optional[bytes] = None,
        *,
        handler: "NotificationHandler",
        offset_top: int = 40,
        offset_right: int = 40,
        fixed_width: int = 380,
        width_factor: float | None = None,
        banner_max_height: int = 140,
        poster_height: int = 160,
        corner_radius: int = 11,
        compact_mode: bool = True,
        dismiss_timeout: int = 5000,
        parent=None
    ):
        super().__init__(parent)
        self.title_text = title
        self.season_num = season_num
        self.episode_num = episode_num
        self.poster_bytes = poster
        self.banner_bytes = banner
        self.handler = handler
        self.offset_top = offset_top
        self.offset_right = offset_right
        self.fixed_width = fixed_width
        self.width_factor = width_factor
        self.banner_max_height = banner_max_height
        self.poster_height = poster_height
        self.corner_radius = corner_radius
        self.compact_mode = compact_mode
        self.hovered = False
        self.timer_expired = False
        self.main_container: Optional[QWidget] = None
        self.close_btn: Optional[QPushButton] = None
        self._configure_window()
        self._build_ui()
        if self.main_container:
            self.main_container.installEventFilter(self)
            self._apply_container_style()
        self.adjustSize()
        QTimer.singleShot(dismiss_timeout, self._on_timeout)

    def eventFilter(self, watched, event) -> bool:
        if watched is self.main_container:
            if event.type() == QEvent.Enter:
                self.hovered = True
                # A new hover event has occurred, so cancel any pending restack.
                self.handler.cancel_pending_restack()
                self.raise_()
            elif event.type() == QEvent.Leave:
                self.hovered = False
                # Schedule a restack to restore order after the mouse leaves.
                self.handler.restack_notifications()
                if self.timer_expired:
                    self.close()
        return super().eventFilter(watched, event)

    def _configure_window(self) -> None:
        self.setWindowFlags(Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint | Qt.Tool)
        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setAttribute(Qt.WA_ShowWithoutActivating)
        self.setModal(False)
        self.setContentsMargins(0, 0, 0, 0)

    def _compute_width(self) -> int:
        if self.width_factor:
            screen = QApplication.primaryScreen()
            if screen:
                geom = screen.availableGeometry()
                return int(geom.width() * self.width_factor)
        return self.fixed_width

    def _build_ui(self) -> None:
        width = self._compute_width()
        self.setFixedWidth(int(width * 0.9))
        outer = QVBoxLayout(self)
        outer.setContentsMargins(0, 0, 0, 0)
        outer.setSpacing(0)
        self.main_container = QWidget(self)
        self.main_container.setObjectName("mainContainer")
        outer.addWidget(self.main_container)
        main_layout = QVBoxLayout(self.main_container)
        main_layout.setContentsMargins(0, 0, 12, 12)
        main_layout.setSpacing(0)
        if self.banner_bytes:
            banner_label = QLabel()
            label_width = int(width * 0.9)
            banner_label.setFixedWidth(label_width)
            banner_height = int(self.banner_max_height * 0.9)
            banner_label.setFixedHeight(banner_height)
            banner_pix = self._prepare_banner(label_width, banner_height)
            if banner_pix:
                banner_label.setPixmap(banner_pix)
            banner_label.setStyleSheet("background: transparent; border: none;")
            main_layout.addWidget(banner_label)
        content_layout = QHBoxLayout()
        content_layout.setContentsMargins(14, 14, 14, 8)
        content_layout.setSpacing(14)
        if self.poster_bytes:
            poster_label = QLabel()
            poster_w = int(width * 0.3)
            poster_h = int(self.poster_height * 0.9)
            poster_label.setFixedSize(poster_w, poster_h)
            poster_pix = self._prepare_poster(poster_w, poster_h)
            if poster_pix:
                poster_label.setPixmap(poster_pix)
            poster_label.setStyleSheet("background: transparent; border: none;")
            content_layout.addWidget(poster_label, alignment=Qt.AlignTop)
        text_layout = QVBoxLayout()
        text_layout.setContentsMargins(0, 0, 0, 0)
        text_layout.setSpacing(4 if self.compact_mode else 6)
        title_label = QLabel(self.title_text)
        title_fs = 17 if self.compact_mode else 19
        title_label.setStyleSheet(f"color: #111; font-weight: 600; font-size: {title_fs}px; background: transparent;")
        title_label.setWordWrap(True)
        body_label = QLabel(f"Season {self.season_num} Episode {self.episode_num} airs today!")
        body_fs = 13 if self.compact_mode else 14
        body_label.setStyleSheet(f"color: #222; font-size: {body_fs}px; background: transparent;")
        body_label.setWordWrap(True)
        text_layout.addWidget(title_label)
        text_layout.addWidget(body_label)
        text_layout.addStretch()
        content_layout.addLayout(text_layout)
        main_layout.addLayout(content_layout)
        self._build_close_button()

    def _build_close_button(self) -> None:
        self.close_btn = QPushButton("✕", self.main_container)
        self.close_btn.setObjectName("closeButton")
        self.close_btn.setFixedSize(26, 26)
        self.close_btn.setCursor(Qt.PointingHandCursor)
        self.close_btn.clicked.connect(self.close)
        self.close_btn.setStyleSheet("""
            #closeButton { background-color: rgba(0, 0, 0, 0.4); color: white; border: none; border-radius: 13px; font-weight: bold; font-size: 14px; }
            #closeButton:hover { background-color: rgba(255, 0, 0, 0.8); color: white; }
        """)
        if self.main_container:
            self.close_btn.move(self.main_container.width() - self.close_btn.width() - 2, 2)
        self.close_btn.raise_()
    def _apply_container_style(self) -> None:
        gradient = "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #ffffff, stop:1 #ececec);"
        if self.main_container:
            self.main_container.setStyleSheet(f"#mainContainer {{ {gradient} border-radius: 12px; }}")
    def _on_timeout(self) -> None:
        self.timer_expired = True
        if not self.hovered:
            self.close()
    def _prepare_banner(self, target_w: int, target_h: int) -> Optional[QPixmap]:
        if not self.banner_bytes: return None
        img = QImage.fromData(self.banner_bytes)
        if img.isNull(): return None
        return self._rounded_pixmap(img, (target_w, target_h), (self.corner_radius, self.corner_radius, 0, 0))
    def _prepare_poster(self, target_w: int, target_h: int) -> Optional[QPixmap]:
        if not self.poster_bytes: return None
        img = QImage.fromData(self.poster_bytes)
        if img.isNull(): return None
        return self._rounded_pixmap(img, (target_w, target_h), (8, 8, 8, 8))
    def _rounded_pixmap(self, img: QImage, size: Tuple[int, int], corner_radii: Tuple[int, int, int, int]) -> QPixmap:
        w_target, h_target = size
        img = img.scaled(w_target, h_target, Qt.IgnoreAspectRatio, Qt.SmoothTransformation)
        rounded = QPixmap(w_target, h_target)
        rounded.fill(Qt.transparent)
        painter = QPainter(rounded)
        painter.setRenderHint(QPainter.Antialiasing, True)
        path = QPainterPath()
        tl, tr, bl, br = corner_radii
        path.moveTo(tl, 0)
        path.lineTo(w_target - tr, 0)
        path.quadTo(w_target, 0, w_target, tr)
        path.lineTo(w_target, h_target - br)
        path.quadTo(w_target, h_target, w_target - br, h_target)
        path.lineTo(bl, h_target)
        path.quadTo(0, h_target, 0, h_target - bl)
        path.lineTo(0, tl)
        path.quadTo(0, 0, tl, 0)
        painter.setClipPath(path)
        painter.drawImage(0, 0, img)
        painter.end()
        return rounded

    def position_from_top_right(self):
        screen = QApplication.primaryScreen()
        if screen:
            geom = screen.availableGeometry()
            x = geom.x() + geom.width() - self.width() - self.offset_right
            y = geom.y() + self.offset_top
            self.move(x, y)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        if self.close_btn:
            offset = 4
            self.close_btn.move(self.width() - self.close_btn.width() - offset, offset)

    def showEvent(self, event):
        super().showEvent(event)
        self.position_from_top_right()

    def show_notification(self):
        self.adjustSize()
        self.position_from_top_right()
        self.show()
        self.raise_()
