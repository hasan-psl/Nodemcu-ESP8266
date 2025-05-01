# Remote Server Monitor with ESP8266 + OLED Display

A smooth, minimalistic real-time system stats monitor for your Linux server using an ESP8266 NodeMCU and a 0.96" OLED display. This project displays CPU usage, RAM usage, and network throughput as radial progress bars using the U8g2 graphics library.

## 🧠 Features
- Real-time polling of system stats from a Flask API (every 1 second)
- 60 FPS OLED refresh for buttery smooth animations
- Radial progress bars for CPU, RAM, and network usage
- Lightweight and snappy JSON communication

## 🧰 Requirements
### Hardware
- ESP8266 NodeMCU (e.g., Amica or Lolin)
- 0.96" 128x64 OLED (SSD1306, I2C)

### Software
- Arduino IDE with ESP8266 board support
- Libraries:
  - `ESP8266WiFi`
  - `ESP8266HTTPClient`
  - `WiFiClient`
  - `ArduinoJson`
  - `U8g2`
- Python 3 on the server side with:
  - `Flask`
  - `psutil`

## ⚙️ Setup
### 1. Server Side (Linux Mint)
Create a Python Flask API to serve system stats:
```bash
pip install flask psutil
```

`server.py` example:
```python
from flask import Flask, jsonify
import psutil

app = Flask(__name__)

@app.route("/stats")
def stats():
    return jsonify({
        "cpu": psutil.cpu_percent(),
        "ram": psutil.virtual_memory().percent,
        "net": psutil.net_io_counters().bytes_sent + psutil.net_io_counters().bytes_recv
    })

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```
`Read the`
[Python Server for Remote PC](https://raw.githubusercontent.com/hasan-psl/Nodemcu-ESP8266/refs/heads/main/Super%20Serious%20Sample/RemoteServerMonitor/Python%20Server%20for%20Remote%20PC.txt)
`for more details and troubleshooting.`

### 2. ESP8266 Side
- Open `RemoteServerMonitor.ino` in Arduino IDE
- Edit these lines with your Wi-Fi credentials and Flask server IP:
```cpp
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";
const char* server = "http://your.server.ip:5000/stats";
```
- Connect your OLED to I2C pins (D1 = SCL, D2 = SDA)
- Upload and run the sketch

## 📡 How It Works
- Every 1 second, the ESP polls `/stats` from your Flask server.
- The data is parsed from JSON.
- The display is updated at 60 FPS with radial progress interpolation for smooth transitions.

## ⚠️ Notes
- Network usage is currently absolute (not Mbps). You might want to normalize it based on your max bandwidth.
- OLED burn-in is possible with static content. Consider screen dimming or sleep features later.

## 🚀 Future Ideas
- Add system temperature monitoring
- Use MQTT for more scalable monitoring
- Touch-based OLED interaction (if supported)

---
Made with ❤️ by Hasan
