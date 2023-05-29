#define HEATER_PIN A0 // pin 4 - output 2 - heater
#define HUM_PIN A1 // pin 7 - output 1 - humidifier

void setup() {
  Serial.begin(9600);

  pinMode(HEATER_PIN, OUTPUT);
  pinMode(HUM_PIN, OUTPUT);
}

byte data[1];

void loop() {
  // receiving data from lcd 
  while (Serial.available() <= 0) {
      return;
  }
    int bytesRead = Serial.readBytes(data, 1); // Read up to 1 byte from the serial buffer
  // bit 0 = humidifier
  // bit 1 = heater
  // ---- --^^
  //        ||
  // heater-||----humidifier
  // 00 = heater off humidifier off
  // 01 = heater off humidifier on
  // 10 = heater on  humidifier off
  // 11 = heater on  humidifier on

  // Humidifier on/off
  if(data[0] & 1) 
  {
    digitalWrite(HUM_PIN, HIGH);
    Serial.println("humi = on");
  } 
  else 
  {
    digitalWrite(HUM_PIN, LOW);
    Serial.println("humi = off");
  }

  // Heater on/off
  if((data[0]>>1) & 1) 
  {
    digitalWrite(HEATER_PIN, HIGH);
    Serial.println("heat = on");
  } 
  else  
  {
    digitalWrite(HEATER_PIN, LOW);
    Serial.println("heat = off");
  }
}

