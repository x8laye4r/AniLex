from PySide6.QtWidgets import QLabel
from PySide6.QtCore import Qt, QRect
from PySide6.QtGui import QPainter, QPainterPath, QPen


class OutlinedLabel(QLabel):
    """
    A QLabel subclass that draws text with an outline.
    Attributes:
        :parameter text: The text to display. Defaults to an empty string.
        :parameter outline: The width of the outline. Defaults to 8.
        :parameter color: The color of the outline. Defaults to black.
        :parameter parent: The parent widget.
    """
    def __init__(self, text="", outline=8, color = Qt.black):
        super().__init__(text)
        self.outline_color = color
        self.outline_width = outline
        self.setWordWrap(True)

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        font_metrics = self.fontMetrics()
        pen = QPen(self.outline_color, self.outline_width)
        pen.setJoinStyle(Qt.RoundJoin)

        path = QPainterPath()

        if self.wordWrap():
            text = self.text()
            rect = self.rect()

            text_rect = rect.adjusted(4, 0, -4, 0)

            line_spacing = font_metrics.lineSpacing()
            y = font_metrics.ascent()

            current_pos = 0
            while current_pos < len(text):
                line_text = ""
                line_width = 0

                words = text[current_pos:].split()
                if not words:
                    break

                for i, word in enumerate(words):
                    next_word_width = font_metrics.horizontalAdvance(word + " ")
                    if line_width + next_word_width > text_rect.width() and line_text:
                        break

                    line_text += word + " "
                    line_width += next_word_width

                    if i == len(words) - 1:
                        break

                line_text = line_text.strip()

                if not line_text:
                    for i in range(len(text) - current_pos):
                        char = text[current_pos + i]
                        if font_metrics.horizontalAdvance(line_text + char) > text_rect.width():
                            break
                        line_text += char

                    if not line_text:
                        line_text = text[current_pos]

                    current_pos += len(line_text)
                else:
                    original_len_of_line = len(line_text)
                    current_pos = text.find(line_text, current_pos) + len(line_text)
                    while current_pos < len(text) and text[current_pos].isspace():
                        current_pos += 1

                path.addText(text_rect.left(), y, self.font(), line_text)
                y += line_spacing

        else:
            path.addText(4, font_metrics.ascent(), self.font(), self.text())

        painter.setPen(pen)
        painter.setBrush(Qt.NoBrush)
        painter.drawPath(path)

        painter.setPen(Qt.NoPen)
        painter.setBrush(self.palette().windowText())
        painter.drawPath(path)
