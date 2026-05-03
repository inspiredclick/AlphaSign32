#include "AlphaSign.h"

AlphaSign::AlphaSign() 
  : _serial(&Serial2), _debug(false), _customPins(false), _rxPin(16), _txPin(17) {
}

void AlphaSign::begin() {
  // Initialize Serial2 with protocol requirements:
  // 4800 baud, 7 data bits, even parity, 2 stop bits
  _serial->begin(4800, SERIAL_7E2, _rxPin, _txPin);
  
  if (_debug) {
    Serial.println("AlphaSign: Serial2 initialized (4800 baud, 7E2)");
    Serial.print("AlphaSign: RX=GPIO");
    Serial.print(_rxPin);
    Serial.print(", TX=GPIO");
    Serial.println(_txPin);
  }
}

void AlphaSign::begin(int rxPin, int txPin) {
  _rxPin = rxPin;
  _txPin = txPin;
  _customPins = true;
  begin();
}

void AlphaSign::write(const AlphaPacket& packet) {
  writePacket(packet);
}

void AlphaSign::write(const String& data) {
  AlphaPacket packet(data);
  writePacket(packet);
}

void AlphaSign::writePacket(const AlphaPacket& packet) {
  String packetStr = packet.build();
  
  if (_debug) {
    Serial.print("AlphaSign: Sending packet (");
    Serial.print(packetStr.length());
    Serial.println(" bytes)");
    
    // Print hex dump for debugging
    // Serial.print("AlphaSign: ");
    // for (unsigned int i = 0; i < packetStr.length(); i++) {
    //   if (packetStr[i] < 0x10) Serial.print("0");
    //   Serial.print((uint8_t)packetStr[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println();
  }
  
  // Write packet to serial
  _serial->print(packetStr);
  _serial->flush();  // Wait for transmission to complete (blocking)
}

void AlphaSign::clearMemory() {
  // Send WRITE_SPECIAL with "$" to clear memory
  String data = "";
  data += (char)WRITE_SPECIAL;
  data += "$";
  
  AlphaPacket packet(data);
  writePacket(packet);
  
  // Critical 1-second delay for sign to process memory clear
  delay(1000);
  
  if (_debug) {
    Serial.println("AlphaSign: Memory cleared");
  }
}

void AlphaSign::allocate(char label, char fileType, char lock, uint8_t size) {
  String allocCmd = buildAllocateCommand(label, fileType, lock, size);
  AlphaPacket packet(allocCmd);
  writePacket(packet);
  
  if (_debug) {
    Serial.print("AlphaSign: Allocated file '");
    Serial.print(label);
    Serial.print("' type=");
    Serial.print(fileType);
    Serial.print(" size=");
    Serial.println(size);
  }
}

String AlphaSign::buildAllocateCommand(char label, char fileType, char lock, uint8_t size) {
  // Format: [WRITE_SPECIAL][$][Label][FileType][Lock][Size(hex)][QQQQ]
  String cmd = "";
  cmd += (char)WRITE_SPECIAL;
  cmd += "$";
  cmd += label;
  cmd += fileType;
  cmd += lock;
  
  // Convert size to 4-digit hex string
  char hexSize[5];
  sprintf(hexSize, "%04X", size);
  cmd += hexSize;
  
  // QQQQ field: FFFF for TEXT files, 0000 for STRING files
  if (fileType == FILE_TYPE_TEXT) {
    cmd += "FFFF";
  } else {
    cmd += "0000";
  }
  
  return cmd;
}

void AlphaSign::allocateAll() {
  // Allocate all 26 TEXT files (A-Z) in a SINGLE packet
  // Critical: Alpha protocol's E$ command REPLACES the entire allocation table
  // So we must send all files concatenated in one packet, not separate packets
  // Format per file: [Label][Type='A'][Lock='U'][Size=0040][QQQQ=FFFF]
  
  String cmd = "";
  cmd += (char)WRITE_SPECIAL;
  cmd += "$";
  
  // Concatenate allocation strings for files A-Z
  for (char label = 'A'; label <= 'Z'; label++) {
    cmd += label;           // File label
    cmd += FILE_TYPE_TEXT;  // 'A' for TEXT
    cmd += FILE_UNLOCKED;   // 'U' for unlocked
    cmd += "0040";          // 64 bytes in hex
    cmd += "FFFF";          // QQQQ field for TEXT files
  }
  
  AlphaPacket packet(cmd);
  writePacket(packet);
  
  if (_debug) {
    Serial.println("AlphaSign: Allocated files A-Z in single packet");
    Serial.print("AlphaSign: Packet size: ");
    Serial.print(cmd.length());
    Serial.println(" bytes");
  }
}

void AlphaSign::setRunSequence(const String& sequence) {
  // Format: [WRITE_SPECIAL][.][sequence]
  String data = "";
  data += (char)WRITE_SPECIAL;
  data += ".TU"; 
  data += sequence;
  
  AlphaPacket packet(data);
  writePacket(packet);
  
  if (_debug) {
    Serial.print("AlphaSign: Run sequence set to: ");
    Serial.println(sequence);
  }
}

void AlphaSign::beep(uint8_t frequency, uint8_t duration, uint8_t repeat) {
  // Format: [WRITE_SPECIAL][0x1C][frequency][0x1C][duration][0x1C][repeat]
  String data = "";
  data += (char)WRITE_SPECIAL;
  data += (char)0x1C;  // Special separator for beep command
  data += (char)frequency;
  data += (char)0x1C;
  data += (char)duration;
  data += (char)0x1C;
  data += (char)repeat;
  
  AlphaPacket packet(data);
  writePacket(packet);
  
  if (_debug) {
    Serial.println("AlphaSign: Beep command sent");
  }
}

void AlphaSign::softReset() {
  // Send WRITE_SPECIAL with "," for non-destructive reset
  String data = "";
  data += (char)WRITE_SPECIAL;
  data += ",";
  
  AlphaPacket packet(data);
  writePacket(packet);
  
  if (_debug) {
    Serial.println("AlphaSign: Soft reset");
  }
}
