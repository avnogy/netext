import sys
from PyQt5.QtWidgets import QApplication, QTextEdit, QWidget, QVBoxLayout, QPushButton, QMainWindow, QLabel, QMenuBar, QMenu, QAction
from PyQt5.QtCore import QTimer, Qt, QObject, pyqtSlot, pyqtSignal, QMutex, QMutexLocker
from diff_match_patch import diff_match_patch
from PyQt5.QtGui import QFont, QKeySequence, QTextCursor

from utils import *
import network
import content
import timeit


class Window(QMainWindow):
    """Main Window."""

    def leave(self):
        network.leave_backend()

    def __init__(self, parent=None):
        """Initializer."""
        super().__init__(parent)

        self.setWindowTitle("netext")
        self.resize(400, 200)

        self.centralWidget = TextEdit()
        self.setCentralWidget(self.centralWidget)
        self._createActions()
        self._connectActions()
        self._createMenuBar()

        self.text_updater_thread = content.ContentHandler()
        self.text_updater_thread.packet_received.connect(
            self.centralWidget.handle_packet)  # type: ignore
        self.text_updater_thread.start()

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

    def _connectActions(self):
        # Connect File actions
        self.newAction.triggered.connect(self.newFile)
        self.openAction.triggered.connect(self.openFile)
        self.saveAction.triggered.connect(self.saveFile)
        self.exitAction.triggered.connect(self.close)  # type: ignore
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
        self.centralwidget.setText("new clicked")

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

    def update_content(self, text):
        self.centralWidget.setText(text)


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
        self.mutex = QMutex()

        font = QFont("Courier")
        font.setPointSize(14)
        self.setFont(font)
        self.setStyleSheet("background-color: #f0f0f0; color: #000000;")

    def diff_handler(self):
        # tic = time.perf_counter()

        self.mutex.lock()

        """
        Triggers the diff_match_patch library to find and notify the difference(s) between last_text and new_text.
        """
        new_text = self.toPlainText()
        diffs = self.diff_tool.diff_main(self.last_text, new_text)
        # self.diff_tool.diff_cleanupSemantic(diffs)
        pos = 0
        for op, content in diffs:
            if op:
                self.notify(self.compile((op, pos, content)))

            if op != DIFF_REMOVE:
                chunk_length = len(content)
                pos += chunk_length
        self.mutex.unlock()

        # toc = time.perf_counter()
        # print(f"timed diff at {toc - tic:0.4f} seconds")

    def compile(self, data):
        """
        Serializes the data for the notification packet.
        """
        op, pos, content = data

        if op == DIFF_INSERT:
            op = Code.FILE_INSERT_REQUEST
            log_data = {"position": pos, "content": content}

        elif op == DIFF_REMOVE:
            op = Code.FILE_REMOVE_REQUEST
            log_data = {"position": pos, "amount": len(content)}
        else:
            raise ValueError("Error: Invalid operation type.")
        return serialize(op, log_data)
    
    def keyPressEvent(self, event):
        self.mutex.lock()
        super().keyPressEvent(event)
        self.mutex.unlock()

    @pyqtSlot(dict)
    def handle_packet(self, packet):
        tic = time.perf_counter()
        self.mutex.lock()
        try:
            print("last_text: ", self.last_text)
            position = packet["data"]["position"]
            remote_change =  packet["data"]["endpoint"] != network.frontend_endpoint

            if not self.valid_position(position):
                raise ValueError("Position is not valid")

            cursor_pos = self.textCursor().position()

            if packet["code"] == Code.FILE_INSERT_RESPONSE:
                insert_text = packet["data"]["content"]

                self.last_text = self.last_text[:position] + \
                    insert_text + self.last_text[position:]
                self.setText(self.last_text)

                if remote_change and position <= cursor_pos:  # and self.last_direction > 0:
                    cursor_pos += len(insert_text)
                    print("insert", cursor_pos)
                else:
                    print("didnt compensatei")

            elif packet["code"] == Code.FILE_REMOVE_RESPONSE:
                amount = packet["data"]["amount"]

                if not self.valid_remove_amount(position, amount):
                    raise ValueError("Remove amount is not valid")

                self.last_text = self.last_text[:position] + \
                    self.last_text[position+amount:]
                self.setText(self.last_text)


                # print(cursor_pos, position ,amount, self.last_direction,cursor_pos >= position + amount -1 , self.last_direction < 0)
                if remote_change and cursor_pos >= position + amount - 1:  # and self.last_direction < 0:
                    cursor_pos -= amount
                    print("removed", cursor_pos)
                else:
                    print("didnt compensater")

                    
            self.textChanged.emit()

            if cursor_pos > len(self.toPlainText()):
                cursor_pos = len(self.toPlainText())
            
            cursor = self.textCursor()
            cursor.setPosition(cursor_pos)
            self.setTextCursor(cursor)
            self.cursorPositionChanged.emit()


        except Exception as e:
            print("Error while applying packet:\n", packet)
            print(e)
        self.mutex.unlock()

        
        # toc = time.perf_counter()
        # print(f"timed handler at {toc - tic:0.4f} seconds")

    def notify(self, packet):
        """
        Sends a notification packet to the backend.
        """
        print(packet)
        network.send(packet)

    def content_size(self):
        return len(self.last_text)

    def valid_position(self, position):
        return position <= self.content_size() and position >= 0

    def valid_remove_amount(self, position, amount):
        return amount > 0 and amount <= self.content_size() - position + 1
