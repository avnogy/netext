import sys
from window import *
from content import ContentHandler

if __name__ == "__main__":

    network.greet_backend()

    app = QApplication(sys.argv)
    win = Window()

    win.show()
    app.exec_()
    network.leave_backend()

    sys.exit()
