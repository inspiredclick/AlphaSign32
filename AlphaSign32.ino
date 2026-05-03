/*
 * BasicExample.ino
 * 
 * Basic example for AlphaSign ESP32 library
 * Displays a simple message with system monitoring every 5 seconds
 * 
 * Hardware:
 * - ESP32 board
 * - LED Matrix Sign with Alpha Communications Protocol
 * - Serial2 connection: GPIO16 (RX), GPIO17 (TX)
 * 
 * Serial configuration: 4800 baud, 7 data bits, even parity, 2 stop bits
 */

#include "config.h"
#include <AlphaSign.h>
#include <AlphaText.h>
#include <AlphaModes.h>
#include <AlphaColors.h>
#include <AlphaSpeeds.h>
#include <AlphaPositions.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "src/HomePage.h"
#include "src/OTAPage.h"

// Create AlphaSign instance
AlphaSign sign;

// Create WebServer on port 80
WebServer server(80);

// Control pin configuration
const int CONTROL_PIN = POWER_CONTROL_PIN;  // GPIO5 for on/off control
bool powerState = false;  // Track power state

// File tracking for run sequence
String activeFiles = "";  // Track which files have content (e.g., "ABC")

// System monitoring variables
unsigned long lastMonitorTime = 0;
const unsigned long MONITOR_INTERVAL = MONITOR_INTERVAL_MS;  // From config.h

// Handle root path
void handleRoot() {
  server.send(200, "text/html", homePage);
}

// Handle sign update
void handleUpdate() {
  if (server.hasArg("message")) {
    String message = server.arg("message");
    String fileStr = server.hasArg("file") ? server.arg("file") : "A";
    String mode = server.hasArg("mode") ? server.arg("mode") : "scroll";
    String color = server.hasArg("color") ? server.arg("color") : "red";
    String speed = server.hasArg("speed") ? server.arg("speed") : "3";
    
    // Get file letter (A-Z only)
    char fileLabel = 'A';
    if (fileStr.length() > 0) {
      char c = fileStr.charAt(0);
      if (c >= 'A' && c <= 'Z') {
        fileLabel = c;
      } else if (c >= 'a' && c <= 'z') {
        fileLabel = toupper(c);
      }
    }
    
    // Map color names to color codes
    String colorCode;
    if (color == "red") colorCode = COLOR_RED;
    else if (color == "green") colorCode = COLOR_GREEN;
    else if (color == "amber") colorCode = COLOR_AMBER;
    else if (color == "orange") colorCode = COLOR_ORANGE;
    else if (color == "yellow") colorCode = COLOR_YELLOW;
    else if (color == "rainbow1") colorCode = COLOR_RAINBOW_1;
    else if (color == "rainbow2") colorCode = COLOR_RAINBOW_2;
    else colorCode = COLOR_RED;
    
    // Map speed to speed code
    String speedCode;
    if (speed == "1") speedCode = SPEED_1;
    else if (speed == "2") speedCode = SPEED_2;
    else if (speed == "3") speedCode = SPEED_3;
    else if (speed == "4") speedCode = SPEED_4;
    else if (speed == "5") speedCode = SPEED_5;
    else speedCode = SPEED_3;
    
    // Map mode to mode code
    String modeCode;
    if (mode == "hold") modeCode = MODE_HOLD;
    else if (mode == "rotate") modeCode = MODE_ROTATE;
    else if (mode == "flash") modeCode = MODE_FLASH;
    else if (mode == "rollup") modeCode = MODE_ROLL_UP;
    else if (mode == "rolldown") modeCode = MODE_ROLL_DOWN;
    else if (mode == "rollleft") modeCode = MODE_ROLL_LEFT;
    else if (mode == "rollright") modeCode = MODE_ROLL_RIGHT;
    else if (mode == "wipeup") modeCode = MODE_WIPE_UP;
    else if (mode == "wipedown") modeCode = MODE_WIPE_DOWN;
    else if (mode == "wipeleft") modeCode = MODE_WIPE_LEFT;
    else if (mode == "wiperight") modeCode = MODE_WIPE_RIGHT;
    else if (mode == "automode") modeCode = MODE_AUTOMODE;
    else if (mode == "rollin") modeCode = MODE_ROLL_IN;
    else if (mode == "rollout") modeCode = MODE_ROLL_OUT;
    else if (mode == "wipein") modeCode = MODE_WIPE_IN;
    else if (mode == "wipeout") modeCode = MODE_WIPE_OUT;
    else if (mode == "compressedrotate") modeCode = MODE_COMPRESSED_ROTATE;
    else if (mode == "explode") modeCode = MODE_EXPLODE;
    else if (mode == "clock") modeCode = MODE_CLOCK;
    // Special effects
    else if (mode == "twinkle") modeCode = MODE_TWINKLE;
    else if (mode == "sparkle") modeCode = MODE_SPARKLE;
    else if (mode == "snow") modeCode = MODE_SNOW;
    else if (mode == "interlock") modeCode = MODE_INTERLOCK;
    else if (mode == "switch") modeCode = MODE_SWITCH;
    else if (mode == "slide") modeCode = MODE_SLIDE;
    else if (mode == "spray") modeCode = MODE_SPRAY;
    else if (mode == "starburst") modeCode = MODE_STARBURST;
    else if (mode == "welcome") modeCode = MODE_WELCOME;
    else if (mode == "slotmachine") modeCode = MODE_SLOT_MACHINE;
    else if (mode == "newsflash") modeCode = MODE_NEWS_FLASH;
    else if (mode == "trumpetanimation") modeCode = MODE_TRUMPET_ANIMATION;
    else if (mode == "cyclecolors") modeCode = MODE_CYCLE_COLORS;
    // Graphics animations
    else if (mode == "thankyou") modeCode = MODE_THANK_YOU;
    else if (mode == "nosmoking") modeCode = MODE_NO_SMOKING;
    else if (mode == "dontdrinkdrive") modeCode = MODE_DONT_DRINK_DRIVE;
    else if (mode == "runninganimal") modeCode = MODE_RUNNING_ANIMAL;
    else if (mode == "fireworks") modeCode = MODE_FIREWORKS;
    else if (mode == "turbocar") modeCode = MODE_TURBO_CAR;
    else if (mode == "cherrybomb") modeCode = MODE_CHERRY_BOMB;
    else modeCode = MODE_SCROLL;
    
    // Write to the pre-allocated file (files must be initialized first via Initialize Files button)
    String fullMessage = colorCode + speedCode + message;
    AlphaText text(fileLabel, fullMessage, POS_MIDDLE_LINE, modeCode);
    sign.write(text.toPacket());
    
    // Track this file as having content (for UI feedback only)
    if (activeFiles.indexOf(fileLabel) == -1) {
      activeFiles += fileLabel;
      
      // Sort alphabetically for cleaner display order
      char fileArray[27];
      activeFiles.toCharArray(fileArray, activeFiles.length() + 1);
      for (int i = 0; i < activeFiles.length() - 1; i++) {
        for (int j = i + 1; j < activeFiles.length(); j++) {
          if (fileArray[i] > fileArray[j]) {
            char temp = fileArray[i];
            fileArray[i] = fileArray[j];
            fileArray[j] = temp;
          }
        }
      }
      activeFiles = String(fileArray);
    }
    
    // Note: Run sequence must be set via "Set Run Sequence" button
    // This will include all files (A-Z), and the sign displays only those with content
    
    Serial.println("Sign updated via web interface:");
    Serial.println("  File: " + String(fileLabel));
    Serial.println("  Message: " + message);
    Serial.println("  Mode: " + mode);
    Serial.println("  Color: " + color);
    Serial.println("  Speed: " + speed);
    Serial.println("  Files updated since boot: " + activeFiles);
    
    server.send(200, "text/plain", "Sign updated! File " + String(fileLabel) + " written.\n\nClick 'Set Run Sequence' to display all files.");
  } else {
    server.send(400, "text/plain", "Missing message parameter");
  }
}

