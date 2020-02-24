#include "greenHouse.h"

GreenHouse::GreenHouse(): oneWire(ONE_WIRE_BUS), sensors(&oneWire), display(__CS, __A0, __DC), dht(airTempHumPin, DHTTYPE) {
  // greenHouse is on manualMode by default
  manualModeOn = true;
  waterPumpOn = false;
  trapdoorFrontOpened = false;
  trapdoorBackOpened = false;

  trapdoorFront.attach(trapdoorFrontPin);
  trapdoorBack.attach(trapdoorBackPin);

  pinMode(MHsensorPinLeft, INPUT);
  pinMode(MHsensorPinRight, INPUT);
  pinMode(relayPin, OUTPUT);

  dht.begin();
  display.begin();
}

bool GreenHouse::onManualMode() {
  return manualModeOn;
}

bool GreenHouse::isWaterPumpOn() {
  return waterPumpOn;
}

bool GreenHouse::isTrapdoorFrontOpened() {
  return trapdoorFrontOpened;
}

bool GreenHouse::isTrapdoorBackOpened() {
  return trapdoorBackOpened;
}

void GreenHouse::setAutomaticMode() {
  manualModeOn = false;
}

void GreenHouse::setManualMode() {
  manualModeOn = true;
}

void GreenHouse::turnWaterPumpOn() {
  waterPumpOn = true;
  // turn relay ON
  digitalWrite(relayPin, HIGH);
}

void GreenHouse::turnWaterPumpOff() {
  waterPumpOn = false;
  // turn relay OFF
  digitalWrite(relayPin, LOW);
}

void GreenHouse::openTrapdoorFront() {
  trapdoorFrontOpened = true;
  for (int pos = 180; pos >= 0; pos--) {
    trapdoorFront.write(pos);
    delay(15);
  }
}

void GreenHouse::closeTrapdoorFront() {
  trapdoorFrontOpened = false;
  for (int pos = 0; pos <= 180; pos++) {
    trapdoorFront.write(pos);
    delay(15);
  }
}

void GreenHouse::openTrapdoorBack() {
  trapdoorBackOpened = true;
  for (int pos = 180; pos >= 0; pos--) {
    trapdoorBack.write(pos);
    delay(15);
  }
}

void GreenHouse::closeTrapdoorBack() {
  trapdoorBackOpened = false;
  for (int pos = 0; pos <= 180; pos++) {
    trapdoorBack.write(pos);
    delay(15);
  }
}

bool GreenHouse::areBothTrapdoorsClosed() {
  return (!trapdoorFrontOpened && !trapdoorBackOpened);
}

bool GreenHouse::areBothTrapdoorsOpened() {
  return (trapdoorFrontOpened && trapdoorBackOpened);
}

void GreenHouse::openBothTrapdoors() {
  if (areBothTrapdoorsClosed()) {
    trapdoorFrontOpened = true;
    trapdoorBackOpened = true;
    for (int pos = 180; pos >= 0; pos--) {
      trapdoorFront.write(pos);
      trapdoorBack.write(pos);
      delay(15);
    }
  }
  else if (!trapdoorFrontOpened)
    openTrapdoorFront();
  else if (!trapdoorBackOpened)
    openTrapdoorBack();
}

void GreenHouse::closeBothTrapdoors() {
  if (areBothTrapdoorsOpened()) {
    trapdoorFrontOpened = false;
    trapdoorBackOpened = false;
    for (int pos = 0; pos <= 180; pos++) {
      trapdoorFront.write(pos);
      trapdoorBack.write(pos);
      delay(15);
    }
  }
  else if (trapdoorFrontOpened)
    closeTrapdoorFront();
  else if (trapdoorBackOpened)
    closeTrapdoorBack();
}

void GreenHouse::recordMHvalueLeft() {
  MHvalueLeft = analogRead(MHsensorPinLeft);
}

void GreenHouse::recordMHvalueRight() {
  MHvalueRight = analogRead(MHsensorPinRight);
}

void GreenHouse::calculateAverageMHsoil() {
  averageMHsoil = (MHvalueLeft + MHvalueRight) / 2;
}

int GreenHouse::getAverageMHsoil() {
  return averageMHsoil;
}

void GreenHouse::recordSoilTemperature() {
  sensors.requestTemperatures();
  soilTemperature = sensors.getTempCByIndex(0);
}

float GreenHouse::getRecordedSoilTemperature() {
  return soilTemperature;
}

void GreenHouse::recordAirTemperature() {
  airTemperature = dht.readTemperature();
}

void GreenHouse::recordAirHumidity() {
  airHumidity = dht.readHumidity();
}

float GreenHouse::getRecordedAirTemperature() {
  return airTemperature;
}

float GreenHouse::getRecordedAirHumidity() {
  return airHumidity;
}

void GreenHouse::displayWelcomeMessage() {
  display.clearScreen();

  display.setCursor(13, 20);
  display.setTextColor(GREEN);
  display.setTextSize(2);
  display.println("Automated");

  display.setCursor(8, 45);
  display.setTextColor(GREEN);
  display.setTextSize(2);
  display.println("GreenHouse");

  display.setCursor(18, 75);
  display.setTextColor(MAGENTA);
  display.setTextSize(1);
  display.println("@Unibuc Robotics");

  display.setCursor(53, 95);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("2020");
}

