from utils import Code, json
import network
from PyQt5.QtCore import QThread, pyqtSignal


class ContentHandler(QThread):
    packet_received = pyqtSignal(dict)

    def __init__(self, parent=None):
        super(ContentHandler, self).__init__(parent)

    def run(self):
        while True:
            try:
                msg = network.recv()
                if not msg:
                    return
                print("Message Received: " + msg.decode())
                jsonMsg = json.loads(msg.decode())
                if jsonMsg["code"] in [Code.FILE_INSERT_RESPONSE, Code.FILE_REMOVE_RESPONSE]:
                    self.packet_received.emit(jsonMsg)

            except Exception as e:
                print("Error while applying packet:\n", msg)
                print(e)
