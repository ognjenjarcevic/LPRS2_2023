#include <dht.h>

// DHT PINS:
// left = data = pin 7
// middle = 5v
// right = gnd

// Instance of dht class
dht DHT;

#define DHT11_PIN 7

#define USB_DEBUG 0
#define UART_TRANSMIT 1

void setup(){
  Serial.begin(9600);
}

void loop(){
  //Reading from sensors
  int chk = DHT.read11(DHT11_PIN);
  int8_t temp = (int8_t) DHT.temperature;
  int8_t hum = (int8_t) DHT.humidity;

  // UART transmission to monitoring arduino
  // Data format
  byte data[2] = {temp, hum};

#if UART_TRANSMIT
  Serial.write(data, sizeof(data)); // Send the data
#elif USB_DEBUG
  // Console output
  Serial.print("T = ");
  Serial.println(temp);
  Serial.print("H = ");
  Serial.println(hum);
#endif

  delay(1000);
}