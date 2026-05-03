# Getting Started with AlphaSign ESP32

## Complete Python to C++ ESP32 Conversion - Successfully Completed! ✓

The Python alphasign library has been fully converted to a C++ library for ESP32, replacing serial/COM port communication with GPIO-based UART using Serial2 on pins GPIO16 (RX) and GPIO17 (TX).

---

## 📁 Library Structure

```
AlphaSign/
├── AlphaSign.h              # Main library include file
├── README.md                # Full documentation
├── QUICK_REFERENCE.md       # Fast API lookup
├── WIRING.md                # Connection diagrams
├── CHANGELOG.md             # Version history
├── library.properties       # Arduino library metadata
├── keywords.txt             # Syntax highlighting
│
├── src/                     # Core library files
│   ├── AlphaSign.h/cpp      # Main controller class
│   ├── AlphaPacket.h/cpp    # Protocol packet builder
│   ├── AlphaText.h/cpp      # TEXT file representation
│   ├── AlphaString.h/cpp    # STRING file representation
│   ├── AlphaConstants.h     # Protocol constants
│   ├── AlphaModes.h         # 26 display modes
│   ├── AlphaColors.h        # Color definitions + RGB
│   ├── AlphaSpeeds.h        # 5 speed levels
│   ├── AlphaPositions.h     # Position constants
│   ├── AlphaExtChars.h      # Extended characters
│   └── AlphaCounters.h      # Counter variables
│
└── examples/                # Example sketches
    ├── BasicExample/        # Simple text with monitoring
    ├── AdvancedEffects/     # Multiple messages & effects
    └── SystemMonitor/       # Detailed ESP32 diagnostics
```

---

## 🚀 Quick Start (5 Minutes)

### Step 1: Install Library

**Option A: Arduino Library Manager** (when published)
```
Sketch → Include Library → Manage Libraries → Search "AlphaSign"
```

**Option B: Manual Installation** (for now)
```bash
cd ~/Documents/Arduino/libraries/
cp -r /Users/andrew/inspiredclick/alphasign/esp32/AlphaSign .
```

Restart Arduino IDE.

### Step 2: Wire Your ESP32

```
ESP32          LED Sign
GPIO17 (TX) → RX (Data In)
GPIO16 (RX) → TX (Data Out)
GND         → GND
```

See [WIRING.md](WIRING.md) for detailed diagrams.

### Step 3: Upload BasicExample

1. Open Arduino IDE
2. File → Examples → AlphaSign → BasicExample
3. Select your ESP32 board: Tools → Board → ESP32 Arduino → [Your Board]
4. Select port: Tools → Port → [Your ESP32 Port]
5. Click Upload ⬆️

### Step 4: Open Serial Monitor

1. Tools → Serial Monitor
2. Set baud rate to **115200**
3. You should see:
   - Initialization messages
   - System status every 5 seconds
   - Memory and CPU stats

---

## 🌐 Web Interface Setup (AlphaSign32.ino)

The main sketch includes a full-featured web application for controlling your sign remotely!

