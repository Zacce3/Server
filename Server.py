from flask import Flask, jsonify, render_template_string, request
import serial
import threading
import time
import socket
import atexit

app = Flask(__name__)

# Serial data storage
sensor_data = {
    "co2": "No data",
    "temperature": "No data",
    "humidity": "No data",
    "distance": "No data",
    "window_state": "Unknown"
}

# Serial connection setup (change COM4 if necessary)
try:
    ser = serial.Serial('COM4', 9600, timeout=1)
    time.sleep(2)  # Allow time for the serial connection to initialize
    atexit.register(ser.close)
except serial.SerialException as e:
    print(f"Serial connection failed: {e}")
    ser = None

# Function to constantly read serial data
def read_serial():
    global sensor_data
    while True:
        if ser:
            try:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').strip()
                    if line.startswith("CO2"):
                        parts = line.split(",")
                        sensor_data["co2"] = parts[0].split(":")[1].strip() + " ppm"
                        sensor_data["temperature"] = parts[1].split(":")[1].strip() + " °C"
                        sensor_data["humidity"] = parts[2].split(":")[1].strip() + " %"
                        sensor_data["distance"] = parts[3].split(":")[1].strip() + " cm"
                        sensor_data["window_state"] = "Open" if "Open" in parts[4] else "Closed"
            except Exception as e:
                print(f"Error reading serial: {e}")
        time.sleep(0.1)

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

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        command = request.form.get('command')
        if ser and command in ['O', 'C']:
            ser.write(command.encode('utf-8'))
    return render_template_string("""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Actuator Control</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; margin: 20px; }
            h1 { color: #333; }
            .data { font-size: 18px; margin: 10px; }
            .control { margin: 20px; }
            button { padding: 10px 20px; font-size: 16px; margin: 5px; }
        </style>
    </head>
    <body>
        <h1>Real-Time Actuator Control</h1>
        <div class="data" id="sensor-data">
            <p>CO2 Concentration: <span id="co2">{{ co2 }}</span></p>
            <p>Temperature: <span id="temperature">{{ temperature }}</span></p>
            <p>Humidity: <span id="humidity">{{ humidity }}</span></p>
            <p>Distance: <span id="distance">{{ distance }}</span></p>
            <p>Window State: <span id="window_state">{{ window_state }}</span></p>
        </div>
        <div class="control">
            <form method="post">
                <button name="command" value="O">Open Window</button>
                <button name="command" value="C">Close Window</button>
            </form>
        </div>
        <p>Access the app at: <strong>http://{{ ip }}:5000</strong></p>
        <script>
            setInterval(() => {
                fetch('/data')
                    .then(response => response.json())
                    .then(data => {
                        document.getElementById('co2').innerText = data.co2;
                        document.getElementById('temperature').innerText = data.temperature;
                        document.getElementById('humidity').innerText = data.humidity;
                        document.getElementById('distance').innerText = data.distance;
                        document.getElementById('window_state').innerText = data.window_state;
                    });
            }, 2000); // Fetch data every 2 seconds
        </script>
    </body>
    </html>
    """, **sensor_data, ip=get_ip_address())

@app.route('/data')
def data():
    return jsonify(sensor_data)

if __name__ == "__main__":
    ip_address = get_ip_address()
    print(f"Server is running. Access it at: http://{ip_address}:5000")
    app.run(host="0.0.0.0", port=5000, debug=True, use_reloader=False)
