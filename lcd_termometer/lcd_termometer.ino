#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);

const int tSensorPin = A0;

const float voltageFactor = 5.0/1024.0;
float prev = 0.0;
float rawTC = 0.0;
float filterStrength = 0.975;
float tempC = 0.0;
float tempF = 0.0;
float minC = 100.0;
float maxC = -50.0;
int tSensorVal = 0;

float toFahrenheit() {
    return (tempC * 9.0/5.0) + 32;
}
float filter() {
    return prev * filterStrength + rawTC * (1.0 - filterStrength);
}

float voltToCelsius(float voltage) {
    return (voltage - 0.5) * 100;
}

void printToLCD() {
    lcd.print(tempC, 1);
    lcd.print("C");
    lcd.print(" Max: ");
    lcd.print(maxC,1);
    lcd.print("C");
    lcd.setCursor(0,1);
    lcd.print(tempF, 1);
    lcd.print("F");
    lcd.print(" Min: ");
    lcd.print(minC,1);
    lcd.print("C");
    delay(1000);
    lcd.clear();
}
void setup() {
    lcd.noBlink();
    //only done to avoid waiting for filter to catch up
    prev = voltToCelsius(analogRead(tSensorPin)*voltageFactor);
    lcd.begin(16,2);
}

void loop() {
    lcd.noBlink();
    tSensorVal = analogRead(tSensorPin);
    rawTC = voltToCelsius(tSensorVal*voltageFactor);
    tempC = filter();
    tempF = toFahrenheit();
    prev = tempC;
    if (tempC > maxC) {
        maxC = tempC;
    }
    if (tempC < minC) {
        minC = tempC;
    }
    printToLCD();
}
