#!/usr/bin/env python3
import socket
import json
import sys
from threading import Thread

class NightRestInterface:
    def __init__(self, port=9090):
        self.host = "127.0.0.1"
        self.port = port
        self.running = True

    def route_request(self, client_socket):
        try:
            raw_data = client_socket.recv(2048).decode("utf-8")
            if "GET /health" in raw_data:
                payload = json.dumps({"system": "nightOS", "status": "nominal"})
                response = f"HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: {len(payload)}\r\n\r\n{payload}"
                client_socket.sendall(response.encode("utf-8"))
        except Exception:
            pass
        finally:
            client_socket.close()

    def start_server(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            server.bind((self.host, self.port))
            server.listen(10)
        except Exception:
            return
        while self.running:
            try:
                conn, _ = server.accept()
                Thread(target=self.route_request, args=(conn,)).start()
            except KeyboardInterrupt:
                self.running = False
        server.close()

if __name__ == "__main__":
    api = NightRestInterface()
