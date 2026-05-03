#include "AlphaText.h"

AlphaText::AlphaText() 
  : _label('A'), _position(POS_MIDDLE_LINE), _mode(MODE_ROTATE), _size(0) {
}

AlphaText::AlphaText(char label, const String& data, const String& position, const String& mode)
  : _label(label), _data(data), _position(position), _mode(mode) {
  _size = calculateSize();
}

AlphaPacket AlphaText::toPacket() const {
  // Format: [WRITE_TEXT][Label][ESC][Position][Mode][Data]
  String packetData = "";
  packetData += (char)WRITE_TEXT;
  packetData += _label;
  packetData += (char)ESC;
  packetData += _position;
  packetData += _mode;
  packetData += _data;
  
  return AlphaPacket(packetData);
}

uint8_t AlphaText::calculateSize() const {
  // Calculate total size including escape sequences and mode
  // ESC + position + mode + data
  uint8_t size = 1 + _position.length() + _mode.length() + _data.length();
  
  // Clamp to max 125 bytes
  if (size > 125) {
    size = 125;
  }
  
  return size;
}
