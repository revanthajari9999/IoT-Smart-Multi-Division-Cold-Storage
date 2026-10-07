#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP32Servo.h>

// -------------------- Sensor configuration --------------------
#define DHTTYPE DHT22

const uint8_t DHT_PINS[3] = {4, 16, 17};
const uint8_t DS18B20_PINS[3] = {18, 19, 21};

// -------------------- Actuator configuration --------------------
const uint8_t PELTIER_RELAY_PINS[3] = {25, 26, 27};
const uint8_t FAN_RELAY_PINS[3]     = {32, 33, 14};
const uint8_t SERVO_PINS[3]         = {13, 12, 23};

// Many relay modules are active LOW.
// Change to HIGH if your relay module is active HIGH.
const uint8_t RELAY_ON  = LOW;
const uint8_t RELAY_OFF = HIGH;

// -------------------- Storage configuration --------------------
struct DivisionConfig {
  const char* name;
  float tempLow;
  float tempHigh;
  float humidityHigh;
};

DivisionConfig divisions[3] = {
  {"Fruits & Vegetables", 4.0, 12.0, 90.0},
  {"Milk Products",        2.0,  6.0, 85.0},
  {"Vaccines",             2.0,  8.0, 70.0}
};

// -------------------- Objects --------------------
DHT* dht[3];
OneWire* oneWire[3];
DallasTemperature* ds18b20[3];
Servo airflowServo[3];

bool coolingState[3] = {false, false, false};

unsigned long lastReadMs = 0;
const unsigned long READ_INTERVAL_MS = 3000;

// -------------------- Helpers --------------------
void setCooling(uint8_t i, bool enabled) {
  coolingState[i] = enabled;

  digitalWrite(PELTIER_RELAY_PINS[i], enabled ? RELAY_ON : RELAY_OFF);
  digitalWrite(FAN_RELAY_PINS[i], enabled ? RELAY_ON : RELAY_OFF);

  // Open airflow more when cooling.
  airflowServo[i].write(enabled ? 120 : 60);
}

void printSensorData(uint8_t i, float humidity, float dhtTemp, float dsTemp) {
  Serial.println("----------------------------------------");
  Serial.print("Division: ");
  Serial.println(divisions[i].name);

  Serial.print("DHT22 Temperature: ");
  Serial.print(dhtTemp);
  Serial.println(" C");

  Serial.print("DHT22 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("DS18B20 Temperature: ");
  Serial.print(dsTemp);
  Serial.println(" C");

  Serial.print("Cooling: ");
  Serial.println(coolingState[i] ? "ON" : "OFF");
}

void controlDivision(uint8_t i, float temperature, float humidity) {
  // Hysteresis prevents rapid ON/OFF relay switching.
  if (!coolingState[i] && temperature > divisions[i].tempHigh) {
    setCooling(i, true);
  }

  if (coolingState[i] && temperature < divisions[i].tempLow) {
    setCooling(i, false);
  }

  // Simple humidity warning. This prototype does not actively dehumidify.
  if (!isnan(humidity) && humidity > divisions[i].humidityHigh) {
    Serial.print("WARNING: High humidity in ");
    Serial.println(divisions[i].name);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("========================================");
  Serial.println(" Smart Multi-Division Cold Storage");
  Serial.println(" ESP32 Controller");
  Serial.println("========================================");

  for (uint8_t i = 0; i < 3; i++) {
    dht[i] = new DHT(DHT_PINS[i], DHTTYPE);
    dht[i]->begin();

    oneWire[i] = new OneWire(DS18B20_PINS[i]);
    ds18b20[i] = new DallasTemperature(oneWire[i]);
    ds18b20[i]->begin();

    pinMode(PELTIER_RELAY_PINS[i], OUTPUT);
    pinMode(FAN_RELAY_PINS[i], OUTPUT);

    digitalWrite(PELTIER_RELAY_PINS[i], RELAY_OFF);
    digitalWrite(FAN_RELAY_PINS[i], RELAY_OFF);

    airflowServo[i].setPeriodHertz(50);
    airflowServo[i].attach(SERVO_PINS[i], 500, 2400);
    airflowServo[i].write(60);
  }

  Serial.println("System initialized.");
}

void loop() {
  if (millis() - lastReadMs < READ_INTERVAL_MS) {
    return;
  }

  lastReadMs = millis();

  for (uint8_t i = 0; i < 3; i++) {
    float humidity = dht[i]->readHumidity();
    float dhtTemp = dht[i]->readTemperature();

    ds18b20[i]->requestTemperatures();
    float dsTemp = ds18b20[i]->getTempCByIndex(0);

    // If DS18B20 is disconnected, use DHT22 temperature for the
    // prototype control decision when available.
    float controlTemperature = dsTemp;

    if (dsTemp == DEVICE_DISCONNECTED_C || isnan(dsTemp)) {
      controlTemperature = dhtTemp;
      Serial.print("WARNING: DS18B20 unavailable in ");
      Serial.println(divisions[i].name);
    }

    if (!isnan(controlTemperature)) {
      controlDivision(i, controlTemperature, humidity);
    } else {
      Serial.print("ERROR: No valid temperature reading in ");
      Serial.println(divisions[i].name);
      setCooling(i, false);
    }

    printSensorData(i, humidity, dhtTemp, dsTemp);
  }
}
