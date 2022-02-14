const int sensorPin = A0;
const float baselineTemp = 26.0;
const float voltageFactor = 5.0/1024.0;
float previous = 0.0;
float filterStrength = 0.98;
int tSensorVal;

float filter(float previousVal, float currentVal, float strengthVal) {
    return previousVal * strengthVal + currentVal * (1.0 - strengthVal);
}

float voltToCelsius(float voltage) {
  return (voltage - 0.5) * 100;
}
void setup() {
  Serial.begin(9600);
  previous = voltToCelsius(analogRead(sensorPin)*voltageFactor);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  float temp = voltToCelsius(sensorVal*voltageFactor);
  float filteredTemp = filter(previous, temp, filterStrength);
  previous = filteredTemp;
  Serial.print(filteredTemp);
  Serial.print("\n");

}
