from PySide6.QtCore import QObject, Slot
from configparser import ConfigParser

from rest_client import RestClient
from rest_APIs import RestAPIs
from websocket_notification import WebSocketNotification
from websocket_streaming import WebSocketStreaming
from model_runner import ModelRunner


class Manager(QObject):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.topics = []
        self.rest_client = RestClient()
        self.config("config.ini")
        self.rest_apis = RestAPIs(self.rest_client)
        self.websocket_stream = WebSocketStreaming(self.rest_client)
        self.websocket_notification = WebSocketNotification(self.rest_client)
        self.model_runner = ModelRunner(self.rest_client)
        self.connector()
        self.rest_apis.login()

    def config(self, config_path):
        config_parser = ConfigParser()
        config_parser.read(config_path)
        self.rest_client.user = config_parser.get("AUTH", "user")
        self.rest_client.passwd = config_parser.get("AUTH", "passwd")
        self.rest_client.rest_url = config_parser.get("SERVER", "rest_url")
        self.rest_client.ws_url = config_parser.get("SERVER", "ws_url")
        self.rest_client.project = config_parser.get("SERVER", "project")
        self.rest_client.retry_interval = config_parser.getint("SETTINGS", "retry_interval")
        self.rest_client.ping_interval = config_parser.getint("SETTINGS", "ping_interval")
        self.rest_client.data_expire_interval = config_parser.getint("SETTINGS", "data_expire_interval")
        
        topics_str = config_parser.get('SERVER', 'topics')
        topics = {}
        for item in topics_str.split(','):
            key, value = item.strip().split(':')
            topics[key.strip()] = value.strip()
            
        self.rest_client.topics = topics
        self.topics = list(topics.keys())

    def connector(self):

        self.rest_apis.login_successful.connect(self.websocket_notification.start)
        self.rest_apis.login_successful.connect(self.websocket_stream.start)
        self.rest_apis.login_successful.connect(self.model_runner.start)
        self.rest_apis.create_raw_successful.connect(self.model_runner.create_raw_successful)

        self.websocket_notification.logout.connect(self.rest_apis.logout)
        self.websocket_notification.logout.connect(self.model_runner.stop)
        self.websocket_notification.logout.connect(self.websocket_stream.stop)

        self.websocket_stream.started.connect(self.subscribe)
        
        self.model_runner.create_raw.connect(self.rest_apis.create_raw)
        self.model_runner.push_raw.connect(self.rest_apis.push_raw)

    @Slot()
    def subscribe(self):
        print("send message")
        print(self.topics)
        self.websocket_stream.subscribe(self.topics)
