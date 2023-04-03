import socket
import subprocess

subprocess.Popen(["upnpc.exe", "-a","192.168.123.43","2222","7777","tcp"])
HOST = "192.168.123.43"
PORT = 2222
try:

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
                while data != '':
                    data = input()
                    conn.sendall(data.encode())
                    
except Exception as e:
    print("Error:", e)
finally:
    subprocess.Popen(["upnpc.exe", "-d","7777","tcp"])