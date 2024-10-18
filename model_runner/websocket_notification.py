from PySide6.QtCore import QObject, Signal, Slot, QTimer, QUrl
from PySide6.QtWebSockets import QWebSocket
import json

class WebSocketNotification(QObject):
    closed = Signal()
    logout = Signal()

    def __init__(self, rest_client):
        super().__init__()
        self.rest_client = rest_client
        self.web_socket = QWebSocket()
        self.ping_timer = QTimer(self)
        self.ping_outstanding = False

        # Connecting signals and slots
        self.web_socket.connected.connect(self.on_connected)
        self.web_socket.disconnected.connect(self.on_close)
        self.web_socket.pong.connect(self.pong)
        self.ping_timer.timeout.connect(self.ping)

    @Slot(str)
    def start(self, token):
        url = self.rest_client.ws_url + "/notification"
        url += "?token=" + token
        print(f"Opening WebSocket Notification connection to {url}")
        self.web_socket.open(QUrl(url))

    @Slot()
    def on_connected(self):
        print("WebSocket Notification connected")
        self.ping_timer.start(self.rest_client.ping_interval)

    @Slot()
    def on_close(self):
        print("WebSocket Notification disconnected")
        self.ping_timer.stop()
        self.closed.emit()
        self.handle_ping_status({"ping": -1, "message": "WebSocket Notification Closed"})

    @Slot()
    def ping(self):
        if not self.ping_outstanding:
            self.web_socket.ping()
            self.ping_outstanding = True
        else:
            self.handle_ping_status({"ping": -1, "message": "no response!"})

    @Slot(int, bytes)
    def pong(self, elapsedTime):
        self.handle_ping_status({"ping": elapsedTime})
        self.ping_outstanding = False
        
    def handle_ping_status(self,ping_data):
        if ping_data["ping"] == -1:
            print("Ping failed, message:", ping_data["message"])
            self.rest_client.user_login = False
            self.logout.emit()
        else:
            print(f"Ping received: {ping_data["ping"]} ms")
