/*
 * SystemMonitor.ino
 * 
 * Comprehensive system monitoring example for ESP32
 * Logs detailed memory, CPU, and system information every 5 seconds
 * Minimal LED sign interaction to focus on monitoring capabilities
 * 
 * Hardware:
 * - ESP32 board
 * - LED Matrix Sign with Alpha Communications Protocol (optional)
 * - Serial2 connection: GPIO16 (RX), GPIO17 (TX)
 * 
 * This example is useful for:
 * - Debugging memory leaks
 * - Monitoring system performance
 * - Tracking resource usage over time
 * - Baseline testing before adding features
 */

#include <AlphaSign.h>
#include <AlphaText.h>
#include <AlphaModes.h>
#include <AlphaColors.h>

// Create AlphaSign instance
AlphaSign sign;

// System monitoring variables
unsigned long lastMonitorTime = 0;
const unsigned long MONITOR_INTERVAL = 5000; // 5 seconds

// Tracking variables for leak detection
uint32_t initialFreeHeap = 0;
uint32_t loopCounter = 0;
unsigned long startTime = 0;

void setup() {
  // Initialize USB Serial for system monitoring
  Serial.begin(115200);
  while (!Serial && millis() < 3000);
  
  Serial.println("\n========================================");
  Serial.println("=== AlphaSign ESP32 System Monitor ===");
  Serial.println("========================================\n");
  
  startTime = millis();
  
  // Record initial memory state
  initialFreeHeap = ESP.getFreeHeap();
  
  Serial.println("ESP32 Chip Information:");
  Serial.println("------------------------");
  printChipInfo();
  Serial.println();
  
  // Initialize sign (optional - comment out if no sign connected)
  Serial.println("Initializing AlphaSign...");
  sign.setDebug(false); // Disable debug to reduce serial output
  sign.begin();
  
  // Send a simple test message
  Serial.println("Sending test message to sign...");
  sign.clearMemory();
  sign.allocate('A', FILE_TYPE_TEXT, FILE_UNLOCKED, 50);
  
  String msg = String(COLOR_GREEN) + "ESP32 Monitoring";
  AlphaText text('A', msg, POS_MIDDLE_LINE, MODE_HOLD);
  sign.write(text.toPacket());
  sign.setRunSequence("A");
  
  Serial.println("\nSystem monitoring started!");
  Serial.println("Logging every 5 seconds...\n");
  
  // Initial status log
  logDetailedSystemStatus();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Increment loop counter for performance metrics
  loopCounter++;
  
  // Log system status every 5 seconds
  if (currentTime - lastMonitorTime >= MONITOR_INTERVAL) {
    logDetailedSystemStatus();
    lastMonitorTime = currentTime;
  }
  
  // Small delay to prevent CPU hammering
  delay(10);
}

