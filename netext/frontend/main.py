import sys
from window import *


if __name__ == "__main__":
    app = QApplication(sys.argv)
    win = Window()

    win.show()
    sys.exit(app.exec_())

