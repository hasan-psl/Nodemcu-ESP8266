/*
  Project: Displaying all files in the MicroSD Card with 2 button navigation (Page-up and Page-down) control.

  Wiring:
    SD Card Module:
      CS  → D8
      SCK → D5
      MOSI → D7
      MISO → D6
      VCC → 3.3V
      GND → GND

    Buttons:
      One terminal of each button → D1 (Up) and D2 (Down).
      Other terminal of each button → GND.

  Notes:
    - Serial Monitor output will be at 115200 baud.
    - This code uses the SSD1306Wire library for OLED display.
*/

#include <Wire.h> // For I2C communication
#include <SPI.h>  // For SPI communication
#include <SD.h>   // For interacting with the SD card
#include "SSD1306Wire.h" // For OLED display control

// Pin definitions
#define SD_CS_PIN D8          // Chip Select pin for the SD card module
#define BUTTON_UP_PIN D1      // GPIO5 for "Up" button
#define BUTTON_DOWN_PIN D2    // GPIO4 for "Down" button

// Display setup
SSD1306Wire display(0x3C, SDA, SCL); // Initialize OLED display (address 0x3C)

// Constants and variables for file navigation
#define ITEMS_PER_PAGE 3       // Number of files to display per page
int currentPage = 0;           // Current page index
int totalFiles = 0;            // Total number of files on the SD card
String fileList[50];           // Array to store file names (maximum of 50 files)

// Initialization Functions
void initializeDisplay() {
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.drawString(0, 20, "Initializing...");
  display.display();
}

void initializeSD() {
  if (!SD.begin(SD_CS_PIN)) { // Check if SD card initialization is successful
    display.clear();
    display.drawString(0, 20, "SD Card Failed!");
    display.display();
    while (true); // Halt execution if SD initialization fails
  }

  display.clear();
  display.drawString(0, 20, "SD Card Ready!");
  display.display();
  delay(1000); // Wait 1 second before proceeding
}

// SD Card Handling Functions
void listFiles(File dir) {
  totalFiles = 0; // Reset file count

  while (true) {
    File entry = dir.openNextFile(); // Open the next file in the directory
    if (!entry) break; // Exit loop if no more files are found

    if (totalFiles < 50) { // Only store up to 50 files
      fileList[totalFiles] = entry.name(); // Save the file name
      totalFiles++;
    }
    entry.close(); // Close the current file
  }
}

// Display Handling Functions
void displayFileList() {
  display.clear();
  display.setFont(ArialMT_Plain_16); // Use a 10-point font size

  // Display page title
  display.drawString(0, 0, "Files & Folders:");

  // Determine which files to display on the current page
  int startIdx = currentPage * ITEMS_PER_PAGE;
  int endIdx = min(startIdx + ITEMS_PER_PAGE, totalFiles);

  // Display file names line by line
  display.setFont(ArialMT_Plain_10); // Use a 10-point font size
  for (int i = startIdx; i < endIdx; i++) {
    display.drawString(0, 15 * (i - startIdx) + 20, fileList[i]);
  }

  // Add navigation indicators ("Up" and "Down") if applicable
  if (currentPage > 0) display.drawString(110, 50, "Up");
  if ((currentPage + 1) * ITEMS_PER_PAGE < totalFiles) display.drawString(100, 50, "Down");

  display.display(); // Update the OLED display
}

// Input Handling Functions
void handleButtonInput() {
  // Check "Up" button for navigation
  if (digitalRead(BUTTON_UP_PIN) == LOW) {
    if (currentPage > 0) { // Ensure we don't go below page 0
      currentPage--;
      displayFileList(); // Update the display
      delay(200); // Debounce delay
    }
  }

  // Check "Down" button for navigation
  if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
    if ((currentPage + 1) * ITEMS_PER_PAGE < totalFiles) { // Ensure we don't exceed the total pages
      currentPage++;
      displayFileList(); // Update the display
      delay(200); // Debounce delay
    }
  }
}

void handleSerialInput() {
  // Optionally allow navigation via Serial Monitor for debugging
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'n' && (currentPage + 1) * ITEMS_PER_PAGE < totalFiles) {
      currentPage++; // Next page
    } else if (input == 'p' && currentPage > 0) {
      currentPage--; // Previous page
    }
    displayFileList(); // Update the display
  }
}

// Main Functions
void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(115200);

  // Initialize the OLED display
  initializeDisplay();

  // Initialize the SD card
  initializeSD();

  // Populate the file list from the SD card
  File root = SD.open("/"); // Open the root directory
  listFiles(root);

  // Set up button pins with internal pull-up resistors
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);

  // Display the initial file list
  displayFileList();
}

void loop() {
  handleButtonInput();
  handleSerialInput();
}
