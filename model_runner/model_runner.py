from PySide6.QtCore import QObject, QTimer, Slot, Signal, QDateTime
# import predict model
import sys
import os
predict_dir = os.path.join(os.path.dirname(__file__), 'predict')
sys.path.append(predict_dir)
from main_dashboard import main_dashboard

class ModelRunner(QObject):
    create_raw = Signal(map)
    push_raw = Signal(map)
    create_flag = False
    def __init__(self, rest_client, parent=None):
        super().__init__(parent)
        self.rest_client = rest_client
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.run_model)

    def run_model(self):
        # Load data from rest_client
        last_received = self.rest_client.last_received
        if(self.rest_client.data_received):
            self.rest_client.data_received = False
            model_input = {}
            for topic in last_received:
                data = last_received[topic]
                name = self.rest_client.get_topic_name(topic)
                # print(f"topic = {topic} name = {name} : {data['value']} : {data['receive_date'].toString()}")
                model_input[name] = data['value']
                
            print("model input : \n" , model_input)

            # run your model with input here
            model_output = main_dashboard(model_input)
            print("model output : \n" , model_output)
            if(self.create_flag):
                self.push_raw.emit(model_output)
            else:
                self.create_raw.emit(model_output)

    @Slot()
    def start(self):
        self.timer.start(self.rest_client.data_expire_interval)

    @Slot()
    def stop(self):
        self.timer.stop()
    @Slot()
    def create_raw_successful(self):
        self.create_flag = True