### Features
- Modern responsive web UI
- Real-time system monitoring
- REST API for automation
- OTA firmware updates
- Power control via GPIO
- mDNS support (access at http://alphasign32.local)

### Setup Steps

1. **Configure WiFi:**
   ```bash
   cp .env.example .env
   ```
   
   Edit `.env` and set your WiFi credentials:
   ```
   WIFI_SSID=YourNetworkName
   WIFI_PASSWORD=YourPassword
   OTA_HOSTNAME=alphasign32
   ```

2. **Build and Upload:**
   ```bash
   ./build.sh
   ```
   
   The script will:
   - Generate `config.h` from your `.env` settings
   - Compile the firmware
   - Upload to your ESP32

3. **Find Your Device:**
   
   Watch the Serial Monitor (115200 baud) for:
   ```
   WiFi connected!
   IP Address: 192.168.1.XXX
   mDNS responder started: http://alphasign32.local
   Web server started!
   ```

4. **Access the Web Interface:**
   
   Open in your browser:
   - By hostname: `http://alphasign32.local`
   - By IP: `http://192.168.1.XXX`

### Web Interface Features

**Sign Control Tab:**
- Send messages to any file (A-Z)
- Select display mode (42 modes available)
- Choose color (red, green, amber, orange, yellow, rainbow)
- Set animation speed (1-5)
- Real-time status display

**Power Control:**
- Toggle sign power on/off via GPIO
- Visual power state indicator

**Memory Management:**
- Initialize all files (A-Z) with one click
- Clear all memory
- Set run sequence

**System Status:**
- IP address and WiFi signal strength
- System uptime
- Free heap memory
- Auto-refresh every 5 seconds

**OTA Updates:**
- Upload new firmware via web browser
- No USB cable needed after initial setup
- Safe update process with verification

### REST API

Control your sign programmatically:

```bash
# Send a scrolling message
curl -X POST http://alphasign32.local/update \
  -d "message=Hello World" \
  -d "file=A" \
  -d "mode=scroll" \
  -d "color=red" \
  -d "speed=3"

# Toggle power
curl -X POST http://alphasign32.local/power/toggle

# Get system status (JSON)
curl http://alphasign32.local/status

# Initialize all files
curl -X POST http://alphasign32.local/files/init

# Clear memory
curl -X POST http://alphasign32.local/memory/clear

# Set run sequence
curl -X POST http://alphasign32.local/runsequence/reset
```

### Available Display Modes (Web UI)
- Basic: hold, rotate, flash, scroll, automode, clock
- Directional: rollup, rolldown, rollleft, rollright, wipeup, wipedown, wipeleft, wiperight
- Advanced: rollin, rollout, wipein, wipeout, compressedrotate, explode
- Effects: twinkle, sparkle, snow, interlock, switch, slide, spray, starburst, welcome, slotmachine, newsflash, trumpetanimation, cyclecolors
- Graphics: thankyou, nosmoking, dontdrinkdrive, runninganimal, fireworks, turbocar, cherrybomb

### Power Control Configuration

The web interface can control sign power via a GPIO pin (default: GPIO5). Wire to a relay or MOSFET:

```
ESP32 GPIO5 → Relay/MOSFET Control Pin
Relay/MOSFET → Sign Power Supply
```

Configure in `.env`:
```
POWER_CONTROL_PIN=5
```

### OTA Firmware Updates

1. Access OTA page: `http://alphasign32.local/ota`
2. Click "Choose File" and select your compiled `.bin` file
3. Click "Update Firmware"
4. Wait for upload and automatic reboot

To generate a `.bin` file:
```bash
./build.sh --export
```

The compiled binary will be in the `build/` directory.

---

## ⚙️ Configuration (Optional)

The library supports build-time configuration through a `.env` file. This allows you to customize GPIO pins, debug settings, and timing without editing code.

### Available Configuration Options

| Variable | Default | Description |
|----------|---------|-------------|
| `ALPHASIGN_RX_PIN` | 16 | GPIO pin for receiving data from sign |
| `ALPHASIGN_TX_PIN` | 17 | GPIO pin for transmitting data to sign |
| `DEBUG_SERIAL_BAUD` | 115200 | Baud rate for USB Serial debugging |
| `ENABLE_DEBUG` | false | Enable/disable packet debug output |
| `MONITOR_INTERVAL_MS` | 5000 | System monitor update interval (ms) |
| `SERIAL_TIMEOUT_MS` | 3000 | Serial connection timeout on startup (ms) |

### How to Configure

1. **Copy the example configuration:**
   ```bash
   cp .env.example .env
   ```

2. **Edit `.env` with your values:**
   ```bash
   # Example: Use different GPIO pins
   ALPHASIGN_RX_PIN=18
   ALPHASIGN_TX_PIN=19
   
   # Enable debug output
   ENABLE_DEBUG=true
   ```

3. **Build using build.sh:**
   ```bash
   ./build.sh
   ```
   
   The script automatically generates `config.h` from your `.env` settings before compilation.

### Notes

- **`.env` is optional** - if not present, defaults from `.env.example` are used
- **`config.h` is auto-generated** - never edit it manually
- **Git ignores** `.env` and `config.h` - your personal settings stay local
- **Arduino IDE users**: Create `.env` in the sketch folder, then use `build.sh` for compilation

---

## 📝 Your First Custom Program

```cpp
#include <AlphaSign.h>

AlphaSign sign;

void setup() {
  Serial.begin(115200);
  
  // Initialize sign
  sign.begin();
  
  // Clear memory
  sign.clearMemory();
  
  // Allocate 64 bytes for text file 'A'
  sign.allocate('A', FILE_TYPE_TEXT, FILE_UNLOCKED, 64);
  
  // Create red scrolling message
  String msg = String(COLOR_RED) + String(SPEED_3) + "Hello ESP32!";
  AlphaText text('A', msg, POS_MIDDLE_LINE, MODE_SCROLL);
  
  // Send to sign
  sign.write(text.toPacket());
  
  // Display it
  sign.setRunSequence("A");
  
  Serial.println("Message sent!");
}

void loop() {
  // Log memory every 5 seconds
  static unsigned long lastLog = 0;
  if (millis() - lastLog >= 5000) {
    Serial.print("Free Heap: ");
    Serial.print(ESP.getFreeHeap());
    Serial.println(" bytes");
    lastLog = millis();
  }
  
  delay(100);
}
```

Save as `MyFirstSign.ino` and upload!

---

## 🎨 Key Features Converted from Python

### ✅ Core Protocol
- [x] Packet formatting (5×NUL + SOH + Type + Address + STX + Data + EOT)
- [x] 4800 baud, 7E2 serial configuration
- [x] Command codes (WRITE_TEXT, WRITE_STRING, WRITE_SPECIAL)

### ✅ Memory Management
- [x] Clear memory (with 1-second delay)
- [x] Allocate files (TEXT and STRING types)
- [x] Allocate all files A-Z in single packet (allocateAll)
- [x] Lock/unlock files
- [x] Run sequence control

### ✅ Display Features
- [x] 42 display modes (scroll, rotate, sparkle, snow, fireworks, and many more)
- [x] 12 predefined colors
- [x] Custom RGB colors
- [x] 5 speed levels
- [x] 6 position options

### ✅ Extended Features
- [x] 22 extended characters (arrows, symbols, etc.)
- [x] 5 counter variables
- [x] Beep function
- [x] Soft reset

### ✅ ESP32 Specific
- [x] Serial2 GPIO communication (GPIO16/17)
- [x] Custom GPIO pin support
- [x] Debug mode with hex dumps
- [x] System monitoring (memory, CPU, flash)
- [x] Blocking write operations
- [x] Arduino library format

### ✅ Web Interface & Connectivity
- [x] WiFi connectivity
- [x] Web-based control panel
- [x] REST API for remote control
- [x] OTA firmware updates
- [x] mDNS support (.local hostname)
- [x] Power control via GPIO
- [x] Real-time system status

### ✅ Build System
- [x] .env configuration files
- [x] Automated config.h generation
- [x] Build script with export option
- [x] Git-ignored local settings

### 📅 Future Enhancements
- [ ] Time/date display functions
- [ ] Character set switching
- [ ] Read operations
- [ ] Non-blocking async mode

---

## 🔧 System Monitoring (Every 5 Seconds)

All examples include automatic monitoring:

```
===== SYSTEM STATUS =====
Uptime: 0h 5m 23s
Free Heap: 285432 bytes
Total Heap: 327680 bytes
Min Free Heap: 285432 bytes
Heap Usage: 12.9%
CPU Frequency: 240 MHz
Flash Size: 4 MB
=========================
```

---

## 🎯 Example Projects

### 1. AlphaSign32.ino (Main Sketch)
**What it does:**
- Full web application with WiFi connectivity
- Modern responsive web UI for sign control
- REST API for automation and integration
- OTA firmware updates
- Power control via GPIO
- Real-time system monitoring dashboard
- mDNS support (http://alphasign32.local)

**Perfect for:**
- Production deployments
- Remote sign control
- Home automation integration
- Multiple user access
- Firmware updates without USB

### 2. BasicExample
**What it does:**
- Displays "Hello ESP32!" in red
- Logs system stats every 5 seconds

**Perfect for:**
- Testing your hardware connection
- Learning the basic API
- Monitoring system health

### 3. AdvancedEffects
**What it does:**
- Rotates through 4 different messages
- Uses colors, speeds, modes
- Includes extended characters
- Changes message every 10 seconds

**Perfect for:**
- Learning advanced features
- Seeing all effects in action
- Multi-message displays

### 4. SystemMonitor
**What it does:**
- Comprehensive ESP32 diagnostics
- Memory leak detection
- Performance metrics
- Health indicators

**Perfect for:**
- Debugging issues
- Long-term stability testing
- Resource optimization

---

## 📚 Documentation Files

| File | Purpose |
|------|---------|
| [README.md](README.md) | Complete API reference and documentation |
| [QUICK_REFERENCE.md](QUICK_REFERENCE.md) | Fast lookup for common tasks |
| [WIRING.md](WIRING.md) | Connection diagrams and troubleshooting |
| [CHANGELOG.md](CHANGELOG.md) | Version history and updates |

---

## 🐛 Troubleshooting

### Sign Not Responding
```cpp
// Enable debug mode to see packets
sign.setDebug(true);
```

Check Serial Monitor for hex dumps showing exactly what's being sent.

### Garbled Display
Verify 7E2 configuration:
```cpp
// Should be 7 data bits, even parity, 2 stop bits
Serial2.begin(4800, SERIAL_7E2, 16, 17);
```

### Memory Issues
Monitor heap usage:
```cpp
Serial.print("Free Heap: ");
Serial.println(ESP.getFreeHeap());
```

See [WIRING.md](WIRING.md#troubleshooting) for complete troubleshooting guide.

---

## 💡 Tips & Best Practices

1. **Always call clearMemory()** before allocating new files
2. **Wait for the 1-second delay** after clearMemory() - it's required by the protocol
3. **Enable debug mode** during development: `sign.setDebug(true)`
4. **Monitor memory usage** to detect leaks early
5. **Use the examples** as templates for your projects
6. **Check QUICK_REFERENCE.md** for fast API lookup
7. **Test with BasicExample first** before writing custom code

---

## 🎓 Learning Path

1. ✅ **Start**: Upload BasicExample → Verify connection works
2. ✅ **Practice**: Modify message text and color
3. ✅ **Explore**: Try AdvancedEffects → See all modes
4. ✅ **Monitor**: Run SystemMonitor → Understand diagnostics
5. ✅ **Create**: Write your own custom display logic
6. ✅ **Optimize**: Use monitoring to ensure stability

---

## 🔗 Additional Resources

- **Alpha Communications Protocol**: Industry standard for LED signs
- **ESP32 Documentation**: https://docs.espressif.com/
- **Arduino Reference**: https://www.arduino.cc/reference/

---

## ✨ Conversion Complete!

The Python library has been successfully converted to C++ for ESP32 with:

✓ Full protocol implementation  
✓ GPIO UART communication (Serial2)  
✓ All display modes and features  
✓ System monitoring every 5 seconds  
✓ Blocking reliable communication  
✓ Three comprehensive examples  
✓ Complete documentation  
✓ Arduino library format  

**You're ready to control LED signs from ESP32!** 🎉

Start with BasicExample and explore from there. Happy coding!

---

*Library created: January 31, 2026*  
*Converted by: Andrew @ Inspired Click*
