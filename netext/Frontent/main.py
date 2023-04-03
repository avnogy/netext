from PyQt5.QtWidgets import QApplication, QTextEdit
from diff_match_patch import diff_match_patch
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QFont
import socket


import json
import time

DIFF_INSERT = 1
DIFF_REMOVE = -1
TIMER_MILLISECONDS = 1000 * 5

FILE_INSERT_REQUEST = 103
FILE_REMOVE_REQUEST = 104

SERVER_ADDRESS = "127.0.0.1"
serverPort = 0

backendSock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)


def getPortFromFile():
    """
        gets the backend port from the file the backend created
    """
    with open("port.txt" , "r") as portFile:
        strPort = portFile.read()
        print(strPort)
        serverPort = int(strPort)

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
            op = FILE_INSERT_REQUEST
            log_data = {"position":pos , "content": content}
        elif op == DIFF_REMOVE:
            op = FILE_REMOVE_REQUEST
            log_data = {"position":pos , "amount": len(content)}
        else:
            raise ValueError("Error: Invalid operation type.")

        log_obj = {"requestCode": op , "timeStamp": timestamp, "data": log_data}
        return json.dumps(log_obj)


    def notify(self,packet):
        """
        Sends a notification packet to the backend.
        """
        print(packet)

if __name__ == "__main__":
    
    getPortFromFile()

    app = QApplication([])

    textedit = TextEdit()
    textedit.setGeometry(100, 100, 400, 200)
    textedit.show()

    app.exec_()
