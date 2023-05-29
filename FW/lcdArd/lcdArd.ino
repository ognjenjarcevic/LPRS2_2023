#include <LiquidCrystal.h>

#define USB_DEBUG 0
#define UART_TRANSMIT 1

// Min and max desired temperature
#define TEMP_MIN 20
#define TEMP_MAX 100
int8_t desiredTemp = TEMP_MIN;

// Min and max desired humidity
#define HUM_MIN 10
#define HUM_MAX 50
int8_t desiredHum = HUM_MIN;

// Sets the pins used for communicating with the display
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Data type for easier LCD button manipulation
typedef enum tasterValue {
  NONE = 0,
  SELECT,
  LEFT,
  UP,
  DOWN, 
  RIGHT
};
tasterValue taster;
// Is the temperature or humidity selected on the LCD
bool changeTemp = true;

// Used for button debounce (software differentiation)
int sd = 0;

// Comes through RX from Sensor Arduino
int8_t currentTemp = 0;
int8_t currentHum = 0;

void setup() {
  // Init UART RX (from sensor) and TX (to actuator)
  Serial.begin(9600);

  // zadavanje broja vrsta i kolona
  // nakon inicijalizacije kursor je na poziciji (0,0)
  lcd.begin(16, 2);

  // Write initial settings and readings to lcd
  lcdUpdate();
}

// lcdUpdate should be manually called only when some value changes 
void lcdUpdate()
{
  // Clear the display
  lcd.clear();

  // Selection dot
  // Position of the dot
  lcd.setCursor(0, !changeTemp);
  lcd.print("*");

  // Text
  lcd.setCursor(1, 0);
  lcd.print("Temp:");
  lcd.setCursor(1, 1);
  lcd.print("Hum:");
  lcd.setCursor(9, 0);
  lcd.print("Curr:");
  lcd.setCursor(9, 1);
  lcd.print("Curr:");

  // Values
  lcd.setCursor(6, 0);
  lcd.print(desiredTemp);
  lcd.setCursor(6, 1);
  lcd.print(desiredHum);
  lcd.setCursor(14, 0);
  lcd.print(currentTemp);
  lcd.setCursor(14, 1);
  lcd.print(currentHum);
}

// New instruction to be sent to the actuator
// bit 0 = humidifier
// bit 1 = heater
// ---- --^^
//        ||
// heater-||----humidifier
// 00 = heater off humidifier off
// 01 = heater off humidifier on
// 10 = heater on  humidifier off
// 11 = heater on  humidifier on

int8_t actInstr = 0;

// Decides if new instructions should be sent to the actuator Arduino
// Should be called manually when the data set changes
// Data set = currTemp, currHum, desiredTemp, desiredHum
void decideActuator()
{
  actInstr = 0;
  // Uninitialized or somethings wrong, don't turn on anything :(
  if (currentTemp == 0 && currentHum == 0)
  {
    actInstr = 0;
  }

  else
  {
    // Turn on the heater
    if (currentTemp < desiredTemp)
    {
      actInstr |= (1 << 1);
    }

    // Turn on the humidifier
    if(currentHum < desiredHum)
    { 
      actInstr |= 1;
    }
  }

#if USB_DEBUG
    Serial.print("ActInstr = ");
    Serial.println(actInstr);
#elif UART_TRANSMIT
  sendToActuator();
#endif
}

// Called when any of the actuators should be turned on/off
void sendToActuator()
{
  byte sendData[1] = {actInstr};
  Serial.write(sendData, sizeof(sendData));
}

void loop() {

  // Receiving data from sensorNode
  if (Serial.available() > 0) {
    byte data[2]; // Allocate an array to store the received bytes
    int bytesRead = Serial.readBytes(data, 2); // Read up to 2 bytes from the serial buffer

    currentTemp = (int8_t)data[0];
    currentHum = (int8_t)data[1];
  
    lcdUpdate();
    decideActuator();

#if USB_DEBUG
    Serial.print("T = ");
    Serial.println(data[0]);
    Serial.print("H = ");
    Serial.println(data[1]);
#endif
  }

  taster = readFromTaster();

  if(taster != NONE && sd == 0) {
    sd = 1;
    delay(50);
    
    if(taster == UP && changeTemp && desiredTemp < TEMP_MAX) {
      desiredTemp++;  
      lcdUpdate();
      decideActuator();
    }
    else if(taster == DOWN && changeTemp && desiredTemp > TEMP_MIN){
      desiredTemp--;
      lcdUpdate();
      decideActuator();
    }
    else if(taster == UP && !changeTemp && desiredHum < HUM_MAX) {
      desiredHum++;
      lcdUpdate();
      decideActuator();
    }
    else if(taster == DOWN && !changeTemp && desiredHum > HUM_MIN) {
      desiredHum--;
      lcdUpdate();
      decideActuator();
    }

    else if((taster == RIGHT || taster == LEFT) && changeTemp) {
      changeTemp = false;
      lcdUpdate();
    }
    else if((taster == RIGHT || taster == LEFT) && !changeTemp) {
      changeTemp = true;
      lcdUpdate();
    }
  }
  if(taster == NONE) {      
    sd = 0;
  }

}

// reading values from tasters
tasterValue readFromTaster() {
  int value = analogRead(0);

  if(value < 50) return RIGHT;//RIGHT
  else if(value < 175) return UP;//UP  
  else if(value < 325) return DOWN;//DOWN
  else if(value < 520) return LEFT;//LEFT
  else if(value < 800) return SELECT;//SELECT
  else return NONE;//NONE
  
}
