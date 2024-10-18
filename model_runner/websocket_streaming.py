from PySide6.QtCore import QObject, Signal, Slot, QUrl , QDateTime
from PySide6.QtWebSockets import QWebSocket
import json


class WebSocketStreaming(QObject):
    closed = Signal()
    pingStatus = Signal(dict)
    started = Signal()

    def __init__(self, rest_client):
        super().__init__()
        self.rest_client = rest_client
        self.web_socket = QWebSocket()

        # Connecting signals and slots
        self.web_socket.connected.connect(self.on_connected)
        self.web_socket.disconnected.connect(self.on_close)
        self.web_socket.textMessageReceived.connect(self.on_text_message_received)

    @Slot(str)
    def start(self, token):  # Slot that receives the token
        url = self.rest_client.ws_url + "/stream"
        url += "?token=" + token
        print(f"Opening WebSocket Streaming connection to {url}")
        self.web_socket.open(QUrl(url))
        
    @Slot()
    def stop(self):
        self.web_socket.close()

    @Slot()
    def on_connected(self):
        print("WebSocket Streaming connected")
        self.started.emit()

    @Slot()
    def on_close(self):
        print("WebSocket Streaming disconnected")
        self.closed.emit()

    @Slot(str)
    def on_text_message_received(self, message):
        try:
            response = json.loads(message)
            if 'data' in response and response['data']:
                topic = response['topic']
                data = response['data'][0]
                now = QDateTime.currentDateTime()

                # Update the last received message for this topic
                self.rest_client.last_received[topic] = {'receive_date': now, 'value': data}
                self.rest_client.data_received = True
                                
        except json.JSONDecodeError as e:
            print(f"Failed to parse JSON response: {e}")


    def subscribe(self, topics):
        """
        Sends a subscribe message with topics to the WebSocket.
        """
            
        data = {"cmd": "addTopic", "topics": topics}
        self.send_message(data)

    def send_message(self, data):
        message_str = json.dumps(data)
        self.web_socket.sendTextMessage(message_str)
