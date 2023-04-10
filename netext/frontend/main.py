from turtle import back
from urllib import request
from PyQt5.QtWidgets import QApplication, QTextEdit
from diff_match_patch import diff_match_patch
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QFont
from socket import *
import threading

import json
import time

DIFF_INSERT = 1
DIFF_REMOVE = -1
TIMER_MILLISECONDS = 1000 * 3

class Code:
    # requests
    CREATE_SESSION_REQUEST = 100
    JOIN_SESSION_REQUEST = 101
    DELETE_SESSION_REQUEST = 102
    FILE_INSERT_REQUEST = 103
    FILE_REMOVE_REQUEST = 104
    FRONTEND_SESSION_REQUEST = 105
    CLIENT_LEAVE_REQUEST = 106

    # responses
    ERROR_RESPONSE = 200
    CREATE_SESSION_RESPONSE = 201
    JOIN_SESSION_RESPONSE = 202
    DELETE_SESSION_RESPONSE = 203
    PEER_INFO_RESPONSE = 204
    FILE_INSERT_RESPONSE = 205
    FILE_REMOVE_RESPONSE = 206

AUTO_SRC_PORT = 0
LOCAL_HOST = "127.0.0.1"
serverPort = 0



backendSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def getPortFromFile():
    """
        gets the backend port from the file the backend created
    """
    with open("../client/port.txt" , "r") as portFile:
        strPort = portFile.read()
        print(strPort)
        return int(strPort)

class ContentHandler:

    def __init__(self):
        self._content = ""
    
    def read_content(self):
        return self._content
    
    def get_content_size(self):
        return len(self._content)
    
    def valid_position(self, position):
        return position <= self.get_content_size() and position >= 0
    
    def valid_remove_amount(self, position, amount):
        return amount > 0 and amount <= self.get_content_size() - position + 1
    
    def insert(self, position, content):
        if not self.valid_position(position):
            raise ValueError("Position is not valid")
        self._content = self._content[:position] + content + self._content[position:]
    
    def remove(self, position, remove_amount):
        if not self.valid_position(position):
            raise ValueError("Position is not valid")
        if not self.valid_remove_amount(position, remove_amount):
            raise ValueError("Remove amount is not valid")
        self._content = self._content[:position] + self._content[position + remove_amount:]

    def handleRequests(self):
        while True:
            msg , addr = backendSock.recvfrom(1024)
            print("Message Received: " + msg.decode())
            jsonMsg = json.loads(msg.decode())
            requestCode = jsonMsg["code"]
            requestData = jsonMsg["data"]

            if requestCode == Code.FILE_INSERT_REQUEST:
                self.insert(requestData["position"] , requestData["content"])

            elif requestCode == Code.FILE_REMOVE_REQUEST:
                self.remove(requestData["position"] , requestData["amount"])


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
            op = Code.FILE_INSERT_REQUEST
            log_data = {"position":pos , "content": content}
        elif op == DIFF_REMOVE:
            op = Code.FILE_REMOVE_REQUEST
            log_data = {"position":pos , "amount": len(content)}
        else:
            raise ValueError("Error: Invalid operation type.")

        log_obj = {"code": op , "time": timestamp, "data": log_data}
        return json.dumps(log_obj)


    def notify(self,packet):
        """
        Sends a notification packet to the backend.
        """
        print(packet)
        print(backendSock.sendto(packet.encode() , (LOCAL_HOST , serverPort)))


if __name__ == "__main__":
    
    contentHandler = ContentHandler()
    serverPort = getPortFromFile()
    backendSock.bind((LOCAL_HOST , AUTO_SRC_PORT))

    timestamp = int(time.time())
    obj = {"code": Code.FRONTEND_SESSION_REQUEST , "time": timestamp, "data": "ready"}
    
    backendSock.sendto(json.dumps(obj).encode() , (LOCAL_HOST , serverPort))

    requestsThread = threading.Thread(target=contentHandler.handleRequests)

    app = QApplication([])

    textedit = TextEdit()
    textedit.setGeometry(100, 100, 400, 200)
    textedit.show()

    app.exec_()
