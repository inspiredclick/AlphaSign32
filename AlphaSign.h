/*
 * AlphaSign.h
 * 
 * Main include file for AlphaSign ESP32 Library
 * 
 * This library allows ESP32 boards to control LED matrix signs
 * using the Alpha Communications Protocol over Serial2 (GPIO16/17).
 * 
 * Compatible with:
 * - Alpha American signs
 * - Betabrite signs
 * - Other Alpha Protocol compatible displays
 * 
 * Serial Configuration: 4800 baud, 7 data bits, even parity, 2 stop bits
 * 
 * Example:
 *   #include <AlphaSign.h>
 *   
 *   AlphaSign sign;
 *   
 *   void setup() {
 *     sign.begin();
 *     sign.clearMemory();
 *     
 *     sign.allocate('A', FILE_TYPE_TEXT, FILE_UNLOCKED, 64);
 *     
 *     String msg = String(COLOR_RED) + "Hello!";
 *     AlphaText text('A', msg, POS_MIDDLE_LINE, MODE_SCROLL);
 *     sign.write(text.toPacket());
 *     
 *     sign.setRunSequence("A");
 *   }
 * 
 * For detailed documentation, see README.md
 */

#ifndef ALPHASIGN_LIBRARY_H
#define ALPHASIGN_LIBRARY_H

// Core classes
#include "src/AlphaSign.h"
#include "src/AlphaPacket.h"
#include "src/AlphaText.h"
#include "src/AlphaString.h"

// Constants and definitions
#include "src/AlphaConstants.h"
#include "src/AlphaModes.h"
#include "src/AlphaColors.h"
#include "src/AlphaSpeeds.h"
#include "src/AlphaPositions.h"
#include "src/AlphaExtChars.h"
#include "src/AlphaCounters.h"

#endif // ALPHASIGN_LIBRARY_H
