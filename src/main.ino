#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

#define SDA 13
#define SCL 14

const int DHT_PIN = 18;

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Store the current sensor data
float temperature = 0.0;
float humidity = 0.0;

// Timing
unsigned long previousSensorTime = 0;
unsigned long previousDisplayTime = 0;

const unsigned long SENSOR_INTERVAL = 2000;
const unsigned long DISPLAY_INTERVAL = 500;


void setup() {

    Serial.begin(115200);

    Wire.begin(SDA, SCL);

    // Check LCD address
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }

    lcd.init();
    lcd.backlight();

    // Initialize DHT11
    dht.setup(DHT_PIN, DHTesp::DHT11);

    lcd.setCursor(0, 0);
    lcd.print("Env Monitor");
    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    delay(1000);

    lcd.clear();
}


void loop() {

    unsigned long currentTime = millis();

    // SENSOR TASK
    if (currentTime - previousSensorTime >= SENSOR_INTERVAL) {

        previousSensorTime = currentTime;

        TempAndHumidity data = dht.getTempAndHumidity();

        if (dht.getStatus() == 0) {

            temperature = data.temperature;
            humidity = data.humidity;

            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.println(" C");

            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.println(" %");

            Serial.println("-------------------");

        }
        else {

            Serial.print("DHT Error: ");
            Serial.println(dht.getStatusString());
        }
    }

    // DISPLAY TASK
    if (currentTime - previousDisplayTime >= DISPLAY_INTERVAL) {

        previousDisplayTime = currentTime;

        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(temperature, 1);
        lcd.print(" C   ");

        lcd.setCursor(0, 1);
        lcd.print("Hum:  ");
        lcd.print(humidity, 1);
        lcd.print(" %   ");
    }
}


// Check whether an I2C device responds
bool i2CAddrTest(uint8_t addr) {

    Wire.beginTransmission(addr);

    if (Wire.endTransmission() == 0) {
        return true;
    }

    return false;
}
