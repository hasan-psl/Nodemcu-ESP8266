#include <Wire.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "SSD1306Wire.h" // Using SSD1306Wire library
#include "images.h"      // Custom images for symbols

// WiFi credentials
const char* ssid = "Hidden in the Leaf"; // Replace with your SSID
const char* password = "AnbuBlackOps"; // Replace with your password

// NTP client setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 6 * 3600, 60000); // GMT+6:00

// OLED display setup
SSD1306Wire display(0x3C, SDA, SCL); // Adjust I2C address if needed
// ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  display.init();
  display.flipScreenVertically(); // Optional

  // Connect to WiFi
  display.clear();
  display.drawString(10, 20, "Connecting to WiFi...");
  display.display();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // WiFi connected
  Serial.println("\nWiFi connected.");
  display.clear();
  display.drawString(10, 20, "WiFi Connected!");
  display.display();
  delay(1000);

  // Initialize NTP client
  timeClient.begin();
}

void loop() {
  // Update time
  timeClient.update();
  String formattedTime = timeClient.getFormattedTime();

  // Extract hours, minutes, and seconds
  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  // Determine AM/PM and convert to 12-hour format
  String amPm = "AM";
  if (hours >= 12) {
    amPm = "PM";
    if (hours > 12) {
      hours -= 12;
    }
  }
  if (hours == 0) {
    hours = 12; // Midnight hour in 12-hour format
  }

  // Format time as "HH:MM:SS"
  String timeString = (hours < 10 ? " " : "") + String(hours) + ":" +
                      (minutes < 10 ? "0" : "") + String(minutes) + ":" +
                      (seconds < 10 ? "0" : "") + String(seconds);

  // Get current date in "DD-MM-YYYY / Day" format
  time_t rawTime = timeClient.getEpochTime();
  struct tm* timeInfo = gmtime(&rawTime);
  String day = String(timeInfo->tm_mday);
  String month = String(timeInfo->tm_mon + 1);
  String year = String(timeInfo->tm_year + 1900);
  String weekday;
  switch (timeInfo->tm_wday) {
    case 0: weekday = "Sunday"; break;
    case 1: weekday = "Monday"; break;
    case 2: weekday = "Tuesday"; break;
    case 3: weekday = "Wednesday"; break;
    case 4: weekday = "Thursday"; break;
    case 5: weekday = "Friday"; break;
    case 6: weekday = "Saturday"; break;
  }
  String dateString = day + "-" + month + "-" + year + " / " + weekday;

  // Display time and date
  display.clear();

  // Draw the date at the top-center of the display
  display.setFont(ArialMT_Plain_10);
  int dateWidth = display.getStringWidth(dateString);
  display.drawString((display.getWidth() - dateWidth) / 2, 0, dateString);

  // Draw the clock (time) in the center
  display.setFont(ArialMT_Plain_24);
  int timeWidth = display.getStringWidth(timeString);
  display.drawString((display.getWidth() - timeWidth) / 1, 16, timeString);

  // Draw AM/PM next to the clock
  display.setFont(ArialMT_Plain_10);
  display.drawString(110, 40, amPm); // Adjusted position for AM/PM

  // Draw decorative active/inactive symbols
  display.drawXbm(0, 0, 8, 8, activeSymbol);
  display.drawXbm(120, 0, 8, 8, inactiveSymbol);

// Cat image variables
int catFrame = 0;

  // Draw the cat (40x40px)
  switch (catFrame) {
    case 0: display.drawXbm(0, 24, 40, 40, catFrame1); break;  // The catFrame1 is in XBM format
  }

  display.display();
  delay(100); // Update every second
}