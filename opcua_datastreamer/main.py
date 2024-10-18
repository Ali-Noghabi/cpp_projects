import sys
import pandas as pd
from PySide6.QtWidgets import (
    QApplication,
    QMainWindow,
    QTableWidget,
    QTableWidgetItem,
    QVBoxLayout,
    QWidget,
    QSpinBox,
    QLabel,
)
from PySide6.QtCore import QTimer
from opcua import Server


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Sensor Data Monitor")
        self.setGeometry(100, 100, 800, 600)

        # Read data from Excel
        self.data = pd.read_excel("input_data.xlsx")  # Replace with your file path

        # Widgets
        self.table = QTableWidget(self)
        self.table.setColumnCount(2)  # Two columns: Sensor Name, Value
        self.table.setHorizontalHeaderLabels(["Sensor", "Value"])

        # SpinBox for setting interval (in seconds)
        self.interval_spinbox = QSpinBox(self)
        self.interval_spinbox.setRange(1, 60)  # You can adjust this range
        self.interval_spinbox.setValue(1)  # Default interval of 1 second

        layout = QVBoxLayout()
        layout.addWidget(QLabel("Select Interval (seconds):"))
        layout.addWidget(self.interval_spinbox)
        layout.addWidget(self.table)
        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

        # Set up timer
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_data)
        self.timer.start(1000)  # Default update every 1 second

        # Set up OPC UA server
        self.server = Server()
        self.server.set_endpoint("opc.tcp://localhost:4840/freeopcua/server/")

        # Setup namespace
        uri = "http://example.org"
        idx = self.server.register_namespace(uri)

        # Create a new object for the sensors
        self.sensor_node = self.server.nodes.objects.add_object(idx, "Sensors")

        # Create variables to represent sensor data
        self.sensor_variables = []
        self.node_info = []  # List to store NodeId and display name

        for column in self.data.columns[1:]:  # Skip timestamp column
            # Initialize with -1 to indicate missing data before streaming starts
            node = self.sensor_node.add_variable(idx, column, -1)
            node.set_writable()  # Set the variable to be writable
            self.sensor_variables.append(node)

            # Store NodeId and display name
            self.node_info.append((node.nodeid, column))

        # Save NodeId and display name to a text file
        self.save_node_info("node_info.txt")

        # Start the server
        self.server.start()
        print("OPC UA Server started at {}".format(self.server.endpoint))

        # Initialize variables
        self.current_row = 0

    def save_node_info(self, filename):
        with open(filename, 'w') as f:
            for nodeid, display_name in self.node_info:
                # Extract the namespace and identifier from the NumericNodeId
                ns = nodeid.NamespaceIndex
                i = nodeid.Identifier
                f.write(f"ns={ns};i={i} , {display_name}\n")
        print(f"Node information saved to {filename}")


    def update_data(self):
        interval = self.interval_spinbox.value() * 1000
        self.timer.setInterval(interval)

        # Update current row data to the table and OPC UA server
        self.update_table()
        self.stream_to_opcua()
        self.current_row = (self.current_row + 1) % len(self.data)

    def update_table(self):
        self.table.setRowCount(len(self.sensor_variables))  # Set rows equal to the number of sensors
        for idx, node in enumerate(self.sensor_variables):
            sensor_name = self.data.columns[idx + 1]  # Skip timestamp column
            value = self.data.iloc[self.current_row, idx + 1]  # Skip timestamp column

            # Check if value is numeric, otherwise set to -1
            value_to_display = self.get_numeric_value(value)

            self.table.setItem(idx, 0, QTableWidgetItem(sensor_name))
            self.table.setItem(idx, 1, QTableWidgetItem(str(value_to_display)))

    def stream_to_opcua(self):
        # Stream data from the current row to OPC UA server
        for idx, node in enumerate(self.sensor_variables):
            value = self.data.iloc[self.current_row, idx + 1]  # Skip timestamp column

            # Make sure to convert to a base Python type
            value_to_stream = self.get_numeric_value(value)

            # Convert to appropriate types if necessary
            if isinstance(value_to_stream, pd.Int64Dtype):
                value_to_stream = int(value_to_stream)  # Convert to plain int

            if isinstance(value_to_stream, pd.Float64Dtype):
                value_to_stream = float(value_to_stream)  # Convert to plain float

            node.set_value(value_to_stream)

    def get_numeric_value(self, value):
        # Convert the value to a number if possible, otherwise return -1
        try:
            numeric_value = pd.to_numeric(value, errors="coerce")  # Coerce non-numeric to NaN
            if pd.isnull(numeric_value):
                return None  # Return -1 for non-numeric values
            # Convert Pandas integer/float types to native Python types
            return (
                int(numeric_value)
                if isinstance(numeric_value, (pd.Int64Dtype, pd.Int32Dtype))
                else float(numeric_value)
            )
        except (ValueError, TypeError):
            return None  # Return -1 for non-numeric values

    def closeEvent(self, event):
        self.server.stop()
        print("OPC UA Server stopped")
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec())
