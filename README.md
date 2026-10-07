# IoT-Based Smart Multi-Division Cold Storage Monitoring and Environmental Control System

An ESP32-based IoT prototype for monitoring and controlling temperature and humidity in multiple cold-storage divisions. The prototype is designed for separate storage conditions for **fruits & vegetables, milk products, and vaccines**.

## Features

- Multi-division cold-storage monitoring
- ESP32 as the main controller
- DHT22 temperature/humidity sensing
- DS18B20 temperature sensing
- TEC1 Peltier cooling control
- Fan control
- Relay-based actuator switching
- SG90 servo-based airflow/valve control
- Serial monitoring
- Threshold-based automatic environmental control
- Modular code structure for extending the system to MQTT/cloud monitoring

## System Architecture

```text
                    +----------------------+
                    |       ESP32          |
                    |   Main Controller    |
                    +----------+-----------+
                               |
          +--------------------+--------------------+
          |                    |                    |
       Division 1           Division 2           Division 3
   Fruits & Vegetables    Milk Products          Vaccines
          |                    |                    |
      DHT22/DS18B20       DHT22/DS18B20       DHT22/DS18B20
          |                    |                    |
       Peltier + Fan       Peltier + Fan       Peltier + Fan
          |                    |                    |
       Relay Control        Relay Control        Relay Control
          |                    |                    |
             +------------- SG90 Servo ------------+
                         Airflow Control
```

## Hardware

| Component | Purpose |
|---|---|
| ESP32 | Main controller |
| DHT22 | Temperature and humidity monitoring |
| DS18B20 | Accurate temperature measurement |
| TEC1 Peltier | Cooling |
| Relay module | Switching high-current loads |
| DC fan | Heat dissipation / air circulation |
| SG90 servo | Airflow/valve positioning |
| Thermocol boxes | Prototype cold-storage chambers |
| 4.7kΩ resistor | DS18B20 pull-up |
| External power supply | Peltier/fan/relay power |

> **Safety:** Do not power Peltier modules, motors, or fans directly from ESP32 GPIO pins. Use suitable driver/relay circuitry and an appropriately rated external supply. For a real cold-storage deployment, use industrial protection, fuses, isolation, proper thermal design, and certified sensors/controllers.

## Software

- Arduino IDE
- ESP32 Arduino Core
- `DHT sensor library`
- `OneWire`
- `DallasTemperature`

## Suggested Pin Mapping

The exact pin mapping depends on the physical prototype. The included firmware uses the following example mapping:

| Device | GPIO |
|---|---:|
| DHT22 Division 1 | 4 |
| DHT22 Division 2 | 16 |
| DHT22 Division 3 | 17 |
| DS18B20 Division 1 | 18 |
| DS18B20 Division 2 | 19 |
| DS18B20 Division 3 | 21 |
| Peltier Relay 1 | 25 |
| Peltier Relay 2 | 26 |
| Peltier Relay 3 | 27 |
| Fan Relay 1 | 32 |
| Fan Relay 2 | 33 |
| Fan Relay 3 | 14 |
| Servo 1 | 13 |
| Servo 2 | 12 |
| Servo 3 | 23 |

**Verify and change these pins to match your actual wiring before uploading.**

## Control Logic

The controller continuously reads temperature and humidity.

Example target ranges used by the prototype:

- Fruits & vegetables: 4–12 °C
- Milk products: 2–6 °C
- Vaccines: 2–8 °C

The thresholds are **prototype control values, not medical/storage certification limits**. Actual vaccine storage must follow the specific product/manufacturer and applicable regulatory requirements.

The firmware uses hysteresis:

```text
Temperature >= HIGH LIMIT
        |
        v
   Cooling ON
        |
Temperature <= LOW LIMIT
        |
        v
   Cooling OFF
```

This reduces rapid relay switching.

## Folder Structure

```text
IoT-Smart-Multi-Division-Cold-Storage/
├── README.md
├── LICENSE
├── .gitignore
├── firmware/
│   └── cold_storage_controller/
│       └── cold_storage_controller.ino
├── docs/
│   ├── architecture.md
│   ├── wiring.md
│   └── test-plan.md
└── media/
    └── README.md
```

## Uploading to ESP32

1. Install Arduino IDE.
2. Install ESP32 board support.
3. Install the required libraries.
4. Open:
   `firmware/cold_storage_controller/cold_storage_controller.ino`
5. Select the correct ESP32 board and COM port.
6. Verify the GPIO mapping.
7. Connect the sensors and actuators.
8. Upload the firmware.
9. Open Serial Monitor at **115200 baud**.

## Future Improvements

- MQTT communication
- Real-time dashboard
- Mobile/web alerts
- Data logging to Firebase/ThingsBoard
- Door-open detection
- Power-failure detection
- Sensor-failure alerts
- PID-based temperature control
- SD-card/cloud historical data
- Independent emergency alarm system

## Disclaimer

This repository documents an educational prototype. It is not a certified refrigeration, food-safety, pharmaceutical-storage, or medical-device system.
