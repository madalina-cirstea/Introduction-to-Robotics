# Gemma v1.0 Automated GreenHouse
...

## User Instructions
### Remote
![remote](https://user-images.githubusercontent.com/56949829/75142137-178b3300-56fb-11ea-8276-287644a78839.png)

## Manual Mode
In this mode you can manually control all functionalities implemented in Gemma v1.0 (using a personalized remote control) based on the sensor recordings shown on the display:
   - open/ close trapdoor front/ back
   - turn water pump ON and OFF
   
## Automatic Mode
Gemma v1.0 can successfully operate autonomously based on the values recorded by the sensors (two soil humidity sensors, one soil temperature sensor and one air temperature and humidity sensor).

Thresholds set to maintain the **optimum soil humidity** inside the greenHouse: 350 - 450 VMC (measured in VMC - volumetric moisture content). If the soil humidity exceeds the maximum permissible dryness threshold (maxDrynessThreshold) - soil is fully dried, than the water pump should be turned on and should remain in this state before the soil humidity drops below this level. If the soil humidity is below the minimum permissible dryness threshold (minDrynessThreshold) - soil is fully watered, than the water pump should be turned off. In an optimal environent soil humidity should remain between minimum and maximum permissible dryness threshold.

Thresholds set to maintain the **optimum air temperature** inside the greenHouse: 10.00 - 30.00 C (measured in C - celsius degrees). If the tempearture rises above the maximum air temperature threshold (maxAirTemperatureThreshold),  both front and back trapdoors must be opened. If the temperature drops below the minimum air temperature threshold (minAirTemperatureThreshold), both trapdoors must be closed.

If the air temperature is within normal parameters we can analize **air humidity** measured in %RH - relative humidity (order of relevance: temperature, than humidity). Gemma v1.0 implements a basic ventilation system for keeping humidity under maximum threshold (40.00 %RH): two open doors for keeping air moving (but only when air temperature is within normal parameters). If both air temperature and air humidity are within normal parameters we want to keep the greenHouse closed by default.
 
## Hardware componenets

- Non Submersible Water Pump
- Relay SRD-05VDC-SL-C
- 2 x Soil Moisture Sensor
- Temperature Sensor DHT21/ AM2301 
- Waterproof Temperature Sensor DS18B20
- 2 x Servo Motor MG996R
- Infrared remote control + Receiver
- TFT Module 1.44 (ILI9163C)
- Power Source, 9V Battery + Adapter
