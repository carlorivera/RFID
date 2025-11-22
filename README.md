# RC522 RFID Module Test Project

This project tests the RC522 RFID module with Arduino Mega 2560.

## Wiring Connections

Connect the RC522 module to the Arduino Mega 2560 as follows:

| RC522 Pin | Arduino Mega Pin | Notes |
|-----------|------------------|-------|
| NSS       | 10               | Chip Select (can be any digital pin) |
| SCK       | 52               | SPI Clock (hardware SPI - fixed) |
| MOSI      | 51               | Master Out Slave In (hardware SPI - fixed) |
| MISO      | 50               | Master In Slave Out (hardware SPI - fixed) |
| RST       | 5                | Reset (can be any digital pin) |
| GND       | GND              | Ground |
| 3.3V      | 3.3V             | **IMPORTANT: Use 3.3V, NOT 5V!** |

## Important Notes

- **Always use 3.3V power** for the RC522 module. Using 5V can damage the module.
- **SPI pins on Arduino Mega are fixed** at pins 50, 51, 52. These cannot be changed.
- The pin numbers printed on the RC522 module (10, 11, 12, 13) are for Arduino UNO, not Mega.

## How to Use

1. Wire the module as shown above
2. Build and upload the code using PlatformIO
3. Open the Serial Monitor at 9600 baud
4. Hold an RFID card or tag near the module
5. The card UID and type will be displayed in the Serial Monitor

## What the Program Does

- Initializes the RC522 module at startup
- Displays module version information
- Continuously scans for RFID cards/tags
- When a card is detected, displays:
  - Card UID in hexadecimal format
  - Card type (MIFARE Classic, MIFARE Ultralight, etc.)
