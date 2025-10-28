import gzip
import logging
import os
import sys
from datetime import datetime
from logging.handlers import RotatingFileHandler

from PySide6.QtCore import QObject, QTimer

from core.notifications.notification_dialog import Notification
from models.NotificationDB import NotificationDatabaseHandler
from utils.anilex_helpers_v2 import get_cache_dir


def load_settings():
    from old.anilex_helper import load_settings
    settings = load_settings()
    notification_settings = settings.get('THEME', {}).get('notifications')
    return notification_settings if notification_settings else {}

class NotificationHandler(QObject):
    """
    Manages scheduling, display, and repositioning of stacking notifications.
    This version correctly repositions notifications when one is closed.
    """

    def __init__(self, parent=None):
        super().__init__(parent)
        self.db_handler = NotificationDatabaseHandler()
        self.shown_notifications = []
        self.settings = load_settings()
        self.base_offset_top = self.settings.get('offset_top')
        self.base_offset_right = self.settings.get('offset_right')
        self.cascade_offset = self.settings.get('cascade_offset')
        self.restack_timer = QTimer(self)
        self.restack_timer.setSingleShot(True)
        self.restack_timer.timeout.connect(self._perform_restack)
        self.active_timers = {}


        log_path = os.path.join(get_cache_dir(), 'logs', 'notification_handler.log')
        if not os.path.exists(log_path):
            with open(log_path, 'w'):
                pass
        archive_path = log_path + '.gz'

        if os.path.getsize(log_path) >= 10_000_000:
            if os.path.exists(archive_path):
                os.remove(archive_path)

            with open(log_path, 'rb') as f_in:
                with gzip.open(archive_path, 'wb') as f_out:
                    f_out.writelines(f_in)

            open(log_path, 'w').close()

            with open(log_path, 'a') as log_file:
                log_file.write(f"Log file was archived at {datetime.now().isoformat()}\n")

        logging.basicConfig(
            level=logging.DEBUG,
            format='%(asctime)s - %(levelname)s - %(message)s',
            handlers=[
                RotatingFileHandler(log_path , maxBytes=1_000_000, backupCount=0),
                logging.StreamHandler(sys.stdout)
            ]
        )
        self.logger = logging.getLogger(__name__)

        self.schedule_notifications_for_today()

    def schedule_notifications_for_today(self):
        todays_notifications = self.db_handler.get_todays_notifications()
        now = datetime.now()
        if not todays_notifications:
            return

        self.logger.info(f"Found {len(todays_notifications)} notifications for today. Scheduling them now...")
        for notification_data in todays_notifications:
            _id, title, season, episode, scheduled_time_str, date, poster, banner = notification_data
            try:
                notification_datetime = datetime.strptime(f"{date} {scheduled_time_str}", '%Y-%m-%d %H:%M')
            except ValueError:
                notification_datetime = datetime.strptime(f"{date} {scheduled_time_str}", '%Y-%m-%d %H:%M:%S')
            delay_ms = (notification_datetime - now).total_seconds() * 1000
            effective_delay = max(1, int(delay_ms))
            QTimer.singleShot(effective_delay, lambda data=notification_data: self.create_notification(data))
            self.active_timers[_id] = notification_datetime

    def create_notification(self, notification_data):
        _id, title, season_number, episode_number, _, _, poster, banner = notification_data
        active_notifications = len([n for n in self.shown_notifications if n.isVisible()])
        vertical_offset = active_notifications * self.cascade_offset

        notification = Notification(
            title=title,
            season_num=season_number,
            episode_num=episode_number,
            poster=poster,
            banner=banner,
            handler=self,
            offset_top=self.base_offset_top + vertical_offset,
            offset_right=self.base_offset_right,
            fixed_width=self.settings.get('fixed_width', 380),
            width_factor=self.settings.get('width_factor', None),
            poster_height=self.settings.get('poster_height', 160),
            banner_max_height=self.settings.get('banner_max_height', 140),
            corner_radius=self.settings.get('corner_radius', 1),
            dismiss_timeout=self.settings.get('dismiss_timeout', 5000)
        )
        notification.finished.connect(lambda: self.on_notification_closed(notification))
        self.shown_notifications.append(notification)
        notification.show_notification()

    def restack_notifications(self):
        """Schedules a Z-order restack operation for hover events."""
        self.restack_timer.start(50)

    def cancel_pending_restack(self):
        """Cancels any scheduled Z-order restack operation."""
        if self.restack_timer.isActive():
            self.restack_timer.stop()

    def _perform_restack(self):
        """The actual Z-order restacking logic for hover."""
        visible_notifications = [n for n in self.shown_notifications if n.isVisible()]
        for notif in visible_notifications:
            notif.raise_()

    def on_notification_closed(self, notification):
        """
        Called when a notification is closed. Triggers a repositioning
        of all remaining notifications.
        """
        QTimer.singleShot(10, self._reposition_all_notifications)

    def _reposition_all_notifications(self):
        """
        Updates the on-screen position of all remaining visible notifications
        to fill the gap left by a closed one.
        """
        # Update list
        self.shown_notifications = [notif for notif in self.shown_notifications if notif.isVisible()]

        # Iterate over all notification to update their positions
        for i, notif in enumerate(self.shown_notifications):
            new_vertical_offset = i * self.cascade_offset
            notif.offset_top = self.base_offset_top + new_vertical_offset
            notif.position_from_top_right()

    def update_scheduled_notification(self) -> None:
        """
        Updates reschedules it if needed.
        :return: None
        """

        self.logger.info("Updating scheduled notifications for today...")
        for timer in self.active_timers.values():
            try:
                timer.stop()
            except Exception as e:
                self.logger.error(f"Error stopping timer: {e}")

        self.active_timers.clear()

        self.logger.info("Cleared all existing timers. Rescheduling now...")
        try:
            self.schedule_notifications_for_today()
        except Exception as e:
            self.logger.error(f"Error while rescheduling notifications: {e}")

    def clear_all_notifications(self) -> None:
        """
        Closes all currently visible notifications and clears any pending timers.
        :return: None
        """
        self.logger.info("Clearing all notifications and timers...")
        for notif in self.shown_notifications:
            try:
                notif.close()
            except Exception as e:
                self.logger.error(f"Error closing notification: {e}")

        self.shown_notifications.clear()

        for timer in self.active_timers.values():
            try:
                timer.stop()
            except Exception as e:
                self.logger.error(f"Error stopping timer: {e}")

        self.active_timers.clear()
        self.logger.info("All notifications and timers cleared.")

    def stop_scheduled_notifications(self, _id: int) -> None:
        """
        Stops scheduling any further notifications for today.
        :return: None
        """
        self.logger.info(f"Stopping scheduled notifications for today with ID {_id}...")
        if _id in self.active_timers:
            try:
                self.active_timers[_id].stop()
                del self.active_timers[_id]
                self.logger.info(f"Stopped and removed timer for ID {_id}.")
            except Exception as e:
                self.logger.error(f"Error stopping timer for ID {_id}: {e}")
        else:
            self.logger.warning(f"No active timer found for ID {_id}.")
