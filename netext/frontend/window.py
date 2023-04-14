from PyQt5.QtWidgets import QApplication, QTextEdit,QWidget,QVBoxLayout,QPushButton,QMainWindow,QLabel,QMenuBar,QMenu,QAction
from diff_match_patch import diff_match_patch
from PyQt5.QtCore import QTimer, Qt, QObject,pyqtSignals
from PyQt5.QtGui import QFont,QKeySequence

from utils import *
import network
import content



class Window(QMainWindow):
    """Main Window."""

    def _createActions(self):
        # Creating action using the first constructor
        self.newAction = QAction(self)
        self.newAction.setText("&New")
        self.newAction.setShortcut("Ctrl+N")
        # Creating actions using the second constructor
        self.openAction = QAction("&Open...", self)
        self.openAction.setShortcut("Ctrl+O")
        self.saveAction = QAction("&Save", self)
        self.saveAction.setShortcut("Ctrl+S")
        self.exitAction = QAction("&Exit", self)
        self.copyAction = QAction("&Copy", self)
        self.copyAction.setShortcut(QKeySequence.Copy)
        self.pasteAction = QAction("&Paste", self)
        self.pasteAction.setShortcut(QKeySequence.Paste)
        self.cutAction = QAction("C&ut", self)
        self.cutAction.setShortcut(QKeySequence.Cut)

        # Edit actions
        # Snip...
        # Using standard keys

    def _connectActions(self):
        # Connect File actions
        self.newAction.triggered.connect(self.newFile)
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.exitAction.triggered.connect(self.close)
        # Connect Edit actions
        self.copyAction.triggered.connect(self.copyContent)
        self.pasteAction.triggered.connect(self.pasteContent)
        self.cutAction.triggered.connect(self.cutContent)


    def _createMenuBar(self):
        menuBar = QMenuBar(self)
        self.setMenuBar(menuBar)

        # File menu
        fileMenu = QMenu("&File", self)
        menuBar.addMenu(fileMenu)
        fileMenu.addAction(self.newAction)
        fileMenu.addAction(self.openAction)
        fileMenu.addAction(self.saveAction)
        fileMenu.addSeparator()
        fileMenu.addAction(self.exitAction)
        
        # Edit menu
        editMenu = menuBar.addMenu("&Edit")
        editMenu.addAction(self.copyAction)
        editMenu.addAction(self.pasteAction)
        editMenu.addAction(self.cutAction)

   
    def newFile(self):
        self.centralwidget.settext("new clicked")

    def openFile(self):
        self.update_content("Open clicked")

    def saveFile(self):
        self.update_content("Save clicked")

    def copyContent(self):
        self.update_content("Copy clicked")

    def pasteContent(self):
        self.update_content("Paste clicked")

    def cutContent(self):
        self.update_content("Cut clicked")
    
    def update_content(self,text):
        self.centralWidget.setText(text)

    def leave(self):
        network.leave_backend()

    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)
        
        self.setWindowTitle("netext")
        self.resize(400, 200)

        self.centralWidget = TextEdit()
        #self.centralWidget.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        self.setCentralWidget(self.centralWidget)
        self._createActions()
        self._connectActions()
        self._createMenuBar()



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
                self.notify(self.compile((op,pos,content)))

            if op != DIFF_REMOVE:
                chunk_length = len(content)
                pos += chunk_length

        self.last_text = new_text


    def compile(self,data):
        """
        Serializes the data for the notification packet.
        """
        op, pos, content = data

        if op == DIFF_INSERT:
            op = Code.FILE_INSERT_REQUEST
            log_data = {"position":pos , "content": content}
        elif op == DIFF_REMOVE:
            op = Code.FILE_REMOVE_REQUEST
            log_data = {"position":pos , "amount": len(content)}
        else:
            raise ValueError("Error: Invalid operation type.")

        
        return serialize(op,log_data)
 


    def notify(self,packet):
        """
        Sends a notification packet to the backend.
        """
        print(packet)
        network.send(packet)

