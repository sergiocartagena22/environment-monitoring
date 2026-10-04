#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

#define SDA 13
#define SCL 14

const int DHT_PIN = 18;
const int LIGHT_PIN = 4;

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Store Data
float temperature = 0.0;
float humidity = 0.0;
int lightRaw =0;
int lightPercent = 0;

// Timers
unsigned long previousDHTTime = 0;
unsigned long previousLightTime = 0;
unsigned long previousDisplayTime = 0;

const unsigned long DHT_INTERVAL = 2000;
const unsigned long LIGHT_INTERVAL = 250;
const unsigned long DISPLAY_INTERVAL = 3000;

// Controls LCD page displayed
bool displayEnvironment = true;


void setup() {

    Serial.begin(115200); 

    Wire.begin(SDA, SCL);

    // Check LCD address
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }

    lcd.init();
    lcd.backlight();

    // DHT11
    dht.setup(DHT_PIN, DHTesp::DHT11);

    // Photoresistor
    pinMode(LIGHT_PIN, INPUT);

    lcd.setCursor(0, 0);
    lcd.print("Env Monitor");

    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    delay(1000);
    lcd.clear();

    Serial.println("ESP32 Environmental Monitor");
    Serial.println("============================");
}


void loop() {

    unsigned long currentTime = millis();

    // DHT sensor task
    if (currentTime - previousDHTTime >= DHT_INTERVAL) {

        previousDHTTime = currentTime;

        TempAndHumidity data = dht.getTempAndHumidity();

        if (dht.getStatus() == 0) {

            temperature = data.temperature;
            humidity = data.humidity;

        } else {

            Serial.print("DHT Error: ");
            Serial.println(dht.getStatusString());
        }
    }

    // Light sensor task
    if (currentTime - previousLightTime >= LIGHT_INTERVAL) {

        previousLightTime = currentTime;

        lightRaw = analogRead(LIGHT_PIN);

        // ESP32 ADC is normally 12-bit:
        // 0 - 4095
        lightPercent = map(lightRaw, 0, 4095, 0, 100);

        lightPercent = constrain(lightPercent, 0, 100);
    }

    // Serial output
    static unsigned long previousSerialTime = 0;

    if (currentTime - previousSerialTime >= 2000) {

        previousSerialTime = currentTime;

        Serial.println("----------------------------");

        Serial.print("Temperature: ");
        Serial.print(temperature, 1);
        Serial.println(" C");

        Serial.print("Humidity: ");
        Serial.print(humidity, 1);
        Serial.println(" %");

        Serial.print("Light RAW: ");
        Serial.println(lightRaw);

        Serial.print("Light: ");
        Serial.print(lightPercent);
        Serial.println(" %");
    }

    // Display task
    if (currentTime - previousDisplayTime >= DISPLAY_INTERVAL) {

        previousDisplayTime = currentTime;

        displayEnvironment = !displayEnvironment;

        lcd.clear();

        if (displayEnvironment) {

            lcd.setCursor(0, 0);
            lcd.print("Temp: ");
            lcd.print(temperature, 1);
            lcd.print(" C");

            lcd.setCursor(0, 1);
            lcd.print("Hum:  ");
            lcd.print(humidity, 1);
            lcd.print(" %");

        } else {

            lcd.setCursor(0, 0);
            lcd.print("Light: ");
            lcd.print(lightPercent);
            lcd.print("%");

            lcd.setCursor(0, 1);
            lcd.print("ADC: ");
            lcd.print(lightRaw);
        }
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
