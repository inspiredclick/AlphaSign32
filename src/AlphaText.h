#ifndef ALPHA_TEXT_H
#define ALPHA_TEXT_H

#include <Arduino.h>
#include "AlphaPacket.h"
#include "AlphaConstants.h"
#include "AlphaPositions.h"
#include "AlphaModes.h"

class AlphaText {
public:
  AlphaText();
  AlphaText(char label, const String& data, const String& position = POS_MIDDLE_LINE, 
            const String& mode = MODE_ROTATE);
  
  // Generate packet for writing this text file to sign
  AlphaPacket toPacket() const;
  
  // Setters
  void setLabel(char label) { _label = label; }
  void setData(const String& data) { _data = data; }
  void setPosition(const String& position) { _position = position; }
  void setMode(const String& mode) { _mode = mode; }
  void setSize(uint8_t size) { _size = size; }
  
  // Getters
  char getLabel() const { return _label; }
  String getData() const { return _data; }
  String getPosition() const { return _position; }
  String getMode() const { return _mode; }
  uint8_t getSize() const { return _size; }
  
  // Calculate size automatically from data
  uint8_t calculateSize() const;
  
private:
  char _label;
  String _data;
  String _position;
  String _mode;
  uint8_t _size;
};

#endif // ALPHA_TEXT_H
