# Wiring Reference

## Sensors

| Component | ESP32 GPIO |
|---|---:|
| DHT22 #1 | GPIO 4 |
| DHT22 #2 | GPIO 16 |
| DHT22 #3 | GPIO 17 |
| DS18B20 #1 | GPIO 18 |
| DS18B20 #2 | GPIO 19 |
| DS18B20 #3 | GPIO 21 |

Each DS18B20 data line should use an appropriate pull-up resistor, commonly 4.7 kΩ, between DATA and 3.3 V.

## Actuators

| Actuator | ESP32 GPIO |
|---|---:|
| Peltier relay #1 | GPIO 25 |
| Peltier relay #2 | GPIO 26 |
| Peltier relay #3 | GPIO 27 |
| Fan relay #1 | GPIO 32 |
| Fan relay #2 | GPIO 33 |
| Fan relay #3 | GPIO 14 |
| Servo #1 | GPIO 13 |
| Servo #2 | GPIO 12 |
| Servo #3 | GPIO 23 |

## Important Electrical Note

ESP32 GPIO pins are control signals only. Use proper transistor/MOSFET/relay driver circuits and external supplies for Peltier modules, fans and other loads.

All grounds must be designed correctly according to the selected driver/isolation architecture.
