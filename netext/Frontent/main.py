from PyQt5.QtWidgets import QApplication, QTextEdit
from diff_match_patch import diff_match_patch
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QFont

import json
import time

DIFF_INSERT = 1
DIFF_REMOVE = -1
TIMER_MILLISECONDS = 1000 * 5

class TextEdit(QTextEdit):
    """
    Attributes:
    -----------
    last_text : str
        The last saved text in the widget.
    diff_tool : diff_match_patch object
        A diff_match_patch object to track the text differences.
    timer : QTimer object
        A QTimer object to trigger the diff_handler() function on a given rate.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.last_text = self.toPlainText()

        self.diff_tool = diff_match_patch()

        self.timer = QTimer()
        self.timer.timeout.connect(self.diff_handler)
        self.timer.start(TIMER_MILLISECONDS)

         # Set font and background color
        font = QFont("Courier")
        font.setPointSize(14)
        self.setFont(font)
        self.setStyleSheet("background-color: #f0f0f0; color: #000000;")

    
    def diff_handler(self):
        """
        Triggers the diff_match_patch library to find and notify the difference(s) between last_text and new_text.
        """
        new_text = self.toPlainText()
        diffs = self.diff_tool.diff_main(self.last_text, new_text)
        self.diff_tool.diff_cleanupSemantic(diffs)
        pos = 0
        for op, content in diffs:
            if op:
                self.notify(self.serializer((op,pos,content)))

            if op != DIFF_REMOVE:
                chunk_length = len(content)
                pos += chunk_length

        self.last_text = new_text

    def serializer(self,data):
        """
        Serializes the data for the notification packet.
        """
        op, pos, content = data
        timestamp = int(time.time())

        if op == DIFF_INSERT:
            log_data = {"op": op,"pos":pos , "data": content}
        elif op == DIFF_REMOVE:
            log_data = {"op": op,"pos":pos , "data": len(content)}
        else:
            raise ValueError("Error: Invalid operation type.")

        log_obj = {"timestamp": timestamp, "data": log_data}
        return json.dumps(log_obj)


    def notify(self,packet):
        """
        Sends a notification packet to the backend.
        """
        print(packet)

if __name__ == "__main__":
    app = QApplication([])
    textedit = TextEdit()

    textedit.setGeometry(100, 100, 400, 200)

    textedit.show()
    app.exec_()
