#ifndef ALPHA_SIGN_H
#define ALPHA_SIGN_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include "AlphaPacket.h"
#include "AlphaConstants.h"

class AlphaSign {
public:
  AlphaSign();
  
  // Initialize Serial2 with 4800 baud, 7 data bits, even parity, 2 stop bits
  // GPIO16 = RX, GPIO17 = TX (default Serial2 pins on ESP32)
  void begin();
  void begin(int rxPin, int txPin);  // Custom pins if needed
  
  // Core communication methods
  void write(const AlphaPacket& packet);
  void write(const String& data);
  
  // Memory management
  void clearMemory();
  void allocate(char label, char fileType, char lock, uint8_t size);
  void allocateAll();  // Allocate files A-Z in single packet
  
  // Run sequence (display order)
  void setRunSequence(const String& sequence);
  
  // Special functions
  void beep(uint8_t frequency = 1, uint8_t duration = 0, uint8_t repeat = 0);
  void softReset();
  
  // Debug mode
  void setDebug(bool enabled) { _debug = enabled; }
  bool isDebug() const { return _debug; }
  
private:
  HardwareSerial* _serial;
  bool _debug;
  bool _customPins;
  int _rxPin;
  int _txPin;
  
  void writePacket(const AlphaPacket& packet);
  String buildAllocateCommand(char label, char fileType, char lock, uint8_t size);
};

#endif // ALPHA_SIGN_H
