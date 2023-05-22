# Smart Sauna Arduino Project

(Zoom out to see the ASCII magic banner art in the text editor)

______________________________________/\\\\\\\\\__________________________/\\\_________________________________________/\\\________/\\\_____________________________        
 ____________________________________/\\\\\\\\\\\\\_______________________\/\\\________________________________________\/\\\_______\/\\\_____________________________       
  ___________________________________/\\\/////////\\\______________________\/\\\________________________________________\/\\\_______\/\\\_____________________________      
   __/\\\\\\\\\\____/\\\\\__/\\\\\___\/\\\_______\/\\\__/\\/\\\\\\\_________\/\\\_____________/\\\\\\\\\\__/\\\\\\\\\____\/\\\_______\/\\\__/\\/\\\\\\_______/\\\\\____     
    _\/\\\//////___/\\\///\\\\\///\\\_\/\\\\\\\\\\\\\\\_\/\\\/////\\\___/\\\\\\\\\____________\/\\\//////__\////////\\\___\/\\\_______\/\\\_\/\\\////\\\____/\\\///\\\__    
     _\/\\\\\\\\\\_\/\\\_\//\\\__\/\\\_\/\\\/////////\\\_\/\\\___\///___/\\\////\\\____________\/\\\\\\\\\\___/\\\\\\\\\\__\/\\\_______\/\\\_\/\\\__\//\\\__/\\\__\//\\\_   
      _\////////\\\_\/\\\__\/\\\__\/\\\_\/\\\_______\/\\\_\/\\\_________\/\\\__\/\\\____________\////////\\\__/\\\/////\\\__\//\\\______/\\\__\/\\\___\/\\\_\//\\\__/\\\__  
       __/\\\\\\\\\\_\/\\\__\/\\\__\/\\\_\/\\\_______\/\\\_\/\\\_________\//\\\\\\\/\\____________/\\\\\\\\\\_\//\\\\\\\\/\\__\///\\\\\\\\\/___\/\\\___\/\\\__\///\\\\\/___ 
        _\//////////__\///___\///___\///__\///________\///__\///___________\///////\//____________\//////////___\////////\//_____\/////////_____\///____\///_____\/////_____



smArd saUno project is a Smart Sauna project that enables automatic sauna control using three Arduino Uno.
Each Arduino represents a main aspect of an automated system. 
The three Arduinos are named: "Sensor Node", "LCD Node" and "Actuator Node" respectively.
Their setup configurations, roles and functions are explained in this document.

## Sensor Node

Sensor Node is an Arduino Uno equipped with DHT11 temperature and humidity sensor, and is connected the LCD Node.
It periodically retreives the measurements from the sensor and sends the data to the LCD Node through the UART TX PIN.

DHT11 is located on a small module board, connected to the arduino with 3 jumper cables.
DHT11 pinout:
- left = data
- middle = 5v
- right = gnd

### Arduino pinout:
- PIN 7 <---> DHT11 data
- UART TX PIN <---> LCD Node RX PIN

## LCD Node

LCD Node is an Arduino Uno equipped with the LCD Keypad Shield (Model: MCS01602M). It is connected to the Sensor Node and the Actuator node.

The LCD Keypad Shield is used to achieve the following functionalities:
- show the current temperature and humidity which are periodically retreived from the Sensor Node. 
- provide the user with the buttons needed to input the desired temperature and humidity
- show the currently desired temperature and humidity

LCD Node periodically receives the measurements from the Sensor Node. They are then compared to the desired temperature and humidity. If current temperature or humidity values are below/above the desired, their respective actuators are to be turned on/off.
Depending on the results of the comparison, one of the following commands is sent to the Actuator Node.
### Signals
- heater off humidifier off (code 00)
- heater off humidifier on  (code 01)
- heater on  humidifier off (code 10)
- heater on  humidifier on  (code 11)

### Arduino pinout:
- Most of the PINs are used by the LCD Keypad Shield
- UART RX PIN <---> Sensor Node TX PIN
- UART TX PIN <---> Actuator Node RX PIN

## Actuator Node

Actuator Node is an Arduino Uno equipped with the wiring controls to create signals to turn on/off the humidifier and heater. It periodically receives the instructions from the LCD Node and creates the power signals.

### Current actuator setup
- Heater: HausmaX W-HKL heater 230V-50Hz 2000W
- Humidifier: 5V 2W Mini Atomization Humidifier

### Arduino pinout:
- UART RX PIN <---> LCD Node TX PIN
- PIN 4 <---> Heater power signal
- PIN 7 <---> Humidifier power signal