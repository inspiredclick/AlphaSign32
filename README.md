# AlphaSign ESP32 Library

ESP32 C++ library for controlling LED matrix signs (Alpha American, Betabrite) using the Alpha Communications Protocol over UART/GPIO.

## Features

### Protocol & Hardware
- **Full Alpha Communications Protocol** implementation
- **ESP32 optimized** with Serial2 (GPIO16 RX, GPIO17 TX)
- **42 display modes** including scroll, rotate, sparkle, snow, fireworks, and many more
- **12 predefined colors** plus custom RGB support
- **5 speed levels** for animation control
- **Extended characters** (arrows, heart, symbols, etc.)
- **Text and String files** with memory allocation
- **Run sequences** for multi-message displays
- **Blocking calls** for reliable communication
- **Debug mode** with detailed packet logging

### Web Interface & Connectivity
- **WiFi web interface** with modern responsive UI
- **REST API** for remote sign control
- **OTA firmware updates** via web browser
- **mDNS support** (access at http://alphasign32.local)
- **Real-time system monitoring** via web dashboard
- **Power control** via GPIO pin

### Development & Monitoring
- **Build configuration system** via .env files
- **System monitoring** with memory/CPU tracking
- **Multiple examples** from basic to advanced

## Hardware Requirements

- ESP32 development board
- LED Matrix Sign with Alpha Communications Protocol support
  - Alpha American signs
  - Betabrite signs
  - Compatible models
- Serial connection: 4800 baud, 7 data bits, even parity, 2 stop bits (7E2)
- Optional: GPIO pin for power control (default GPIO5)

## Wiring

### Basic Connection
```
ESP32          LED Sign
GPIO16 (RX) -> TX
GPIO17 (TX) -> RX
GND         -> GND
```

### Optional Power Control
```
ESP32          Relay/MOSFET
GPIO5       -> Control Input
```

## Installation

### Arduino Library Manager (Recommended)
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries**
3. Search for "AlphaSign"
4. Click Install

### Manual Installation
1. Download this repository as ZIP
2. Open Arduino IDE
3. Go to **Sketch > Include Library > Add .ZIP Library**
4. Select the downloaded ZIP file

## Quick Start

### Basic Usage (Library Only)

```cpp
#include <AlphaSign.h>
#include <AlphaText.h>
#include <AlphaModes.h>
#include <AlphaColors.h>

AlphaSign sign;

void setup() {
  Serial.begin(115200);
  
  // Initialize sign communication
  sign.begin();
  
  // Clear memory
  sign.clearMemory();
  
  // Allocate all files A-Z in one packet (recommended)
  sign.allocateAll();
  
  // Create and send message
  String msg = String(COLOR_RED) + "Hello World!";
  AlphaText text('A', msg, POS_MIDDLE_LINE, MODE_SCROLL);
  sign.write(text.toPacket());
  
  // Display the message
  sign.setRunSequence("A");
}

void loop() {
  // Your code here
}
```

### Web Interface Setup

1. **Configure WiFi** - Copy `.env.example` to `.env` and set your WiFi credentials:
   ```bash
   WIFI_SSID=YourNetworkName
   WIFI_PASSWORD=YourPassword
   ```

2. **Build and upload** using the provided build script:
   ```bash
   ./build.sh
   ```

3. **Access the web interface**:
   - By IP: http://YOUR_ESP32_IP
   - By hostname: http://alphasign32.local

4. **Control your sign** via the web dashboard:
   - Send messages with different colors, modes, and effects
   - Toggle power on/off
   - Allocate memory and manage files
   - Monitor system status
   - Upload firmware updates (OTA)
```

## Examples

### AlphaSign32.ino (Main Sketch)
Full-featured web application with:
- WiFi connectivity and mDNS
- Web-based control panel
- REST API for all sign functions
- OTA firmware updates
- Power control via GPIO
- System monitoring dashboard

### BasicExample
Simple text display with system monitoring every 5 seconds.

### AdvancedEffects
Multiple messages with different colors, modes, and effects. Demonstrates:
- Rainbow colors
- Extended characters
- Multiple text files
- Run sequences
- String files
- System resource monitoring

### SystemMonitor
Comprehensive ESP32 diagnostics including:
- Memory leak detection
- Performance metrics
- Health indicators

## API Reference

### AlphaSign Class

#### Initialization
```cpp
AlphaSign sign;
sign.begin();                    // Use default GPIO16/17
sign.begin(rxPin, txPin);        // Use custom pins
```

#### Memory Management
```cpp
sign.clearMemory();              // Clear all sign memory (1s delay)
sign.allocate(label, type, lock, size);  // Allocate single file
sign.allocateAll();              // Allocate all files A-Z in one packet (recommended)
```

#### Communication
```cpp
sign.write(packet);              // Write packet to sign
sign.write(data);                // Write raw data string
```

#### Control
```cpp
sign.setRunSequence("ABC");      // Set display order
sign.beep(freq, duration, repeat); // Sound beeper
sign.softReset();                // Non-destructive reset
sign.setDebug(true);             // Enable debug output
```

### AlphaText Class

```cpp
AlphaText text(label, data, position, mode);
text.toPacket();                 // Generate protocol packet
text.setLabel('A');              // Set file label
text.setData("message");         // Set message text
text.setPosition(POS_MIDDLE_LINE); // Set position
text.setMode(MODE_SCROLL);       // Set display mode
```

### AlphaString Class

```cpp
AlphaString str(label, data);
str.toPacket();                  // Generate protocol packet
str.call();                      // Get insertion sequence
```

### Display Modes (42 Total)

#### Basic Modes (20)
```cpp
MODE_ROTATE, MODE_HOLD, MODE_FLASH, MODE_SCROLL
MODE_ROLL_UP, MODE_ROLL_DOWN, MODE_ROLL_LEFT, MODE_ROLL_RIGHT
MODE_WIPE_UP, MODE_WIPE_DOWN, MODE_WIPE_LEFT, MODE_WIPE_RIGHT
MODE_ROLL_IN, MODE_ROLL_OUT, MODE_WIPE_IN, MODE_WIPE_OUT
MODE_AUTOMODE, MODE_COMPRESSED_ROTATE, MODE_EXPLODE, MODE_CLOCK
```

#### Special Effects (13)
```cpp
MODE_TWINKLE, MODE_SPARKLE, MODE_SNOW, MODE_INTERLOCK
MODE_SWITCH, MODE_SLIDE, MODE_SPRAY, MODE_STARBURST
MODE_WELCOME, MODE_SLOT_MACHINE, MODE_NEWS_FLASH
MODE_TRUMPET_ANIMATION, MODE_CYCLE_COLORS
```

#### Graphics Animations (9)
```cpp
MODE_THANK_YOU, MODE_NO_SMOKING, MODE_DONT_DRINK_DRIVE
MODE_RUNNING_ANIMAL, MODE_FISH_ANIMATION, MODE_FIREWORKS
MODE_TURBO_CAR, MODE_BALLOON_ANIMATION, MODE_CHERRY_BOMB
```

### Colors

```cpp
COLOR_RED, COLOR_GREEN, COLOR_AMBER, COLOR_YELLOW
COLOR_ORANGE, COLOR_RAINBOW_1, COLOR_RAINBOW_2
COLOR_DIM_RED, COLOR_DIM_GREEN, COLOR_BROWN
COLOR_MIX, COLOR_AUTOCOLOR

// Custom RGB colors
colorRGB("FF0000");              // Bright red
colorRGB("#00FF00");             // Bright green
colorShadowRGB("0000FF");        // Blue shadow
```

### Speeds

```cpp
SPEED_1  // Slowest
SPEED_2
SPEED_3
SPEED_4
SPEED_5  // Fastest
```

### Positions

```cpp
POS_MIDDLE_LINE, POS_TOP_LINE, POS_BOTTOM_LINE
POS_FILL, POS_LEFT, POS_RIGHT
```

### Extended Characters

```cpp
EXT_HEART, EXT_ARROW_UP, EXT_ARROW_DOWN, EXT_ARROW_LEFT, EXT_ARROW_RIGHT
EXT_PACMAN, EXT_CAR, EXT_TELEPHONE, EXT_MUSICAL_NOTE
EXT_COPYRIGHT_SYMBOL, EXT_INFINITY_SYMBOL
// ... and more
```

## Web Interface & REST API

### REST API Endpoints

The main sketch includes a full REST API for remote control:

#### Sign Control
- `POST /update` - Update sign message
  - Parameters: `message`, `file` (A-Z), `mode`, `color`, `speed`
- `POST /files/init` - Allocate all files A-Z
- `POST /memory/clear` - Clear all sign memory
- `POST /runsequence/reset` - Set run sequence to A-Z

#### Power Control
- `POST /power/toggle` - Toggle sign power on/off
- `GET /power/state` - Get current power state

#### System Status
- `GET /status` - Get system status (JSON)
  - Returns: IP, RSSI, uptime, free heap

#### OTA Updates
- `GET /ota` - OTA upload page
- `POST /ota` - Upload firmware binary

### Example API Usage

```bash
# Send a message to the sign
curl -X POST http://alphasign32.local/update \
  -d "message=Hello World" \
  -d "file=A" \
  -d "mode=scroll" \
  -d "color=red" \
  -d "speed=3"

# Toggle power
curl -X POST http://alphasign32.local/power/toggle

# Get system status
curl http://alphasign32.local/status
```

## Build Configuration

The project supports build-time configuration via `.env` files:

### Available Options

| Variable | Default | Description |
|----------|---------|-------------|
| `ALPHASIGN_RX_PIN` | 16 | GPIO pin for RX |
| `ALPHASIGN_TX_PIN` | 17 | GPIO pin for TX |
| `POWER_CONTROL_PIN` | 5 | GPIO pin for power control |
| `DEBUG_SERIAL_BAUD` | 115200 | USB serial baud rate |
| `ENABLE_DEBUG` | false | Enable packet debugging |
| `MONITOR_INTERVAL_MS` | 5000 | System monitor interval |
| `SERIAL_TIMEOUT_MS` | 3000 | Serial connection timeout |
| `WIFI_SSID` | - | WiFi network name |
| `WIFI_PASSWORD` | - | WiFi password |
| `OTA_HOSTNAME` | alphasign32 | mDNS hostname |

### Configuration Steps

1. Copy `.env.example` to `.env`
2. Edit `.env` with your values
3. Run `./build.sh` to generate config and compile

The build script automatically generates `config.h` from your `.env` settings.

## System Monitoring

Both example sketches include system monitoring that logs every 5 seconds:
- Uptime
- Free/Total/Min heap memory
- Heap usage percentage
- CPU frequency
- Flash size and speed
- PSRAM (if available)
- Sketch size

## Protocol Details

The library implements the Alpha Communications Protocol:

**Packet Format:** `[NUL×5][SOH][Type][Address][STX][Contents][EOT]`

**Serial Configuration:**
- Baud rate: 4800
- Data bits: 7
- Parity: Even
- Stop bits: 2
- Mode: SERIAL_7E2

## Troubleshooting

### Sign not responding
- Check wiring (TX/RX crossover)
- Verify 4800 baud, 7E2 configuration
- Enable debug mode: `sign.setDebug(true)`
- Check Serial monitor for packet hex dumps

### Memory issues
- Reduce message sizes (max 125 bytes per file)
- Call `clearMemory()` before allocating
- Monitor heap usage in examples

### Garbled display
- Verify serial configuration (7E2 is critical)
- Check for loose connections
- Ensure sign supports Alpha Communications Protocol

## License

Converted from Python alphasign library. See original project for license details.

## Credits

Based on the Python alphasign library for controlling Alpha LED signs.
Converted to C++ for ESP32 by Andrew at Inspired Click.

## Support

For issues and questions, please open an issue on the GitHub repository.
