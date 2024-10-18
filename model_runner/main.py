import sys
import signal
from PySide6.QtCore import QCoreApplication
from manager import Manager

def signal_handler(sig, frame):
    print("Ctrl+C detected. Exiting application.")
    app.quit()  # Quit the application gracefully


if __name__ == "__main__":
    app = QCoreApplication(sys.argv)
    manger = Manager(app)

    # Register the signal handler for SIGINT (Ctrl+C)
    signal.signal(signal.SIGINT, signal_handler)

    try:
        sys.exit(app.exec())
    except SystemExit:
        print("Application closed.")
