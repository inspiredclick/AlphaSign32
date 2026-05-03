# AlphaSign ESP32 - Quick Reference

## Installation
```cpp
#include <AlphaSign.h>
```

## Basic Setup
```cpp
AlphaSign sign;

void setup() {
  sign.begin();                    // Init Serial2 (GPIO16/17)
  sign.clearMemory();              // Clear all memory
  sign.allocateAll();              // Allocate all files A-Z (recommended)
  
  String msg = String(COLOR_RED) + "Hello!";
  AlphaText text('A', msg, POS_MIDDLE_LINE, MODE_SCROLL);
  sign.write(text.toPacket());
  sign.setRunSequence("A");
}
```

## Colors
```cpp
COLOR_RED          COLOR_GREEN        COLOR_AMBER
COLOR_YELLOW       COLOR_ORANGE       COLOR_BROWN
COLOR_DIM_RED      COLOR_DIM_GREEN
COLOR_RAINBOW_1    COLOR_RAINBOW_2
COLOR_MIX          COLOR_AUTOCOLOR

// Custom RGB
colorRGB("FF0000")           // Red
colorRGB("#00FF00")          // Green
colorShadowRGB("0000FF")     // Blue shadow
```

## Display Modes (42 Total)
```cpp
// Basic (20)
MODE_ROTATE        MODE_HOLD          MODE_FLASH         MODE_SCROLL
MODE_ROLL_UP       MODE_ROLL_DOWN     MODE_ROLL_LEFT     MODE_ROLL_RIGHT
MODE_WIPE_UP       MODE_WIPE_DOWN     MODE_WIPE_LEFT     MODE_WIPE_RIGHT
MODE_ROLL_IN       MODE_ROLL_OUT      MODE_WIPE_IN       MODE_WIPE_OUT
MODE_AUTOMODE      MODE_COMPRESSED_ROTATE  MODE_EXPLODE  MODE_CLOCK

// Special Effects (13)
MODE_TWINKLE       MODE_SPARKLE       MODE_SNOW          MODE_INTERLOCK
MODE_SWITCH        MODE_SLIDE         MODE_SPRAY         MODE_STARBURST
MODE_WELCOME       MODE_SLOT_MACHINE  MODE_NEWS_FLASH
MODE_TRUMPET_ANIMATION  MODE_CYCLE_COLORS

// Graphics Animations (9)
MODE_THANK_YOU     MODE_NO_SMOKING    MODE_DONT_DRINK_DRIVE
MODE_RUNNING_ANIMAL  MODE_FISH_ANIMATION  MODE_FIREWORKS
MODE_TURBO_CAR     MODE_BALLOON_ANIMATION  MODE_CHERRY_BOMB
```

## Speeds
```cpp
SPEED_1    // Slowest
SPEED_2
SPEED_3
SPEED_4
SPEED_5    // Fastest
```

## Positions
```cpp
POS_MIDDLE_LINE    POS_TOP_LINE       POS_BOTTOM_LINE
POS_FILL           POS_LEFT           POS_RIGHT
```

## Extended Characters
```cpp
EXT_HEART          EXT_CAR            EXT_TELEPHONE
EXT_UP_ARROW       EXT_DOWN_ARROW     EXT_LEFT_ARROW    EXT_RIGHT_ARROW
EXT_PACMAN         EXT_SAIL_BOAT      EXT_BALL
EXT_MUSICAL_NOTE   EXT_INFINITY_SYMBOL
EXT_COPYRIGHT_SYMBOL EXT_MALE_SYMBOL  EXT_FEMALE_SYMBOL
```

## Counters
```cpp
COUNTER_1          COUNTER_2          COUNTER_3
COUNTER_4          COUNTER_5
```

## AlphaSign Methods
```cpp
sign.begin();                                    // Init with default pins
sign.begin(rxPin, txPin);                       // Init with custom pins
sign.write(packet);                             // Send packet
sign.write(data);                               // Send raw data
sign.clearMemory();                             // Clear all (1s delay)
sign.allocate(label, type, lock, size);        // Allocate single file
sign.allocateAll();                             // Allocate all A-Z (recommended)
sign.setRunSequence("ABC");                     // Set display order
sign.beep(freq, duration, repeat);             // Beep
sign.softReset();                               // Soft reset
sign.setDebug(true);                            // Enable debug
```

## AlphaText
```cpp
AlphaText text('A', data, position, mode);
text.toPacket();                                // Get packet
text.setLabel('B');                             // Change label
text.setData("new message");                    // Change data
text.setPosition(POS_TOP_LINE);                // Change position
text.setMode(MODE_SPARKLE);                     // Change mode
```

## AlphaString
```cpp
AlphaString str('1', data);
str.toPacket();                                 // Get packet
str.call();                                     // Get call sequence "\x10[label]"

// Use in text
String msg = "Hello " + str.call() + "!";      // Inserts string content
```

