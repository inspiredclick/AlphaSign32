# Changelog

All notable changes to the AlphaSign ESP32 library will be documented in this file.

## [1.0.0] - 2026-01-31

### Added
- Initial release of AlphaSign ESP32 library
- Full Alpha Communications Protocol implementation
- ESP32 Serial2 support (GPIO16 RX, GPIO17 TX)
- Core classes:
  - `AlphaSign`: Main sign controller with Serial2 communication
  - `AlphaPacket`: Protocol packet builder
  - `AlphaText`: TEXT file representation
  - `AlphaString`: STRING file representation
- Display features:
  - 42 display modes including:
    - Basic modes (20): scroll, rotate, hold, flash, roll/wipe in 8 directions, automode, compressed rotate, explode, clock
    - Special effects (13): twinkle, sparkle, snow, interlock, switch, slide, spray, starburst, welcome, slot machine, news flash, trumpet animation, cycle colors
    - Graphics animations (9): thank you, no smoking, don't drink drive, running animal, fish, fireworks, turbo car, balloon, cherry bomb
  - 12 predefined colors plus custom RGB support
  - 5 speed levels
  - 6 position options
  - 22 extended characters (arrows, symbols, etc.)
  - 5 counter variables
- Sign control methods:
  - Memory management (clear, allocate single file, allocateAll for A-Z)
  - Run sequence control
  - Beep function
  - Soft reset
  - Debug mode with hex dump
- Serial configuration: 4800 baud, 7E2 (7 data bits, even parity, 2 stop bits)
- Blocking write operations for reliable communication
- Examples:
  - AlphaSign32.ino: Full web application with WiFi, OTA, REST API, and power control
  - BasicExample: Simple text display with system monitoring
  - AdvancedEffects: Multiple messages with colors and effects
  - SystemMonitor: Detailed ESP32 diagnostics and health monitoring
- System monitoring every 5 seconds:
  - Free/Total/Min heap memory
  - Heap usage percentage
  - CPU frequency
  - Flash size and usage
  - PSRAM (if available)
  - Uptime tracking
  - Loop performance metrics
  - Memory leak detection
  - Health indicators
- Documentation:
  - Comprehensive README with API reference
  - WIRING.md with connection diagrams and troubleshooting
  - QUICK_REFERENCE.md for fast lookup
  - Arduino keywords.txt for syntax highlighting
  - Library metadata (library.properties)
- Arduino Library Manager compatibility
- Web interface and connectivity:
  - WiFi connectivity with configurable credentials
  - Modern responsive web UI for sign control
  - REST API with endpoints for all sign functions
  - OTA firmware updates via web browser
  - mDNS support for .local hostname access
  - Power control via GPIO pin
  - Real-time system status dashboard
- Build configuration system:
  - .env file support for configuration
  - Automated config.h generation
  - Build script with export option
  - Git-ignored local settings
  - Configurable GPIO pins, WiFi, debug settings, intervals

### Technical Details
- Converted from Python alphasign library
- Optimized for ESP32 architecture
- Uses HardwareSerial for GPIO UART communication
- Custom GPIO pin support via begin(rxPin, txPin)
- Protocol-accurate packet formatting
- 1-second delay after clearMemory() as per protocol spec
- Max file size: 125 bytes (protocol limit)
- Debug output via Serial (USB) for monitoring

### Protocol Support
- Command codes: WRITE_TEXT, WRITE_STRING, WRITE_SPECIAL
- File types: TEXT ('A'), STRING ('B')
- Memory allocation with lock state
- Run sequence configuration
- Special functions (beep, reset, memory clear)

### Hardware Requirements
- ESP32 development board
- LED Matrix Sign with Alpha Communications Protocol
- Serial connection (default: GPIO16 RX, GPIO17 TX)
- Optional: GPIO pin for power control (default: GPIO5)
- Optional: Level shifter for 3.3V↔5V conversion
- Optional: RS-232 to TTL converter for DB9 signs

### Known Limitations
- No read operations implemented (write-only)
- No time/date functions yet (planned for future release)
- No character set switching (planned for future release)
- Blocking operations may not be suitable for real-time applications
- Web interface requires WiFi configuration

### Future Enhancements (Planned)
- Time/date display functions
- Character set selection
- Read operations for sign status
- Non-blocking async write option
- Multiple sign support

---

## Version History

**1.0.0** - Initial public release (January 31, 2026)

---

## Credits

Based on the Python alphasign library for controlling Alpha LED signs.
Converted to C++ for ESP32 by Andrew at Inspired Click.

Original Python library: https://github.com/msparks/alphasign
