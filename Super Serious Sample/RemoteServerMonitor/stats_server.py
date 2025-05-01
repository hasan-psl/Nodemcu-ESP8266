from flask import Flask, jsonify
import psutil
import time

app = Flask(__name__)

prev_net = psutil.net_io_counters()
prev_time = time.time()

@app.route('/stats')
def get_stats():
    global prev_net, prev_time

    # CPU and RAM usage
    cpu_percent = psutil.cpu_percent(interval=0.5)
    ram_percent = psutil.virtual_memory().percent

    # Power draw (if supported)
    try:
        # Works on many systems with upower or acpi tools
        import subprocess
        result = subprocess.check_output(["upower", "-i", "/org/freedesktop/UPower/devices/line_power_AC"])
        result = result.decode()
        power_draw = None
        for line in result.split("\n"):
            if "energy-rate:" in line:
                power_draw = float(line.split(":")[1].strip().split()[0])  # in W
                break
        if power_draw is None:
            power_draw = 0.0
    except:
        power_draw = 0.0

    # Network usage
    current_net = psutil.net_io_counters()
    current_time = time.time()

    duration = current_time - prev_time
    bytes_sent = current_net.bytes_sent - prev_net.bytes_sent
    bytes_recv = current_net.bytes_recv - prev_net.bytes_recv

    mb_sent = (bytes_sent / duration) / (1024 * 1024)  # Mbps-ish
    mb_recv = (bytes_recv / duration) / (1024 * 1024)

    prev_net = current_net
    prev_time = current_time

    return jsonify({
        "cpu": cpu_percent,
        "ram": ram_percent,
        "power": round(power_draw, 2),
        "net_up": round(mb_sent * 8, 1),    # Convert MBps to Mbps
        "net_down": round(mb_recv * 8, 1)
    })

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)