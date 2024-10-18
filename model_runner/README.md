# Model Runner: Real-Time Data Processing Client

## Project Overview

**Model Runner** is a real-time data processing client built using PySide6 that connects to a server via REST APIs and WebSockets. It retrieves live data, processes it using a customizable neural network model, and pushes the results back to the server. The system is designed to handle continuous streams of data efficiently, making it adaptable for various machine learning models. By leveraging `QObject`, the project ensures rapid data processing, surpassing Python’s default processing capabilities, making it ideal for high-performance tasks.

![alt text](thumbnail.webp)

## Features

- **REST API & WebSocket Integration**: Establishes secure connections to a server, retrieving live data using REST APIs and WebSockets.
- **Real-Time Data Processing**: Processes live data using a customizable neural network model, allowing you to integrate any predictive model.
- **Fast Processing with `QObject`**: Utilizes `QObject` from PySide6 to handle data processing efficiently, ensuring faster performance compared to Python's default methods.
- **Signal-Based Communication**: Uses Qt signals and slots to manage communication between components, ensuring smooth, asynchronous data flow.
- **Configurable Subscriptions**: Subscribes to specific server topics defined in the configuration file (`config.ini`) for live data streaming.
- **Seamless Data Handling**: Efficiently handles both raw data creation and updating, ensuring that results are promptly sent back to the server.

## Installation and Usage Guide

### Prerequisites

- Python 3.7 or above
- Install required libraries:

```bash
pip install -r requirements.txt
```

### Starting the Project

1. Clone the repository to your local machine.
2. Ensure that the `config.ini` file is configured with the correct server details (see below for more details).
3. Start the application by running:

```bash
python main.py
```

### Configuration

Before starting the application, make sure to set up the `config.ini` file with the correct server URLs, authentication tokens, and other necessary settings:

```ini
[SERVER]
base_url = https://yourserver.com/api
token = your_api_token
data_expire_interval = 5000  # Data refresh interval in milliseconds

[WEBSOCKET]
ws_url = wss://yourserver.com/ws
topics = topic1,topic2
```

## How to Connect to the Server

- The application connects to the server using both REST APIs and WebSocket for real-time data.
- REST API authentication is managed via tokens configured in `config.ini`.
- Live data is streamed from the server using the WebSocket protocol, and the app subscribes to the configured topics to receive the data.
  
  For example, after launching the app, it will authenticate using the API token, subscribe to the topics provided in `config.ini`, start receiving live data, and process it using your integrated neural network model. The processed data is then automatically sent back to the server.
