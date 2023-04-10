import socket
import utils
from utils import Code,serialize

SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 0
SERVER_ENDPOINT = (SERVER_ADDRESS , SERVER_PORT)

backendSock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
serverPort = utils.getPortFromFile()

def greet_backend():
    backendSock.sendto(utils.serialize(Code.FRONTEND_SESSION_REQUEST,"ready").encode() ,SERVER_ENDPOINT)

def notify(packet):
    backendSock.sendto(packet.encode() ,SERVER_ENDPOINT)
