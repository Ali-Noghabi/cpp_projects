import json
from datetime import datetime
import pandas as pd
import numpy as np
from PySide6.QtCore import QObject, Slot, Signal, QTimer, QUrl
from PySide6.QtNetwork import QNetworkAccessManager, QNetworkRequest, QNetworkReply

class RestAPIs(QObject):
    login_successful = Signal(str)
    create_raw_successful = Signal()

    def __init__(self, rest_client):
        super().__init__()
        self.rest_client = rest_client
        self.network_manager = QNetworkAccessManager(self)
        self.timer = QTimer(self)
        self.login_retry_interval = self.rest_client.retry_interval

    def login(self):
        passwd = self.rest_client.passwd
        user = self.rest_client.user
        endpoint = f"{self.rest_client.rest_url}/api/v1/login"
        payload = {"user": user, "passwd": passwd}

        request = QNetworkRequest()
        request.setUrl(QUrl(endpoint))
        request.setHeader(QNetworkRequest.ContentTypeHeader, "application/json")

        reply = self.network_manager.post(request, json.dumps(payload).encode('utf-8'))
        reply.finished.connect(self.login_reply)

    @Slot()
    def login_reply(self):
        reply = self.sender()
        if reply.error() == QNetworkReply.NoError:
            try:
                response = json.loads(reply.readAll().data())
                if 'access_token' in response:
                    token = response['access_token']
                    print(f"Login successful, token: {token}")
                    self.rest_client.token = token
                    self.login_successful.emit(token)
                    self.rest_client.user_login = True

                else:
                    print("Login failed, no token received.")
                    self.retry_login()
            except json.JSONDecodeError as e:
                print(f"Failed to parse JSON response: {e}")
                self.retry_login()
        else:
            print(f"Login failed: {reply.errorString()}")
            self.retry_login()

        reply.deleteLater()
        
    def retry_login(self):
        print("Retrying login...")
        self.timer.singleShot(self.login_retry_interval, self.login)

    @Slot()
    def logout(self):
        self.retry_login()
        
    @Slot(map)
    def create_raw(self, data_map):
        if not self.rest_client.user_login:
            print("User not logged in. Cannot push data.")
            return

        endpoint = f"{self.rest_client.rest_url}/api/v1/MM/createRaw"
        token = self.rest_client.token
        data_map["project"] = self.rest_client.project
        payload = {"data": data_map}

        request = QNetworkRequest()
        request.setUrl(QUrl(endpoint))
        request.setHeader(QNetworkRequest.ContentTypeHeader, "application/json")
        request.setRawHeader(b"Authorization", f"Bearer {token}".encode('utf-8'))

        reply = self.network_manager.post(request, json.dumps(payload, cls=CustomEncoder).encode('utf-8'))
        reply.finished.connect(self.create_raw_reply)
        
    @Slot()
    def create_raw_reply(self):
        reply = self.sender()  # Get the reply from the sender
        if reply.error() == QNetworkReply.NoError:
            try:
                response = json.loads(reply.readAll().data())
                self.create_raw_successful.emit()
                print(f"Data pushed successfully: {response}")
                
            except json.JSONDecodeError as e:
                print(f"Failed to parse response JSON: {e}")
        else:
            print(f"Failed to push data: {reply.errorString()}")

        reply.deleteLater()  # Clean up the reply object
        
    @Slot(map)
    def push_raw(self, data_map):
        if not self.rest_client.user_login:
            print("User not logged in. Cannot push data.")
            return

        endpoint = f"{self.rest_client.rest_url}/api/v1/MM/pushRaw"
        token = self.rest_client.token
        data_map["project"] = self.rest_client.project
        payload = {"data": data_map}

        request = QNetworkRequest()
        request.setUrl(QUrl(endpoint))
        request.setHeader(QNetworkRequest.ContentTypeHeader, "application/json")
        request.setRawHeader(b"Authorization", f"Bearer {token}".encode('utf-8'))

        reply = self.network_manager.post(request, json.dumps(payload, cls=CustomEncoder).encode('utf-8'))
        reply.finished.connect(self.push_raw_reply)
        
    @Slot()
    def push_raw_reply(self):
        reply = self.sender()  # Get the reply from the sender
        if reply.error() == QNetworkReply.NoError:
            try:
                response = json.loads(reply.readAll().data())
                self.create_raw_successful.emit()
                print(f"Data pushed successfully: {response}")
                
            except json.JSONDecodeError as e:
                print(f"Failed to parse response JSON: {e}")
        else:
            print(f"Failed to push data: {reply.errorString()}")

        reply.deleteLater()  # Clean up the reply object
        
class CustomEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, pd.Timestamp):
            return obj.isoformat()  # Convert Timestamp to ISO 8601 string
        elif isinstance(obj, np.ndarray):
            return obj.tolist()  # Convert numpy array to list
        elif isinstance(obj, (np.float32, np.float64)):  # Handle numpy floating points
            return float(obj)  # Convert numpy float32 or float64 to a Python float
        elif isinstance(obj, datetime):
            return obj.isoformat()  # Convert datetime to ISO 8601 string
        elif isinstance(obj, np.int64 ):
            return int(obj)
        return super().default(obj)

