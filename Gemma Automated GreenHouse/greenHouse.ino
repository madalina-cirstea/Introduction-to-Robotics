#include <IRremote.h>
#include "greenHouse.h"

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

GreenHouse greenHouse;

const unsigned int timeBetweenRemoteSignals = 100;
unsigned long lastRemoteSignal = 0;

const unsigned int timeBetweenRecordings = 5000;
unsigned long lastRecording = 0;

const unsigned long timeBetweenUpdates = 6000;
unsigned long lastUpdate = 0;

void validateCode() {
  switch (results.value) {
    case 0xFFA25D:
      // CH-
      Serial.println("Manual Mode");
      greenHouse.setManualMode();
      break;

    case 0xFFE21D:
      // CH+
      Serial.println("Automatic Mode");
      greenHouse.setAutomaticMode();
      break;

    case 0xFF22DD:
      // |<<
      if (greenHouse.onManualMode()) {
        if (greenHouse.isWaterPumpOn()) {
          Serial.println("water pump ALREADY turned on");
        }
        else {
          Serial.println("Water pump turned ON");
          greenHouse.turnWaterPumpOn();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break;

    case 0xFFC23D:
      // >|
      if (greenHouse.onManualMode()) {
        if (!greenHouse.isWaterPumpOn()) {
          Serial.println("water pump ALREADY turned off");
        }
        else {
          Serial.println("Water pump turned OFF");
          greenHouse.turnWaterPumpOff();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break;

    case 0xFFE01F:
      // -
      if (greenHouse.onManualMode()) {
        if (greenHouse.isTrapdoorFrontOpened()) {
          Serial.println("trapdoor front ALREADY opened");
        }
        else {
          Serial.println("Trapdoor front Opened");
          greenHouse.openTrapdoorFront();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break;

    case 0xFF906F:
      // EQ
      if (greenHouse.onManualMode()) {
        if (!greenHouse.isTrapdoorFrontOpened()) {
          Serial.println("trapdoor front ALREADY closed");
        }
        else {
          Serial.println("Trapdoor front Closed");
          greenHouse.closeTrapdoorFront();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break;

    case 0xFF6897:
      // 0
      if (greenHouse.onManualMode()) {
        if (greenHouse.isTrapdoorBackOpened()) {
          Serial.println("trapdoor back ALREADY opened");
        }
        else {
          Serial.println("Trapdoor back Opened");
          greenHouse.openTrapdoorBack();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break ;

    case 0xFFB04F:
      // 200+
      if (greenHouse.onManualMode()) {
        if (!greenHouse.isTrapdoorBackOpened()) {
          Serial.println("trapdoor back ALREADY closed");
        }
        else {
          Serial.println("Trapdoor back Closed");
          greenHouse.closeTrapdoorBack();
        }
      }
      else {
        Serial.println("Switch to Manual Mode");
      }
      break ;

    default:
      if (results.value != 0XFFFFFFFF) {
        Serial.println("Key unknown");
      }
      else {
        Serial.println("Press");
      }
      break;
  }
}

void setup() {
  Serial.begin(9600);
  greenHouse.display.begin();
  irrecv.enableIRIn();

  greenHouse.trapdoorFront.attach(greenHouse.getTrapdoorFrontPin());
  greenHouse.trapdoorBack.attach(greenHouse.getTrapdoorBackPin());

  greenHouse.displayWelcomeMessage();
  delay(4000);
  greenHouse.displayRecordedValues();
}

void loop() {
  // catch remote signal
  if (millis() - lastRemoteSignal >= timeBetweenRemoteSignals) {
    if (irrecv.decode(&results)) {
      validateCode();
      irrecv.resume();
    }
    lastRemoteSignal = millis();
  }

  // update sensors' values
  if (millis() - lastUpdate >= timeBetweenUpdates) {
    greenHouse.updateRecordedValues();
    lastUpdate = millis();
  }

  // automatic mode
  if (!greenHouse.onManualMode())
    if (millis() - lastRecording >= timeBetweenRecordings) {
      greenHouse.analizeRecordings();
      lastRecording = millis();
    }
}
