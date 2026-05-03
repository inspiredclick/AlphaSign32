/*
 * AdvancedEffects.ino
 * 
 * Advanced example demonstrating multiple display effects, colors, and modes
 * Shows rotating messages with different visual effects
 * Includes system monitoring every 5 seconds
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
#include <AlphaString.h>
#include <AlphaModes.h>
#include <AlphaColors.h>
#include <AlphaSpeeds.h>
#include <AlphaPositions.h>
#include <AlphaExtChars.h>
#include <AlphaCounters.h>

// Create AlphaSign instance
AlphaSign sign;

// System monitoring variables
unsigned long lastMonitorTime = 0;
unsigned long lastMessageChange = 0;
const unsigned long MONITOR_INTERVAL = 5000; // 5 seconds
const unsigned long MESSAGE_INTERVAL = 10000; // Change message every 10 seconds

int currentMessage = 0;
const int NUM_MESSAGES = 4;

void setup() {
  // Initialize USB Serial for debugging and system monitoring
  Serial.begin(115200);
  while (!Serial && millis() < 3000);
  
  Serial.println("\n=== AlphaSign ESP32 Advanced Effects ===");
  Serial.println("Initializing...");
  
  // Enable debug output
  sign.setDebug(true);
  
  // Initialize Serial2 communication with LED sign
  sign.begin();
  
  Serial.println("Clearing sign memory...");
  sign.clearMemory();
  
  Serial.println("Setting up multiple text files and strings...");
  
  // Allocate memory for 4 different text files
  sign.allocate('A', FILE_TYPE_TEXT, FILE_UNLOCKED, 80);
  sign.allocate('B', FILE_TYPE_TEXT, FILE_UNLOCKED, 80);
  sign.allocate('C', FILE_TYPE_TEXT, FILE_UNLOCKED, 80);
  sign.allocate('D', FILE_TYPE_TEXT, FILE_UNLOCKED, 80);
  
  // Allocate memory for a string file (reusable text snippet)
  sign.allocate('1', FILE_TYPE_STRING, FILE_UNLOCKED, 30);
  
  // Create and send a STRING file
  AlphaString counterString('1', String("Count: ") + String(COUNTER_1));
  sign.write(counterString.toPacket());
  
  // Message 1: Rainbow scrolling text with heart
  String msg1 = String(COLOR_RAINBOW_1) + String(SPEED_4) + 
                "Welcome " + String(EXT_HEART) + " ESP32!";
  AlphaText text1('A', msg1, POS_MIDDLE_LINE, MODE_SCROLL);
  sign.write(text1.toPacket());
  
  // Message 2: Red rotating text with arrows
  String msg2 = String(COLOR_RED) + String(SPEED_3) + 
                String(EXT_RIGHT_ARROW) + " STATUS OK " + String(EXT_LEFT_ARROW);
  AlphaText text2('B', msg2, POS_MIDDLE_LINE, MODE_ROTATE);
  sign.write(text2.toPacket());
  
  // Message 3: Green sparkle effect with custom RGB
  String msg3 = colorRGB("00FF00") + String(SPEED_2) + 
                "SPARKLE EFFECT!";
  AlphaText text3('C', msg3, POS_MIDDLE_LINE, MODE_SPARKLE);
  sign.write(text3.toPacket());
  
  // Message 4: Yellow snow effect with extended characters
  String msg4 = String(COLOR_YELLOW) + String(SPEED_3) + 
                String(EXT_MUSICAL_NOTE) + " Music " + String(EXT_MUSICAL_NOTE) +
                " Snow " + String(EXT_INFINITY_SYMBOL);
  AlphaText text4('D', msg4, POS_FILL, MODE_SNOW);
  sign.write(text4.toPacket());
  
  // Set initial run sequence
  sign.setRunSequence("A");
  
  Serial.println("\nSetup complete!");
  Serial.println("Messages will rotate every 10 seconds");
  Serial.println("System monitoring every 5 seconds\n");
  
  // Beep to indicate setup complete
  sign.beep(3, 1, 1);
  
  // Initial system monitor
  logSystemStatus();
}

void loop() {
  unsigned long currentTime = millis();
  
  // System monitoring every 5 seconds
  if (currentTime - lastMonitorTime >= MONITOR_INTERVAL) {
    logSystemStatus();
    lastMonitorTime = currentTime;
  }
  
  // Change displayed message every 10 seconds
  if (currentTime - lastMessageChange >= MESSAGE_INTERVAL) {
    currentMessage = (currentMessage + 1) % NUM_MESSAGES;
    changeDisplayedMessage();
    lastMessageChange = currentTime;
  }
  
  delay(100);
}

void changeDisplayedMessage() {
  // Cycle through different messages
  char messageLabel = 'A' + currentMessage;
  
  Serial.print("Switching to message: ");
  Serial.println(messageLabel);
  
  String sequence = "";
  sequence += messageLabel;
  sign.setRunSequence(sequence);
  
  // Print current message info
  const char* messageNames[] = {
    "Rainbow Scroll with Heart",
    "Red Rotate with Arrows", 
    "Green Sparkle Effect",
    "Yellow Snow with Music Notes"
  };
  
  Serial.print("Now displaying: ");
  Serial.println(messageNames[currentMessage]);
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
  
  // PSRAM if available
  if (ESP.getPsramSize() > 0) {
    Serial.print("PSRAM Size: ");
    Serial.print(ESP.getPsramSize() / 1024);
    Serial.println(" KB");
    
    Serial.print("Free PSRAM: ");
    Serial.print(ESP.getFreePsram() / 1024);
    Serial.println(" KB");
  }
  
  // Flash information
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");
  
  Serial.print("Flash Speed: ");
  Serial.print(ESP.getFlashChipSpeed() / 1000000);
  Serial.println(" MHz");
  
  // Sketch size
  Serial.print("Sketch Size: ");
  Serial.print(ESP.getSketchSize() / 1024);
  Serial.println(" KB");
  
  Serial.print("Free Sketch Space: ");
  Serial.print(ESP.getFreeSketchSpace() / 1024);
  Serial.println(" KB");
  
  Serial.println("=========================\n");
}
