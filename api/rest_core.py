#!/usr/bin/env python3
import json
import socket
from threading import Thread

class NightApiServer:
    def __init__(self, port=9090):
        self.host = "127.0.0.1"
        self.port = port

    def handle_client(self, conn):
        req = conn.recv(1024).decode()
        if "GET /status" in req:
            res = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + json.dumps({"status": "ONLINE"})
            conn.sendall(res.encode())
        conn.close()

    def boot(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((self.host, self.port))
        s.listen(5)
        while True:
            c, a = s.accept()
            Thread(target=self.handle_client, args=(c,)).start()

if __name__ == "__main__":
    pass
