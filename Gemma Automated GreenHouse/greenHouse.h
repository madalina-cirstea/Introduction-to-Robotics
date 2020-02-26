#ifndef greenHouse_lib
#define greenHouse_lib

#include <Arduino.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <DHT.h>

#define DHTTYPE DHT21

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define __CS 53
#define __DC 2
#define __A0 3

const int closeAngle = 180;
const int openAngle = 0;

const int minDrynessThreshold = 350;
const int maxDrynessThreshold = 450;

const float maxAirTemperatureThreshold = 30.00;
const float minAirTemperatureThreshold = 10.00;
const float maxAirHumidityThreshold = 40.00;

class GreenHouse {
    // soil temperature (water proof) sensor
    const int ONE_WIRE_BUS = 5;
    OneWire oneWire;
    DallasTemperature sensors;
    float soilTemperature;

    // soil moisture humidity sensors
    const int MHsensorPinLeft = A0;
    const int MHsensorPinRight = A1;
    int MHvalueLeft;
    int MHvalueRight;
    int averageMHsoil;

    // air temperature and humidity sensor
    const int airTempHumPin = 10;
    DHT dht;
    float airTemperature;
    float airHumidity;

    // waterPump
    const int relayPin = 6;

    // trapdoors' pins
    const int trapdoorFrontPin = 8;
    const int trapdoorBackPin = 9;

    bool manualModeOn;
    bool waterPumpOn;
    bool trapdoorFrontOpened;
    bool trapdoorBackOpened;

  public:
    TFT_ILI9163C display;
    Servo trapdoorFront;
    Servo trapdoorBack;

    GreenHouse();

    // soil humidity
    void recordMHvalueLeft();
    void recordMHvalueRight();
    void calculateAverageMHsoil();
    int getAverageMHsoil();

    // soil temperature
    void recordSoilTemperature();
    float getRecordedSoilTemperature();

    // air temperature
    void recordAirTemperature();
    float getRecordedAirTemperature();

    // air humidity
    void recordAirHumidity();
    float getRecordedAirHumidity();

    // trapdoors' pins
    const int getTrapdoorFrontPin();
    const int getTrapdoorBackPin();

    bool onManualMode();
    bool isWaterPumpOn();
    bool isTrapdoorFrontOpened();
    bool isTrapdoorBackOpened();
    bool areBothTrapdoorsOpened();
    bool areBothTrapdoorsClosed();

    void setManualMode();
    void setAutomaticMode();
    void turnWaterPumpOn();
    void turnWaterPumpOff();
    void openTrapdoorFront();
    void closeTrapdoorFront();
    void openTrapdoorBack();
    void closeTrapdoorBack();
    void openBothTrapdoors();
    void closeBothTrapdoors();

    void displayWelcomeMessage();
    void displayRecordedValues();

    void updateRecordedValues();
    void analizeRecordings();
};

#endif
