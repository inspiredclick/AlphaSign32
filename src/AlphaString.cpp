#include "AlphaString.h"

AlphaString::AlphaString() 
  : _label('1'), _size(0) {
}

AlphaString::AlphaString(char label, const String& data)
  : _label(label), _data(data) {
  _size = calculateSize();
}

AlphaPacket AlphaString::toPacket() const {
  // Format: [WRITE_STRING][Label][Data]
  String packetData = "";
  packetData += (char)WRITE_STRING;
  packetData += _label;
  packetData += _data;
  
  return AlphaPacket(packetData);
}

String AlphaString::call() const {
  // Returns the call sequence to insert this string into text: \x10[label]
  String callSeq = "";
  callSeq += (char)0x10;
  callSeq += _label;
  return callSeq;
}

uint8_t AlphaString::calculateSize() const {
  uint8_t size = _data.length();
  
  // Clamp to max 125 bytes
  if (size > 125) {
    size = 125;
  }
  
  return size;
}
