import socket
import subprocess

subprocess.Popen(["C:\\Users\\avner\\Downloads\\upnpc-exe-win32-20220515\\upnpc.exe", "-a","192.168.123.43","2222","7777","tcp"])
HOST = "192.168.123.43"
PORT = 2222


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected to {addr}")
        while True:
            data = conn.recv(1024)
            if not data:
                break
            conn.sendall(data)
subprocess.Popen(["C:\\Users\\avner\\Downloads\\upnpc-exe-win32-20220515\\upnpc.exe", "-d","7777","tcp"])