void GreenHouse::displayRecordedValues() {
  display.clearScreen();

  display.setCursor(13, 5);
  display.setTextColor(BLUE);
  display.setTextSize(1);
  display.println("Humidity:");

  // soil humidity (measured in VMC - volumetric moisture content)
  recordMHvalueLeft();
  recordMHvalueRight();
  calculateAverageMHsoil();
  display.setCursor(13, 20);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print(getAverageMHsoil());
  display.println("  VMC (soil)");
  Serial.print("humidity:  ");
  Serial.print(getAverageMHsoil());
  Serial.println("  (soil)");

  // air humidity (measured in %RH - relative humidity)
  recordAirHumidity();
  display.setCursor(13, 35);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print(getRecordedAirHumidity());
  display.println("  %RH (air)");
  Serial.print("humidity:  ");
  Serial.print(getRecordedAirHumidity());
  Serial.println("  (air)");

  display.setCursor(13, 55);
  display.setTextColor(BLUE);
  display.setTextSize(1);
  display.println("Temperature:");

  // soil temperature (measured in C - celsius degrees)
  recordSoilTemperature();
  display.setCursor(13, 70);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print(getRecordedSoilTemperature());
  display.println("  C (soil)");
  Serial.print("temperature:  ");
  Serial.print(getRecordedSoilTemperature());
  Serial.println("  (soil)");

  // air temperature (measured in C - celsius degrees)
  recordAirTemperature();
  display.setCursor(13, 85);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.print(getRecordedAirTemperature());
  display.print("  C (air)");
  Serial.print("temperature:  ");
  Serial.print(getRecordedAirTemperature());
  Serial.println("  (air)");

  // water level
  display.setCursor(13, 105);
  display.setTextColor(BLUE);
  display.setTextSize(1);
  display.print("Water level:");

  display.setCursor(15, 120);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("..% under constr.");
}

void GreenHouse::updateRecordedValues() {
  Serial.println("update recordings");

  // update soil humidity
  int previousAverageMHsoil = getAverageMHsoil();
  recordMHvalueLeft();
  recordMHvalueRight();
  calculateAverageMHsoil();
  int recordedAverageMHsoil = getAverageMHsoil();

  if (recordedAverageMHsoil != previousAverageMHsoil) {
    Serial.print("update soil humidity: ");
    Serial.println(recordedAverageMHsoil);

    display.setCursor(13, 20);
    display.fillRect(13, 20, 20, 10, BLACK);

    display.setCursor(13, 20);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println(recordedAverageMHsoil);
  }

  // update air humidity
  float previousAirHumidity = getRecordedAirHumidity();
  recordAirHumidity();
  float recordedAirHumidity = getRecordedAirHumidity();

  if (recordedAirHumidity != previousAirHumidity) {
    Serial.print("update air humidity: ");
    Serial.println(recordedAirHumidity);

    display.setCursor(13, 35);
    display.fillRect(13, 35, 40, 10, BLACK);

    display.setCursor(13, 35);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println(recordedAirHumidity);
  }

  // update soil temperature
  float previousSoilTemperature = getRecordedSoilTemperature();
  recordSoilTemperature();
  float recordedSoilTemperature = getRecordedSoilTemperature();

  if (recordedSoilTemperature != previousSoilTemperature) {
    Serial.print("update soil temperature: ");
    Serial.println(recordedSoilTemperature);

    display.setCursor(13, 70);
    display.fillRect(13, 70, 40, 10, BLACK);

    display.setCursor(13, 70);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println(recordedSoilTemperature);
  }

  // update air Temperature
  float previousAirTemperature = getRecordedAirTemperature();
  recordAirTemperature();
  float recordedAirTemperature = getRecordedAirTemperature();

  if (recordedAirTemperature != previousAirTemperature) {
    Serial.print("update air temperature: ");
    Serial.println(recordedAirTemperature);

    display.setCursor(13, 85);
    display.fillRect(13, 85, 40, 10, BLACK);

    display.setCursor(13, 85);
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.println(recordedAirTemperature);
  }

  // update water level
}

void GreenHouse::analizeRecordings() {
  Serial.println("analize recordings");

  // analize soil humidity
  if (averageMHsoil <= minDrynessThreshold && isWaterPumpOn()) {
    // soil fully watered => turn off water pump
    turnWaterPumpOff();
  }
  else if (averageMHsoil >= maxDrynessThreshold && !isWaterPumpOn()) {
    // soil fully dried => turn on water pump
    turnWaterPumpOn();
  }

  // analize air temperature and air humidity in this order of relevance
  if (airTemperature > maxAirTemperatureThreshold && !areBothTrapdoorsOpened())
    openBothTrapdoors();
  else if (airTemperature < minAirTemperatureThreshold && !areBothTrapdoorsClosed())
    closeBothTrapdoors();
  else {
    // air temperature is within normal parameters => analize air humidity
    if (airHumidity > maxAirHumidityThreshold) {
      // ventilation system for keeping humidity under maxThreshold: two open doors for keeping air moving
      // only when air temperature is within normal parameters
      if (!areBothTrapdoorsOpened())
        openBothTrapdoors();
    }
    else if (!areBothTrapdoorsClosed()) {
      // if both air temperature and air humidity are within normal parameters
      // we want to keep the greenHouse closed by default
      closeBothTrapdoors();
    }
  }

  // analize soil temperature (under construction)
  // for keeping water temperature at a constant level in the water tank
}
