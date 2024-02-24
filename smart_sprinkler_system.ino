#include <EEPROM.h>
#include <dht.h>
#define rainSensorPin A0
#define humiditySensorPin A1

dht DHT;

int cropDataAddress = 0;
int currentSelectedCrop = 0;

const int totalCrops = 3;
// #define rainThreshold 400  // Adjust based on sensor sensitivity
// #define moistureThreshold 300

struct CropConfig {
  char name[128];
  int rainThreshold;
  int moistureThreshold;
  int blinks;
};

float calculatePercentage(int value, int maxValue) {
  float percentage = (float)value / (float)maxValue * 100.0;
  return percentage;
}

struct CropConfig cropConfigs[totalCrops] = { { "Wheat", 400, 400, 2 }, { "Paddy", 200, 200, 4 }, {"Barley", 250, 200, 6}};

void setup() {
  currentSelectedCrop = EEPROM.read(cropDataAddress);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
  pinMode(4, INPUT);

  digitalWrite(12, HIGH);

  delay(1000);
  digitalWrite(13, HIGH);
  showSelectedMode(cropConfigs[currentSelectedCrop].blinks);
  
}

void showSelectedMode(int blinks) {
  delay(1000);
  for (int blink = 0; blink <= blinks; blink++) {
    digitalWrite(13, HIGH);
    delay(250);
    digitalWrite(13, LOW);
    delay(250);
  }
  delay(2500);
}
float getA0Data() {
  return calculatePercentage(analogRead(A0), 1023);
}

void loop() {
  int sensorValueOfRainfall = analogRead(A0);
  DHT.read11(humiditySensorPin);
  Serial.print("Current Humidity: ");
  Serial.print(DHT.humidity);
  Serial.println(" %");
  Serial.print("Current Temperature: ");
  Serial.print(DHT.temperature);
  Serial.println(" C");
  Serial.print("Current Rainfall: ");
  Serial.print(getA0Data());
  Serial.println(" %");
  Serial.print("Current Selected Crop: ");
  Serial.println(cropConfigs[currentSelectedCrop].name);
  Serial.println();
  delay(2000);

  if (digitalRead(2) == HIGH) {
    // Serial.println("currentSelectedCrop");
    // Serial.println(currentSelectedCrop);
    // Serial.println("totalCrops");
    // Serial.println(totalCrops - 1);

    if (currentSelectedCrop == totalCrops - 1) {
      currentSelectedCrop = 0;
    } else {
      currentSelectedCrop = currentSelectedCrop + 1;
    }
  }
  delay(250);
  if (digitalRead(4) == HIGH) {
    EEPROM.update(cropDataAddress, currentSelectedCrop);
    Serial.print("Saved Selected Crop: ");
    Serial.println(cropConfigs[currentSelectedCrop].name);
  }
}