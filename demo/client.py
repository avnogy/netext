
import socket

HOST = "81.218.189.62"  # The server's hostname or IP address
PORT = 7777  # The port used by the server

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.sendall(b"Hello, world")
    data = s.recv(1024)
    while data:
        print(f"Received {data.decode()}")
        data = s.recv(1024)