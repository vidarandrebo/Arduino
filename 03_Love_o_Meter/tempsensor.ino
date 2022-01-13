const int sensorPin = A0;
const float baselineTemp = 26.0;
const float voltageFactor = 5.0/1024.0;
float previous = 0.0;
float filterStrength = 0.98;

float filter(float previousVal, float currentVal, float strengthVal) {
    return previousVal * strengthVal + currentVal * (1.0 - strengthVal);
}

float voltToCelsius(float voltage) {
  return (voltage - 0.5) * 100;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int pinNumber = 2; pinNumber < 5; pinNumber++) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  Serial.print("Sensor Value: ");
  Serial.print(sensorVal);
  Serial.print("\t");
  Serial.print("Voltage Value: ");
  float voltage = sensorVal*voltageFactor;
  Serial.print("\t");
  Serial.print("Temperature Value: ");
  float temp = voltToCelsius(sensorVal*voltageFactor);
  Serial.print(temp);
  Serial.print("\t");
  Serial.print("Filtered Value: ");
  float filteredTemp = filter(previous, temp, filterStrength);
  previous = filteredTemp;
  Serial.print(filteredTemp);
  Serial.print("\n");
  if (filteredTemp > baselineTemp + 4) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }
  else if (filteredTemp > baselineTemp + 2) {
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
  }
  else if (filteredTemp > baselineTemp) {
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }
  else {
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
  }

}
