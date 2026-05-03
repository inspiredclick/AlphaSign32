#ifndef ALPHA_PACKET_H
#define ALPHA_PACKET_H

#include <Arduino.h>
#include "AlphaConstants.h"

class AlphaPacket {
public:
  AlphaPacket();
  AlphaPacket(const String& data, char type = DEFAULT_TYPE, const String& address = DEFAULT_ADDRESS);
  
  // Build packet with protocol format: [NUL×5][SOH][Type][Address][STX][Contents][EOT]
  String build() const;
  
  // Setters
  void setData(const String& data);
  void setType(char type);
  void setAddress(const String& address);
  
  // Getters
  String getData() const { return _data; }
  char getType() const { return _type; }
  String getAddress() const { return _address; }
  
private:
  String _data;
  char _type;
  String _address;
};

#endif // ALPHA_PACKET_H
