import socket
import utils
from utils import Code,serialize
import network

SERVER_ADDRESS = "127.0.0.1"
server_port = 0

backendSock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
backendSock.bind((SERVER_ADDRESS,server_port))   
backendSock.settimeout(1)

SERVER_ENDPOINT = (SERVER_ADDRESS,utils.getPortFromFile())

def recv():
    """
    Receiving message while being able to quit
    """
    while True:
        try:
            msg , addr = network.backendSock.recvfrom(1024)
            return msg
        except socket.timeout: 
            continue
        except Exception as e:
            print(e)
            break
    return

def greet_backend():
    backendSock.sendto(utils.serialize(Code.FRONTEND_SESSION_JOIN,"join").encode() ,SERVER_ENDPOINT)

def leave_backend():
    backendSock.sendto(utils.serialize(Code.FRONTEND_SESSION_LEAVE,"leave").encode() ,SERVER_ENDPOINT)
    backendSock.close()

def send(packet):
    backendSock.sendto(packet.encode() ,SERVER_ENDPOINT)
