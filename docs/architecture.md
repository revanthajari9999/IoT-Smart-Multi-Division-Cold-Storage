# System Architecture

## High-Level Flow

```text
Sensors
  |
  v
ESP32
  |
  +--> Temperature/Humidity Processing
  |
  +--> Threshold + Hysteresis Controller
  |
  +--> Relay Control --> Peltier
  |
  +--> Relay Control --> Fans
  |
  +--> Servo Control --> Airflow
  |
  +--> Serial Monitoring
```

## Divisions

### Division 1 — Fruits & Vegetables
Prototype target: 4–12 °C.

### Division 2 — Milk Products
Prototype target: 2–6 °C.

### Division 3 — Vaccines
Prototype target: 2–8 °C.

These are educational prototype values. Real storage requirements must be obtained from the relevant product specifications and regulations.

## Why Hysteresis?

Without hysteresis, a controller close to a threshold can repeatedly switch a relay:

```text
ON -> OFF -> ON -> OFF
```

Hysteresis creates separate ON and OFF thresholds, reducing unnecessary switching and actuator wear.
