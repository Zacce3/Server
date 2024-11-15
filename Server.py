from flask import Flask, render_template_string, jsonify
import random  # For simulating sensor data
import socket  # To get the IP address

app = Flask(__name__)

# Simulate sensor data (replace with actual data from SCD30)
def get_sensor_data():
    return {
        "temperature": round(random.uniform(20, 30), 2),  # Simulated temperature
        "co2": round(random.uniform(400, 1000), 2)        # Simulated CO2
    }

# Function to get the current IP address of the Raspberry Pi
def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Doesn't actually connect, just checks for the IP
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
    except Exception:
        ip = "127.0.0.1"  # Default to localhost if unable to determine IP
    finally:
        s.close()
    return ip

@app.route('/')
def index():
    data = get_sensor_data()
    return render_template_string("""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Real-Time Sensor Data</title>
        <style>
            body { font-family: Arial, sans-serif; text-align: center; }
            h1 { color: #333; }
            .data { font-size: 24px; margin-top: 20px; }
        </style>
        <meta http-equiv="refresh" content="5">  <!-- Refresh every 5 seconds -->
    </head>
    <body>
        <h1>Real-Time Sensor Data</h1>
        <div class="data">Temperature: {{ data['temperature'] }} °C</div>
        <div class="data">CO2: {{ data['co2'] }} ppm</div>
        <p>Your public access URL: <strong>http://{{ ip }}:5000</strong></p>
    </body>
    </html>
    """, data=data, ip=get_ip_address())

if __name__ == "__main__":
    ip_address = get_ip_address()
    print(f"Server is running. Access it at: http://{ip_address}:5000")
    app.run(host="0.0.0.0", port=5000, debug=True)
