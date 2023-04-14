from utils import Code,json
import network

class ContentHandler:

    def __init__(self):
        self._content = ""

    def content_size(self):
        return len(self._content)

    def valid_position(self, position):
        return position <= self.content_size() and position >= 0
   
    def valid_remove_amount(self, position, amount):
        return amount > 0 and amount <= self.content_size() - position + 1
    
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
            msg = network.recv()
            if not msg: 
                return
            print("Message Received: " + msg.decode())
            jsonMsg = json.loads(msg.decode())
            requestCode = jsonMsg["code"]
            requestData = jsonMsg["data"]

            try:
                if requestCode == Code.FILE_INSERT_RESPONSE:
                    self.insert(requestData["position"] , requestData["content"])
                elif requestCode == Code.FILE_REMOVE_RESPONSE:
                    self.remove(requestData["position"] , requestData["amount"])
                win.centralWidget.setText(self._content)
            except Exception as e:
                print("Error while applying packet:\n",msg)
                print (e)
            print(self._content)

