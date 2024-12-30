
# Nodemcu-ESP8266

These are some example flash files for testing Nodemcu ESP8266 with Built-in 0.96 inch OLED display.

![Nodemcu ESP8266](https://github.com/user-attachments/assets/395621ba-7f39-4f3d-a893-f996e7e0cb59)

# Project Requirments

To run this project, you will need the following devices and softwares.

### » Hardware Requirment:

`1. Nodemcu ESP8266-12F with built-in 0.96 inch OLED display`

`2. USB A to Type-C or USB A to Micro-B cable (Your device specific cable)`

### » Software Requirment:

`1. Arduino IDE (installed on a Mac, Windows or a Linux machine) `

# Environment Variables

To run this project, you will need to add the following libraries to your Arduino IDE.

`ESP8266 and ESP32 OLED driver for SSD1306 displays`

`NTPClient`

`Time`

`ESP8266wifi` (Comes pre-installed with `ESP8266 Arduino core`)
# Deployment

### Install `Nodemcu ESP8266` Board Manager:

First things first. Connect your `Nodemcu ESP8266` to your computer.

You need to install `Nodemcu ESP8266` board manager in your IDE.

In `Arduino IDE`, go to File > Preferences...

![image](https://github.com/user-attachments/assets/ad9f5709-4030-4b38-92a5-b34daf2d9194)

Paste the following link in the `Additional boards manager URL's:` input field to install Arduino core for ESP8266 WiFi chip

```
https://arduino.esp8266.com/stable/package_esp8266com_index.json
```

![Image](https://github.com/user-attachments/assets/66740e4e-6178-4346-917e-cdfdeec695ed)

Click `OK`

Now, open Boards Manager from Tools > Board menu and install esp8266 platform.

(and don't forget to select your ESP8266 board from Tools > Board menu after installation)

### Install Libraries:

In `Arduino IDE`, go to Sketch > Include Library > Manage Libraries...

or, press `Ctrl+Shift+I` to open Library Manager directly.

![image](https://github.com/user-attachments/assets/0ae58837-928f-4f7d-85d2-be5ecf9986de)

Now, search and install these libraries:

```
ESP8266 and ESP32 OLED driver for SSD1306 displays

NTPClient

Time
```

After installing all the necessary libraries, Restart `Arduino IDE`

Now, you have proper environment to code and flash your `Nodemcu ESP8266`
# Demo

I created some example codes for testing Nodemcu ESP8266 with it's built-in 0.96 inch OLED display. Feel free to test them in your development board.

I am working on an internet clock program. This program will connect to your Wi-Fi and use a NTP client to fetch current date and time. It will fetch GMT +6 (You can change later) and convert 24h time format to 12h with amPM function. Then, it will show the date and time as a digital clock on the OLED display.

Currently, I have 2 versions of this program.

First one shows just the time and date. Second version has a cute cat in it. If you wanna see the cat, you have to flash the program and see for yourself. 😉

All of my current and future demo codes can be found at the `Super Serious Sample` folder.

If you like my stuff, you can `Fork`🍴 and `Star` ⭐ my repo.

Thanks for visiting.💚
# Documentation

[Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)

[Oled docs & Example by CHSElectronics](https://github.com/hasan-psl/Nodemcu-ESP8266/tree/main/Oled%20docs%20%26%20Example%20by%20CHSElectronics) (Created by [CHSElectronics](https://www.youtube.com/@CHSElectronics))