// Handle status request
void handleStatus() {
  // Calculate uptime
  unsigned long uptimeMs = millis();
  unsigned long uptimeSec = uptimeMs / 1000;
  unsigned long hours = uptimeSec / 3600;
  unsigned long minutes = (uptimeSec % 3600) / 60;
  unsigned long seconds = uptimeSec % 60;
  
  String uptimeStr = String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
  
  // Build JSON response
  String json = "{";
  json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
  json += "\"rssi\":" + String(WiFi.RSSI()) + ",";
  json += "\"uptime\":\"" + uptimeStr + "\",";
  json += "\"heap\":" + String(ESP.getFreeHeap());
  json += "}";
  
  server.send(200, "application/json", json);
}

// Handle power toggle
void handlePowerToggle() {
  powerState = !powerState;
  digitalWrite(CONTROL_PIN, powerState ? HIGH : LOW);
  
  String json = "{";
  json += "\"state\":" + String(powerState ? "true" : "false");
  json += "}";
  
  Serial.println(powerState ? "Power: ON" : "Power: OFF");
  server.send(200, "application/json", json);
}

// Handle power state request
void handlePowerState() {
  String json = "{";
  json += "\"state\":" + String(powerState ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

// Handle files allocation (allocate without clearing)
void handleFilesInit() {
  Serial.println("Allocating files A-Z in single packet...");
  
  // Allocate all 26 text files (A-Z) in ONE packet
  // Critical: Each separate allocate() call REPLACES the entire allocation table
  // So we must use allocateAll() which sends all files concatenated in one packet
  sign.allocateAll();
  
  String json = "{";
  json += "\"success\":true,";
  json += "\"message\":\"Files A-Z allocated in single packet\"";
  json += "}";
  
  Serial.println("File allocation complete!");
  server.send(200, "application/json", json);
}

// Handle memory clear (separate from allocation)
void handleMemoryClear() {
  Serial.println("Clearing sign memory...");
  
  // Clear the active files list
  activeFiles = "";
  
  // Clear all sign memory
  sign.clearMemory();
  
  String json = "{";
  json += "\"success\":true,";
  json += "\"message\":\"Memory cleared\"";
  json += "}";
  
  Serial.println("Memory cleared!");
  server.send(200, "application/json", json);
}

// Handle run sequence reset - sets rotation to all files A-Z
void handleRunSequenceReset() {
  // Always set to all files A-Z
  // The sign will only display files that actually have content
  String sequence = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  sign.setRunSequence(sequence);
  
  String json = "{";
  json += "\"success\":true,";
  json += "\"message\":\"Run sequence set to A-Z. Files with content will display.\"";
  json += "}";
  
  Serial.println("Run sequence set to: " + sequence);
  Serial.println("Note: Only files with content will actually display");
  server.send(200, "application/json", json);
}

// Handle OTA page
void handleOTAPage() {
  server.send(200, "text/html", otaPage);
}

// Handle OTA firmware upload
void handleOTAUpdate() {
  server.sendHeader("Connection", "close");
  server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
  delay(1000);
  ESP.restart();
}

void handleOTAUpload() {
  HTTPUpload& upload = server.upload();
  
  if (upload.status == UPLOAD_FILE_START) {
    Serial.printf("OTA Update: %s\n", upload.filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      Update.printError(Serial);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (Update.end(true)) {
      Serial.printf("OTA Update Success: %u bytes\nRebooting...\n", upload.totalSize);
    } else {
      Update.printError(Serial);
    }
  }
}


void setup() {
  // Initialize USB Serial for debugging and system monitoring
  Serial.begin(DEBUG_SERIAL_BAUD);
  while (!Serial && millis() < SERIAL_TIMEOUT_MS)
    ;  // Wait for Serial connection

  delay(1000);  // Short delay for stability

  Serial.println("\n=== AlphaSign ESP32 ===");
  Serial.println("Initializing...");

  // Initialize control pin
  pinMode(CONTROL_PIN, OUTPUT);
  digitalWrite(CONTROL_PIN, LOW);  // Start with power off
  powerState = false;
  Serial.print("Control Pin (GPIO");
  Serial.print(CONTROL_PIN);
  Serial.println(") initialized: OFF");

  Serial.println("\n=== WIFI CONNECTION ===");
  // Connect to WiFi (optional, for NTP time or remote monitoring)
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize mDNS
  if (!MDNS.begin(OTA_HOSTNAME)) {
    Serial.println("Error setting up mDNS responder!");
  } else {
    Serial.print("mDNS responder started: http://");
    Serial.print(OTA_HOSTNAME);
    Serial.println(".local");
  }

  // Setup web server routes
  Serial.println("\n=== WEB SERVER ===");
  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.on("/status", handleStatus);
  server.on("/power/toggle", handlePowerToggle);
  server.on("/power/state", handlePowerState);
  server.on("/files/init", handleFilesInit);
  server.on("/memory/clear", handleMemoryClear);
  server.on("/runsequence/reset", handleRunSequenceReset);
  server.on("/ota", HTTP_GET, handleOTAPage);
  server.on("/ota", HTTP_POST, handleOTAUpdate, handleOTAUpload);
  
  // Start web server
  server.begin();
  Serial.println("Web server started!");
  Serial.print("Access the control panel at: http://");
  Serial.println(WiFi.localIP());
  Serial.print("OTA updates available at: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ota");
  Serial.print("Control Pin: GPIO");
  Serial.println(CONTROL_PIN);

  // Enable debug output
  sign.setDebug(ENABLE_DEBUG);

  // Initialize Serial2 communication with LED sign
  sign.begin(ALPHASIGN_RX_PIN, ALPHASIGN_TX_PIN);

  // Initial system monitor
  logSystemStatus();
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Check if it's time to log system status
  unsigned long currentTime = millis();

  if (currentTime - lastMonitorTime >= MONITOR_INTERVAL) {
    logSystemStatus();
    lastMonitorTime = currentTime;
  }

  // Add your other code here
  delay(10);  // Small delay to prevent tight loop
}

void logSystemStatus() {
  Serial.println("===== SYSTEM STATUS =====");

  // Uptime
  unsigned long uptimeMs = millis();
  unsigned long uptimeSec = uptimeMs / 1000;
  unsigned long uptimeMin = uptimeSec / 60;
  unsigned long uptimeHr = uptimeMin / 60;

  Serial.print("Uptime: ");
  Serial.print(uptimeHr);
  Serial.print("h ");
  Serial.print(uptimeMin % 60);
  Serial.print("m ");
  Serial.print(uptimeSec % 60);
  Serial.println("s");

  // Memory information
  Serial.print("Free Heap: ");
  Serial.print(ESP.getFreeHeap());
  Serial.println(" bytes");

  Serial.print("Total Heap: ");
  Serial.print(ESP.getHeapSize());
  Serial.println(" bytes");

  Serial.print("Min Free Heap: ");
  Serial.print(ESP.getMinFreeHeap());
  Serial.println(" bytes");

  Serial.print("Heap Usage: ");
  float heapUsage = 100.0 * (1.0 - (float)ESP.getFreeHeap() / (float)ESP.getHeapSize());
  Serial.print(heapUsage, 1);
  Serial.println("%");

  // CPU information
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");

  // Flash information
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");

  Serial.println("=========================\n");
}
