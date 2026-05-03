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

#include <AlphaSign.h>
#include <AlphaText.h>
#include <AlphaModes.h>
#include <AlphaColors.h>
#include <AlphaSpeeds.h>
#include <AlphaPositions.h>

// Create AlphaSign instance
AlphaSign sign;

// System monitoring variables
unsigned long lastMonitorTime = 0;
const unsigned long MONITOR_INTERVAL = 5000;  // 5 seconds

void setup() {
  // Initialize USB Serial for debugging and system monitoring
  Serial.begin(115200);
  while (!Serial && millis() < 3000)
    ;  // Wait up to 3 seconds for Serial

  Serial.println("\n=== AlphaSign ESP32 Example ===");
  Serial.println("Initializing...");

  // Enable debug output
  sign.setDebug(true);

  // Initialize Serial2 communication with LED sign
  sign.begin();  // Uses default GPIO16 (RX), GPIO17 (TX)

  Serial.println("Clearing sign memory...");
  sign.clearMemory();

  Serial.println("Allocating memory for text file...");
  // Allocate memory for text file 'A' - 64 bytes, unlocked
  sign.allocate('A', FILE_TYPE_TEXT, FILE_UNLOCKED, 64);

  Serial.println("Creating text message...");
  // Create a text message with color and speed
  String message = String(COLOR_RED) + String(SPEED_3) + "Hello ESP32!";
  AlphaText text('A', message, POS_MIDDLE_LINE, MODE_SCROLL);

  Serial.println("Sending text to sign...");
  sign.write(text.toPacket());

  Serial.println("Setting run sequence...");
  // Set run sequence to display file 'A'
  sign.setRunSequence("A");

  Serial.println("\nSetup complete!");
  Serial.println("System monitoring will log every 5 seconds...\n");

  // Initial system monitor
  logSystemStatus();
}

void loop() {
  // Check if it's time to log system status
  unsigned long currentTime = millis();

  if (currentTime - lastMonitorTime >= MONITOR_INTERVAL) {
    logSystemStatus();
    lastMonitorTime = currentTime;
  }

  // Add your other code here
  delay(100);  // Small delay to prevent tight loop
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
