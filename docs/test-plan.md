# Test Plan

| Test | Procedure | Expected Result |
|---|---|---|
| DHT22 reading | Power system and observe Serial Monitor | Temperature and humidity are displayed |
| DS18B20 reading | Connect sensor and observe output | Temperature is displayed |
| High temperature | Raise sensor temperature above high limit | Cooling and fan turn ON |
| Low temperature | Reduce temperature below low limit | Cooling and fan turn OFF |
| Humidity warning | Raise humidity above configured threshold | Warning appears |
| Sensor disconnect | Disconnect DS18B20 | Warning appears and fallback is used when possible |
| Servo control | Trigger cooling | Servo moves to airflow position |
| Startup | Reset ESP32 | Actuators start in safe OFF state |

## Acceptance Criteria

The prototype passes when:

1. All available sensors produce valid readings.
2. Cooling activates above the configured upper threshold.
3. Cooling deactivates below the configured lower threshold.
4. Rapid relay oscillation is avoided by hysteresis.
5. Sensor disconnection is reported.
6. Actuators are not powered directly from ESP32 GPIO pins.