## Common Patterns

### Simple Message
```cpp
sign.clearMemory();
sign.allocateAll();  // Allocate all files A-Z in one packet
AlphaText text('A', "Hello World", POS_MIDDLE_LINE, MODE_SCROLL);
sign.write(text.toPacket());
sign.setRunSequence("A");
```

### Colored Message
```cpp
String msg = String(COLOR_RED) + String(SPEED_4) + "Fast Red Text";
AlphaText text('A', msg);
```

### Multiple Messages
```cpp
sign.allocateAll();  // Allocate all files A-Z

AlphaText text1('A', "Message 1");
AlphaText text2('B', "Message 2");

sign.write(text1.toPacket());
sign.write(text2.toPacket());

sign.setRunSequence("AB");  // Display A then B
```

### With Extended Characters
```cpp
String msg = String(EXT_HEART) + " Love " + String(EXT_HEART);
AlphaText text('A', msg);
```

### Custom RGB Color
```cpp
String msg = colorRGB("FF00FF") + "Purple Text";
AlphaText text('A', msg);
```

### Reusable String
```cpp
// Create string file
sign.allocate('1', FILE_TYPE_STRING, FILE_UNLOCKED, 30);
AlphaString counter('1', "Count: 42");
sign.write(counter.toPacket());

// Use in text
String msg = "Status: " + counter.call();
AlphaText text('A', msg);
```

## Protocol Format
```
Packet: [NUL×5][SOH][Type][Address][STX][Contents][EOT]
Serial: 4800 baud, 7 data bits, even parity, 2 stop bits (SERIAL_7E2)
```

## GPIO Pins
```
Default: GPIO16 (RX), GPIO17 (TX)
Custom:  Any GPIO except 6-11, 34-39
```

## Debugging
```cpp
sign.setDebug(true);
// Serial monitor shows:
// - Initialization info
// - Packet hex dumps
// - Operation confirmations
```

## File Limits
```
Max file size: 125 bytes
Label: Single character ('A'-'Z', '0'-'9')
File types: FILE_TYPE_TEXT, FILE_TYPE_STRING
Lock: FILE_LOCKED, FILE_UNLOCKED
```

## System Monitoring (Every 5 seconds)
```cpp
unsigned long lastMonitor = 0;
const unsigned long INTERVAL = 5000;

void loop() {
  if (millis() - lastMonitor >= INTERVAL) {
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());
    lastMonitor = millis();
  }
}
```

## Troubleshooting

**No response:**
- Check TX/RX crossover
- Verify 7E2 serial config
- Enable debug mode
- Check GND connection

**Garbled display:**
- Confirm 7E2 mode
- Check cable quality
- Verify protocol support

**Memory issues:**
- Max 125 bytes per file
- Call clearMemory() first
- Monitor heap usage

## Examples
- **AlphaSign32.ino**: Full web interface with WiFi, OTA, REST API
- **BasicExample**: Simple text with monitoring
- **AdvancedEffects**: Multiple messages, colors, modes
- **SystemMonitor**: Detailed ESP32 diagnostics

## Web Interface (AlphaSign32.ino)

### Configuration
```bash
# Copy and edit .env file
cp .env.example .env

# Set WiFi credentials
WIFI_SSID=YourNetwork
WIFI_PASSWORD=YourPassword

# Build and upload
./build.sh
```

### Access
- By IP: `http://YOUR_ESP32_IP`
- By hostname: `http://alphasign32.local`
- OTA updates: `http://alphasign32.local/ota`

### REST API Endpoints
```bash
# Send message
curl -X POST http://alphasign32.local/update \
  -d "message=Hello" -d "file=A" -d "mode=scroll" -d "color=red"

# Toggle power
curl -X POST http://alphasign32.local/power/toggle

# Get status (JSON)
curl http://alphasign32.local/status

# Allocate all files
curl -X POST http://alphasign32.local/files/init

# Clear memory
curl -X POST http://alphasign32.local/memory/clear

# Set run sequence to A-Z
curl -X POST http://alphasign32.local/runsequence/reset
```

### Configuration Variables (.env)
```
ALPHASIGN_RX_PIN=16           # RX GPIO pin
ALPHASIGN_TX_PIN=17           # TX GPIO pin
POWER_CONTROL_PIN=5           # Power control GPIO
DEBUG_SERIAL_BAUD=115200      # USB serial speed
ENABLE_DEBUG=false            # Enable packet debug
MONITOR_INTERVAL_MS=5000      # Monitor interval
WIFI_SSID=YourNetwork         # WiFi SSID
WIFI_PASSWORD=YourPassword    # WiFi password
OTA_HOSTNAME=alphasign32      # mDNS hostname
```
