#ifndef ALPHA_STRING_H
#define ALPHA_STRING_H

#include <Arduino.h>
#include "AlphaPacket.h"
#include "AlphaConstants.h"

class AlphaString {
public:
  AlphaString();
  AlphaString(char label, const String& data);
  
  // Generate packet for writing this string file to sign
  AlphaPacket toPacket() const;
  
  // Get call sequence to insert this string into text
  String call() const;
  
  // Setters
  void setLabel(char label) { _label = label; }
  void setData(const String& data) { _data = data; }
  void setSize(uint8_t size) { _size = size; }
  
  // Getters
  char getLabel() const { return _label; }
  String getData() const { return _data; }
  uint8_t getSize() const { return _size; }
  
  // Calculate size automatically from data
  uint8_t calculateSize() const;
  
private:
  char _label;
  String _data;
  uint8_t _size;
};

#endif // ALPHA_STRING_H
