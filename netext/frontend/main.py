import sys
from window import *
from content import ContentHandler

if __name__ == "__main__":

    network.greet_backend()

    app = QApplication(sys.argv)
    win = Window()

    contentHandler = ContentHandler()
    requestsThread = threading.Thread(target=contentHandler.handleRequests)
    requestsThread.start()

    win.show()
    app.exec_()
    network.leave_backend()

    requestsThread.join()
    sys.exit()

