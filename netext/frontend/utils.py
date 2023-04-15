import json
import time
import threading

DIFF_INSERT = 1
DIFF_REMOVE = -1
TIMER_MILLISECONDS = 1000


class Code:
    # requests
    CREATE_SESSION_REQUEST = 100
    JOIN_SESSION_REQUEST = 101
    DELETE_SESSION_REQUEST = 102
    FILE_INSERT_REQUEST = 103
    FILE_REMOVE_REQUEST = 104
    FRONTEND_SESSION_JOIN = 105
    FRONTEND_SESSION_LEAVE = 106
    CLIENT_LEAVE_REQUEST = 106

    # responses
    ERROR_RESPONSE = 200
    CREATE_SESSION_RESPONSE = 201
    JOIN_SESSION_RESPONSE = 202
    DELETE_SESSION_RESPONSE = 203
    PEER_INFO_RESPONSE = 204
    FILE_INSERT_RESPONSE = 205
    FILE_REMOVE_RESPONSE = 206


def getPortFromFile():
    """
        gets the backend port from the file the backend created
    """
    with open("../client/port.txt", "r") as portFile:
        strPort = portFile.read()
        print(strPort)
        return int(strPort)


def serialize(type, data):
    message = {"code": type, "time": int(time.time()), "data": data}
    return json.dumps(message)
