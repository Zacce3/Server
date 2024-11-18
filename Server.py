from flask import Flask, render_template_string
import serial
import threading
import time
import socket
import atexit  # Import atexit to handle cleanup

app = Flask(__name__)

# Serial data storage
serial_data = "No data yet"

# Serial connection setup (change COM4 if necessary)
try:
    ser = serial.Serial('COM7', 115200, timeout=1)
    time.sleep(2)  # Allow time for the serial connection to initialize

    # Register the cleanup function to close the serial connection on exit
    atexit.register(ser.close)
except serial.SerialException as e:
    print(f"Serial connection failed: {e}")
    ser = None
    serial_data = "No serial connection"

# Function to constantly read serial data
def read_serial():
    global serial_data
    while True:
        if ser:
            try:
                if ser.in_waiting > 0:
                    serial_data = ser.readline().decode('utf-8').strip()
            except serial.SerialException as e:
                serial_data = f"Serial disconnected: {e}"
                break
            except Exception as e:
                serial_data = f"Error reading serial: {e}"
        else:
            serial_data = "No serial connection"
        time.sleep(0.1)  # Add a delay to reduce CPU usage

# Start the serial reading in a separate thread
if ser:
    serial_thread = threading.Thread(target=read_serial, daemon=True)
    serial_thread.start()

# Function to get the current IP address of the PC
def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
    except Exception:
        ip = "127.0.0.1"
    finally:
        s.close()
    return ip

@app.route('/')
def index():
    return render_template_string("""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Real-Time Serial Data</title>
        <style>
    body {{ '{{' }} font-family: Arial, sans-serif; text-align: center; {{ '}}' }}
    h1 {{ '{{' }} color: #333; {{ '}}' }}
    .data {{ '{{' }} font-size: 24px; margin-top: 20px; {{ '}}' }}
</style>

        <meta http-equiv="refresh" content="5">  <!-- Refresh every 5 seconds -->
    </head>
    <body>
        <h1>Real-Time Serial Data</h1>
        <div class="data">Data from Mega: {{ data }}</div>
        <p>Your public access URL: <strong>http://{{ ip }}:5000</strong></p>

        <!-- Add a button -->
        <form action="/action" method="post">
            <button type="submit">Click Me</button>
        </form>
    </body>
    </html>
    """, data=serial_data, ip=get_ip_address())

@app.route('/action', methods=['POST'])
def action():
    if ser:
        ser.write(b'1')
    # Add your logic here, e.g., interacting with the serial device
    return render_template_string("""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <title>Action Triggered</title>
    </head>
    <body>
        <h1>Button Pressed!</h1>
        <a href="/">Go Back</a>
    </body>
    </html>
    """)


if __name__ == "__main__":
    ip_address = get_ip_address()
    print(f"Server is running. Access it at: http://{ip_address}:5000")
    app.run(host="0.0.0.0", port=5000, debug=True, use_reloader=False)
