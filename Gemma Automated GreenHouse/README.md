# Gemma v1.0 Automated GreenHouse
Gemma v1.0 is a smart automated greenHouse developed as a final project for the Introduction to Robotics course held by @Unibuc Robotics at University of Bucharest.

## User Instructions
### Remote
![remote](https://user-images.githubusercontent.com/56949829/75142137-178b3300-56fb-11ea-8276-287644a78839.png)

## Manual Mode
In this mode you can manually control all functionalities implemented in Gemma v1.0 (using a personalized remote control) based on the sensor recordings shown on the display:
   - open/ close trapdoor front/ back
   - turn water pump ON and OFF
   
## Automatic Mode
Gemma v1.0 can successfully operate autonomously based on the values recorded by the sensors (two soil humidity sensors, one soil temperature sensor (water proof) and one air temperature and humidity sensor).

Thresholds set to maintain the **optimum soil humidity** inside the greenHouse: 350 - 450 VMC (measured in VMC - volumetric moisture content). If the soil humidity exceeds the maximum permissible dryness threshold (maxDrynessThreshold) - soil is fully dried, than the water pump should be turned on and should remain in this state before the soil humidity drops below this level. If the soil humidity is below the minimum permissible dryness threshold (minDrynessThreshold) - soil is fully watered, than the water pump should be turned off. In an optimal environment soil humidity should remain between minimum and maximum permissible dryness threshold.

Thresholds set to maintain the **optimum air temperature** inside the greenHouse: 10.00 - 30.00 C (measured in C - celsius degrees). If the tempearture rises above the maximum air temperature threshold (maxAirTemperatureThreshold),  both front and back trapdoors must be opened. If the temperature drops below the minimum air temperature threshold (minAirTemperatureThreshold), both trapdoors must be closed.

If the air temperature is within normal parameters we can analize **air humidity** measured in %RH - relative humidity (order of relevance: temperature, than humidity). Gemma v1.0 implements a basic ventilation system for maintaining humidity under maximum threshold (40.00 %RH): two open doors for keeping air moving (but only when air temperature is within normal parameters). If both air temperature and air humidity are within normal parameters we want to keep the greenHouse closed by default.
 
## Hardware components

- Arduino MEGA 2560
- Non Submersible Water Pump
- Relay SRD-05VDC-SL-C
- 2 x Soil Moisture Sensor
- Temperature Sensor DHT21/ AM2301 
- Waterproof Temperature Sensor DS18B20
- Servo Motor MG996R (for trapdoor front)
- Servo Motor MG995 (for trapdoor back)
- Infrared remote control + Receiver
- TFT Module 1.44 (ILI9163C)
- Power Source, 9V Battery + Adapter

### Other components:
- irrigation system components (connectors, hose, dyke, etc.)
- hobby glass
- wooden frame
- greenHouse plastic wrap

## Used liraries
- for LCD display: tft_ili9163c, Adafruit_GFX, SPI
- for air temperature and humidity sensor: DHT
- for water proof soil temperature sensor: 

[Let's meet Bill! (of materials)](https://docs.google.com/spreadsheets/d/1Htry010sDG5Vxl1XxuDkIDsEU6a6pIBbHVVmY9l-o_E/edit#gid=1130803135)