void printChipInfo() {
  // Chip model
  Serial.print("Chip Model: ");
  Serial.println(ESP.getChipModel());
  
  // Chip revision
  Serial.print("Chip Revision: ");
  Serial.println(ESP.getChipRevision());
  
  // Number of cores
  Serial.print("CPU Cores: ");
  Serial.println(ESP.getChipCores());
  
  // CPU Frequency
  Serial.print("CPU Frequency: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  
  // Flash information
  Serial.print("Flash Size: ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");
  
  Serial.print("Flash Speed: ");
  Serial.print(ESP.getFlashChipSpeed() / 1000000);
  Serial.println(" MHz");
  
  // MAC Address
  Serial.print("MAC Address: ");
  Serial.println(ESP.getEfuseMac(), HEX);
}

void logDetailedSystemStatus() {
  Serial.println("================================================");
  Serial.println("           SYSTEM STATUS REPORT");
  Serial.println("================================================");
  
  // --- UPTIME ---
  unsigned long uptimeMs = millis() - startTime;
  unsigned long uptimeSec = uptimeMs / 1000;
  unsigned long uptimeMin = uptimeSec / 60;
  unsigned long uptimeHr = uptimeMin / 60;
  unsigned long uptimeDay = uptimeHr / 24;
  
  Serial.println("\n[UPTIME]");
  if (uptimeDay > 0) {
    Serial.print("  ");
    Serial.print(uptimeDay);
    Serial.print(" days, ");
  }
  Serial.print("  ");
  Serial.print(uptimeHr % 24);
  Serial.print("h ");
  Serial.print(uptimeMin % 60);
  Serial.print("m ");
  Serial.print(uptimeSec % 60);
  Serial.println("s");
  
  // --- MEMORY ---
  Serial.println("\n[MEMORY]");
  
  uint32_t freeHeap = ESP.getFreeHeap();
  uint32_t totalHeap = ESP.getHeapSize();
  uint32_t minFreeHeap = ESP.getMinFreeHeap();
  uint32_t usedHeap = totalHeap - freeHeap;
  
  Serial.print("  Free Heap:       ");
  Serial.print(freeHeap);
  Serial.print(" bytes (");
  Serial.print(freeHeap / 1024.0, 2);
  Serial.println(" KB)");
  
  Serial.print("  Used Heap:       ");
  Serial.print(usedHeap);
  Serial.print(" bytes (");
  Serial.print(usedHeap / 1024.0, 2);
  Serial.println(" KB)");
  
  Serial.print("  Total Heap:      ");
  Serial.print(totalHeap);
  Serial.print(" bytes (");
  Serial.print(totalHeap / 1024.0, 2);
  Serial.println(" KB)");
  
  Serial.print("  Min Free Heap:   ");
  Serial.print(minFreeHeap);
  Serial.print(" bytes (");
  Serial.print(minFreeHeap / 1024.0, 2);
  Serial.println(" KB)");
  
  Serial.print("  Heap Usage:      ");
  float heapUsage = 100.0 * (float)usedHeap / (float)totalHeap;
  Serial.print(heapUsage, 2);
  Serial.println("%");
  
  // Memory leak detection
  int32_t heapDelta = (int32_t)freeHeap - (int32_t)initialFreeHeap;
  Serial.print("  Heap Change:     ");
  if (heapDelta > 0) {
    Serial.print("+");
  }
  Serial.print(heapDelta);
  Serial.println(" bytes");
  
  if (heapDelta < -1000) {
    Serial.println("  ⚠️  WARNING: Possible memory leak detected!");
  }
  
  // PSRAM if available
  if (ESP.getPsramSize() > 0) {
    Serial.println("\n[PSRAM]");
    Serial.print("  Total PSRAM:     ");
    Serial.print(ESP.getPsramSize() / 1024);
    Serial.println(" KB");
    
    Serial.print("  Free PSRAM:      ");
    Serial.print(ESP.getFreePsram() / 1024);
    Serial.println(" KB");
    
    Serial.print("  Used PSRAM:      ");
    Serial.print((ESP.getPsramSize() - ESP.getFreePsram()) / 1024);
    Serial.println(" KB");
  }
  
  // --- CPU / PERFORMANCE ---
  Serial.println("\n[CPU & PERFORMANCE]");
  
  Serial.print("  CPU Frequency:   ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  
  Serial.print("  Loop Counter:    ");
  Serial.println(loopCounter);
  
  // Calculate loops per second
  float loopsPerSec = (float)loopCounter / ((float)uptimeSec);
  Serial.print("  Loops/Second:    ");
  Serial.print(loopsPerSec, 2);
  Serial.println(" Hz");
  
  // --- FLASH / STORAGE ---
  Serial.println("\n[FLASH STORAGE]");
  
  Serial.print("  Flash Size:      ");
  Serial.print(ESP.getFlashChipSize() / 1024 / 1024);
  Serial.println(" MB");
  
  Serial.print("  Flash Speed:     ");
  Serial.print(ESP.getFlashChipSpeed() / 1000000);
  Serial.println(" MHz");
  
  Serial.print("  Sketch Size:     ");
  Serial.print(ESP.getSketchSize() / 1024.0, 2);
  Serial.println(" KB");
  
  Serial.print("  Free Space:      ");
  Serial.print(ESP.getFreeSketchSpace() / 1024.0, 2);
  Serial.println(" KB");
  
  float sketchUsage = 100.0 * (float)ESP.getSketchSize() / 
                      (float)(ESP.getSketchSize() + ESP.getFreeSketchSpace());
  Serial.print("  Sketch Usage:    ");
  Serial.print(sketchUsage, 2);
  Serial.println("%");
  
  // --- HEALTH INDICATORS ---
  Serial.println("\n[SYSTEM HEALTH]");
  
  // Check various health indicators
  bool memoryHealthy = (heapUsage < 80.0) && (heapDelta > -5000);
  bool loopsHealthy = (loopsPerSec > 50.0);
  bool flashHealthy = (sketchUsage < 90.0);
  
  Serial.print("  Memory Health:   ");
  Serial.println(memoryHealthy ? "✓ GOOD" : "✗ POOR");
  
  Serial.print("  Loop Health:     ");
  Serial.println(loopsHealthy ? "✓ GOOD" : "✗ POOR");
  
  Serial.print("  Flash Health:    ");
  Serial.println(flashHealthy ? "✓ GOOD" : "✗ POOR");
  
  bool overallHealthy = memoryHealthy && loopsHealthy && flashHealthy;
  Serial.print("\n  Overall Status:  ");
  if (overallHealthy) {
    Serial.println("✓ ALL SYSTEMS NOMINAL");
  } else {
    Serial.println("⚠️  ATTENTION REQUIRED");
  }
  
  Serial.println("\n================================================\n");
}
