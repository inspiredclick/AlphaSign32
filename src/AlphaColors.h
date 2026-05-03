#ifndef ALPHA_COLORS_H
#define ALPHA_COLORS_H

#include <Arduino.h>

// Standard colors
#define COLOR_RED       "\x1C\x31"
#define COLOR_GREEN     "\x1C\x32"
#define COLOR_AMBER     "\x1C\x33"
#define COLOR_DIM_RED   "\x1C\x34"
#define COLOR_DIM_GREEN "\x1C\x35"
#define COLOR_BROWN     "\x1C\x36"
#define COLOR_ORANGE    "\x1C\x37"
#define COLOR_YELLOW    "\x1C\x38"
#define COLOR_RAINBOW_1 "\x1C\x39"
#define COLOR_RAINBOW_2 "\x1C\x41"
#define COLOR_MIX       "\x1C\x42"
#define COLOR_AUTOCOLOR "\x1C\x43"

// Helper functions for custom RGB colors
inline String colorRGB(const char* rgb) {
  String result = "\x1CZ";
  String rgbStr = String(rgb);
  if (rgbStr[0] == '#') {
    result += rgbStr.substring(1);
  } else {
    result += rgbStr;
  }
  return result;
}

inline String colorShadowRGB(const char* rgb) {
  String result = "\x1CY";
  String rgbStr = String(rgb);
  if (rgbStr[0] == '#') {
    result += rgbStr.substring(1);
  } else {
    result += rgbStr;
  }
  return result;
}

#endif // ALPHA_COLORS_H
