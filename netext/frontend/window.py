import sys
from PyQt5.QtWidgets import QApplication, QTextEdit, QWidget, QVBoxLayout, QPushButton, QMainWindow, QLabel, QMenuBar, QMenu, QAction, QFileDialog, QMessageBox
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
        fileMenu.addSeparator()
        fileMenu.addAction(self.exitAction)

        # Edit menu
        editMenu = menuBar.addMenu("&Edit")
        editMenu.addAction(self.copyAction)
        editMenu.addAction(self.pasteAction)
        editMenu.addAction(self.cutAction)

    def newFile(self):
        if not self.centralWidget.toPlainText():
            self.current_file = ""
            self.centralWidget.clear()
        else:
            msg_box = QMessageBox(self)
            msg_box.setIcon(QMessageBox.Warning)
            msg_box.setText("Do you want to discard unsaved changes and create a new file?")
            msg_box.setWindowTitle("Confirm New File")
            msg_box.setStandardButtons(QMessageBox.Yes | QMessageBox.No)
            button_yes = msg_box.button(QMessageBox.Yes)
            button_yes.setText("Yes, discard changes")
            button_no = msg_box.button(QMessageBox.No)
            button_no.setText("No, cancel")
            msg_box.setDefaultButton(button_no)
            msg_box.exec_()
            if msg_box.clickedButton() == button_yes:
                self.centralWidget.clear()
            else:
                return

    def openFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        fileName, _ = QFileDialog.getOpenFileName(
            self, "Open File", "", "All Files (*);;Text Files (*.txt)", options=options)
        if fileName:
            with open(fileName, "r") as f:
                file_content = f.read()
            #  network.send(serialize(Code.FILE_OPEN_REQUEST , {"path" : fileName}))
            if len(file_content) > BUFFER_SIZE:
                QMessageBox.warning(self, "File Too Large",
                                    "The file is too large. Please try another file.")
            else:
                self.centralWidget.setText(file_content)

    def copyContent(self):
        self.centralWidget.copy()

    def pasteContent(self):
        self.centralWidget.paste()

    def cutContent(self):
        self.centralWidget.cut()



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
        """
        Triggers the diff_match_patch library to find and notify the difference(s) between last_text and new_text.
        """
        self.mutex.lock()
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
        self.mutex.lock()
        try:
            new_text = self.toPlainText()

            remote_change = packet["data"]["endpoint"] != network.frontend_endpoint
            cursor_pos = self.textCursor().position()

            position = packet["data"]["position"]
            if not self.valid_position(position):
                raise ValueError("Position is not valid")

            if packet["code"] == Code.FILE_INSERT_RESPONSE:
                insert_text = packet["data"]["content"]

                # update last text
                self.last_text = self.last_text[:position] + \
                    insert_text + self.last_text[position:]

                if remote_change:
                    # update new text
                    new_text = new_text[:position] + \
                        insert_text + new_text[position:]

                    # update cursor position
                    if position <= cursor_pos:
                        cursor_pos += len(insert_text)

            elif packet["code"] == Code.FILE_REMOVE_RESPONSE:
                amount = packet["data"]["amount"]

                if not self.valid_remove_amount(position, amount):
                    raise ValueError("Remove amount is not valid")

                # update last text
                self.last_text = self.last_text[:position] + \
                    self.last_text[position+amount:]

                if remote_change:
                    # update new text
                    new_text = new_text[:position] + \
                        new_text[position+amount:]

                    # update cursor position
                    if cursor_pos >= position + amount - 1:
                        cursor_pos -= amount
            
            elif packet["code"] == Code.FILE_OPEN_RESPONSE:
                new_text = packet["data"]["content"]

            self.setText(new_text)
            self.textChanged.emit()

            # restore cursor position
            cursor = self.textCursor()
            cursor.setPosition(cursor_pos)
            self.setTextCursor(cursor)
            self.cursorPositionChanged.emit()

        except Exception as e:
            print("Error while applying packet:\n", packet)
            print(e)
        self.mutex.unlock()

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
