from PySide6.QtCore import QTimer, QPropertyAnimation, Signal, Qt, QEasingCurve
from PySide6.QtWidgets import QFrame, QVBoxLayout

from .vertical_tab_button import VerticalTabButton


class AutoHideVerticalTabs(QFrame):
    """
    A vertical tab bar that automatically collapses when the mouse leaves,
    and expands when the mouse hovers over it.

    Usage:
    tabs = AutoHideVerticalTabs()
    tabs.setup_tabs(["Dashboard", "Profile", "Settings", "Help"])
    tabs.currentChanged.connect(on_tab_changed)
    layout.addWidget(tabs)
    """
    currentChanged = Signal(int)

    def __init__(self, parent=None,
                 expanded_width=150,
                 collapsed_width=64,
                 collapse_delay=1500,
                 animation_duration=300,
                 background_color="#2c3e50",
                 font_size=11):
        """
        Initialize the auto-hiding vertical tab panel.

        Args:
            parent: The parent widget
            expanded_width: Width when expanded (pixels)
            collapsed_width: Width when collapsed (pixels)
            collapse_delay: Time to wait before collapsing (milliseconds)
            animation_duration: Duration of expand/collapse animation (milliseconds)
            background_color: Background color of the tab panel
            font_size: Base font size for tab buttons
        """
        super().__init__(parent)
        self.expanded_width = expanded_width
        self.collapsed_width = collapsed_width
        self.animation_duration = animation_duration
        self.font_size = font_size

        # Start with the collapsed width
        self.setMinimumWidth(self.collapsed_width)
        self.setMaximumWidth(self.collapsed_width)

        self.setStyleSheet(f"background-color: {background_color}; border: none;")

        self.layout = QVBoxLayout(self)
        self.layout.setContentsMargins(5, 5, 5, 5)
        self.layout.setSpacing(5)
        self.layout.setAlignment(Qt.AlignmentFlag.AlignTop)

        self.buttons = []
        self.tab_names = []

        # Timer for auto collapsing
        self.hide_timer = QTimer(self)
        self.hide_timer.setInterval(collapse_delay)
        self.hide_timer.setSingleShot(True)
        self.hide_timer.timeout.connect(self.minimize)

        # Flag to track current state
        self.is_expanded = False

    def setup_tabs(self, tab_names):
        """
        Set up the tabs using a list of tab names.

        Args:
            tab_names: List of strings for tab names
        """
        # Clear any existing tabs
        self.clear_tabs()

        # Add the new tabs
        for name in tab_names:
            self.add_tab(name)

    def clear_tabs(self):
        """Remove all tabs from the panel."""
        for button in self.buttons:
            self.layout.removeWidget(button)
            button.deleteLater()

        self.buttons = []
        self.tab_names = []

    def add_tab(self, name):
        """
        Add a single tab to the panel.

        Args:
            name: Name of the tab to add

        Returns:
            Index of the added tab
        """
        index = len(self.buttons)
        button = VerticalTabButton(name, base_font_pt=self.font_size)
        button.clicked.connect(lambda: self._on_button_clicked(index))

        self.layout.addWidget(button)
        self.buttons.append(button)
        self.tab_names.append(name)

        # Select the first tab by default
        if index == 0:
            button.setChecked(True)

        return index

    def _on_button_clicked(self, index):
        """Called when a tab button is clicked."""
        for i, btn in enumerate(self.buttons):
            btn.setChecked(i == index)
        self.currentChanged.emit(index)

    def enterEvent(self, event):
        """Expand the bar when mouse enters."""
        self.hide_timer.stop()
        self.expand()
        super().enterEvent(event)

    def leaveEvent(self, event):
        """Start the timer to minimize when mouse leaves."""
        self.hide_timer.start()
        super().leaveEvent(event)

    def minimize(self):
        """Minimize the bar and hide button text."""
        if self.width() == self.collapsed_width or not self.is_expanded:
            return
        for button in self.buttons:
            button.setShortText()
        self.animate_resize(self.collapsed_width)
        self.is_expanded = False

    def expand(self):
        """Expand the bar and show full button text."""
        if self.width() == self.expanded_width or self.is_expanded:
            return
        for button in self.buttons:
            button.setFullText()
        self.animate_resize(self.expanded_width)
        self.is_expanded = True

    def animate_resize(self, target_width):
        """Animate the width change of the bar."""

        # Animate both minimum and maximum width
        self.animation_min = QPropertyAnimation(self, b"minimumWidth")
        self.animation_min.setDuration(self.animation_duration)
        self.animation_min.setStartValue(self.width())
        self.animation_min.setEndValue(target_width)
        self.animation_min.setEasingCurve(QEasingCurve.InOutCubic)

        self.animation_max = QPropertyAnimation(self, b"maximumWidth")
        self.animation_max.setDuration(self.animation_duration)
        self.animation_max.setStartValue(self.width())
        self.animation_max.setEndValue(target_width)
        self.animation_max.setEasingCurve(QEasingCurve.InOutCubic)

        self.animation_min.start()
        self.animation_max.start()

    def set_current_index(self, index):
        """
        Set the current tab by index.

        Args:
            index: Index of the tab to select
        """
        if 0 <= index < len(self.buttons):
            self._on_button_clicked(index)

    def get_current_index(self):
        """Get the index of the currently selected tab."""
        for i, btn in enumerate(self.buttons):
            if btn.isChecked():
                return i
        return -1