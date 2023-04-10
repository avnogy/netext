import sys
from window import *
from content import ContentHandler

if __name__ == "__main__":

    network.greet_backend()
    contentHandler = ContentHandler()

    requestsThread = threading.Thread(target=contentHandler.handleRequests)
    requestsThread.start()
    app = QApplication(sys.argv)
    win = Window()

    win.show()
    sys.exit(app.exec_())

