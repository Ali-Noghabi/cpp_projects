from PySide6.QtCore import QObject

class RestClient(QObject):
    def __init__(self):        
        super().__init__()
        self._user_login = False
        self._token = None
        self._user = None
        self._passwd = None
        self._rest_url = None
        self._ws_url = None
        self._project = None
        self._retry_interval = None
        self._ping_interval = None
        self._data_expire_interval = None
        self._topics = {}
        
        self.last_received = {}
        self.data_received = False

    @property    
    def user(self):
        return self._user

    @user.setter
    def user(self, value):
        self._user = value
        
    @property
    def passwd(self):
        return self._passwd

    @passwd.setter
    def passwd(self, value):
        self._passwd = value
    
    @property
    def rest_url(self):
        return self._rest_url

    @rest_url.setter
    def rest_url(self, value):
        self._rest_url = value

    @property
    def ws_url(self):
        return self._ws_url

    @ws_url.setter
    def ws_url(self, value):
        self._ws_url = value
        
    @property
    def project(self):
        return self._project

    @project.setter
    def project(self, value):
        self._project = value

    @property
    def retry_interval(self):
        return self._retry_interval

    @retry_interval.setter
    def retry_interval(self, value):
        self._retry_interval = value

    @property
    def ping_interval(self):
        return self._ping_interval

    @ping_interval.setter
    def ping_interval(self, value):
        self._ping_interval = value

    @property
    def data_expire_interval(self):
        return self._data_expire_interval

    @data_expire_interval.setter
    def data_expire_interval(self, value):
        self._data_expire_interval = value
        
    @property
    def user_login(self):
        return self._user_login

    @user_login.setter
    def user_login(self, value):
        self._user_login = value

    @property
    def token(self):
        return self._token

    @token.setter
    def token(self, value):
        self._token = value
        
    @property
    def topics(self):
        return self._topics

    @topics.setter
    def topics(self, value):
        self._topics = value

    def get_topic_name(self, topic):
        """Returns the name associated with the given topic."""
        return self.topics.get(topic, None)