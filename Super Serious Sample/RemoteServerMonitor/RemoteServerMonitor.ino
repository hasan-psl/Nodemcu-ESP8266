#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <U8g2lib.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

// Display setup: SSD1306 128x64 I2C, full buffer mode
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// Wi-Fi and server config
const char* ssid = "Cat5 Got Your Tongue? 😼";
const char* password = "masmaskulas";
const char* server = "http://10.10.10.200:5000/stats";

// Timers
unsigned long lastFetchTime = 0;
const unsigned long fetchInterval = 1000; // 1 second
const int fps = 60;
unsigned long lastFrameTime = 0;

// Stats (smoothed)
float cpu_actual = 0, cpu_target = 0;
float ram_actual = 0, ram_target = 0;
float net_actual = 0, net_target = 0;

// Interpolation function
float smoothStep(float current, float target, float step = 0.05) {
  return current + (target - current) * step;
}

// Fetch JSON from server
void fetchData() {
  WiFiClient client;
  HTTPClient http;

  if (http.begin(client, server)) {
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      StaticJsonDocument<256> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        cpu_target = doc["cpu"];
        ram_target = doc["ram"];
        net_target = doc["net_down"]; // Mbps
      }
    }
    http.end();
  }
}

// Draw a ring arc around the edge of a circle
void drawRing(int cx, int cy, int radius, float percent, const char* label, String value) {
  int segments = 100;
  int filled = (percent / 100.0f) * segments;

  // Optional: draw full circle outline as background
  u8g2.drawCircle(cx, cy, radius);

int thickness = 3;
for (int i = 0; i < filled; i++) {
  float angle = ((float)i / segments) * 2 * PI - PI / 2;
  for (int t = -thickness/2; t <= thickness/2; t++) {
    float r = radius + t; // Draw outside
    int x = cx + cos(angle) * r;
    int y = cy + sin(angle) * r;
    u8g2.drawPixel(x, y);
  }
}

  // Draw value text in center
  u8g2.setFont(u8g2_font_6x10_tf);
  int w = u8g2.getStrWidth(value.c_str());
  u8g2.drawStr(cx - w / 2, cy + 4, value.c_str());

  // Draw label text under ring
  u8g2.setFont(u8g2_font_5x7_tr);
  int lw = u8g2.getStrWidth(label);
  u8g2.drawStr(cx - lw / 2, cy + radius + 10, label);
}

void setup() {
  WiFi.begin(ssid, password);
  u8g2.begin();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 30, "Connecting...");
    u8g2.sendBuffer();
  }
}

void loop() {
  unsigned long now = millis();

  // Fetch new data every second
  if (now - lastFetchTime > fetchInterval) {
    lastFetchTime = now;
    fetchData();
  }

  // Smooth interpolate between values
  cpu_actual = smoothStep(cpu_actual, cpu_target);
  ram_actual = smoothStep(ram_actual, ram_target);
  net_actual = smoothStep(net_actual, net_target);

  // Draw at 60 FPS
  if (now - lastFrameTime >= 1000 / fps) {
    lastFrameTime = now;

    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_6x10_tf); // Or use a slightly smaller one if it looks too chunky
    const char* title = "Linux Mint Server";
    int tw = u8g2.getStrWidth(title);
    u8g2.drawStr((128 - tw) / 2, 10, title); // y=10 leaves some breathing room

    // Draw 3 progress rings side by side
    drawRing(22, 35, 18, cpu_actual, "CPU", String((int)cpu_actual) + "%");
    drawRing(64, 35, 18, ram_actual, "RAM", String((int)ram_actual) + "%");
    drawRing(106, 35, 18, min(net_actual, 100.0f), "NET", String(net_actual, 1) + "Mb");

    u8g2.sendBuffer();
  }
}
