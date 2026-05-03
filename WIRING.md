# Wiring Guide

## ESP32 to LED Sign Connection

### RS-232 to TTL Converter (If Sign Has RS-232 Port)

Some LED signs use RS-232 signaling with either DB9 connectors or RJ12 jacks. You'll need an RS-232 to TTL converter.

#### RS-232 with RJ12 Connector

Some older Alpha signs use RS-232 on an RJ12 (6P6C) jack:

```
┌─────────────┐    ┌──────────┐    ┌─────────────┐
│    ESP32    │    │ RS-232   │    │  RJ12 Jack  │
│             │    │ to TTL   │    │ (LED Sign)  │
│ GPIO17 (TX) ├───►┤ TTL RX   │    │             │
│ GPIO16 (RX) ◄────┤ TTL TX   │    │             │
│ GND         ├────┤ GND      │    │             │
│             │    │ RS-232 TX├───►┤ Pin 5 (RX)  │
│             │    │ RS-232 RX◄────┤ Pin 2 (TX)  │
│             │    │ GND      ├────┤ Pin 1 (GND) │
└─────────────┘    └──────────┘    └─────────────┘
```

