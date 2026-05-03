#include "AlphaPacket.h"

AlphaPacket::AlphaPacket() 
  : _type(DEFAULT_TYPE), _address(DEFAULT_ADDRESS) {
}

AlphaPacket::AlphaPacket(const String& data, char type, const String& address)
  : _data(data), _type(type), _address(address) {
}

String AlphaPacket::build() const {
  String packet = "";
  
  // Add 5 NUL bytes as preamble
  for (int i = 0; i < 5; i++) {
    packet += (char)NUL;
  }
  
  // Add SOH (Start of Header)
  packet += (char)SOH;
  
  // Add Type Code
  packet += _type;
  
  // Add Address (2 characters)
  packet += _address;
  
  // Add STX (Start of Text)
  packet += (char)STX;
  
  // Add data/contents
  packet += _data;
  
  // Add EOT (End of Transmission)
  packet += (char)EOT;
  
  return packet;
}

void AlphaPacket::setData(const String& data) {
  _data = data;
}

void AlphaPacket::setType(char type) {
  _type = type;
}

void AlphaPacket::setAddress(const String& address) {
  _address = address;
}
