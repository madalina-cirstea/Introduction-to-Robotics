const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

const int ledPinRed = 9;
const int ledPinGreen = 10;
const int ledPinBlue = 11;

int potValueRed = 0;
int potValueGreen = 0;
int potValueBlue = 0;

int ledValueRed = 0;
int ledValueGreen = 0;
int ledValueBlue = 0;

void setup() {
  pinMode(potPinRed, INPUT);
  pinMode(potPinGreen, INPUT);
  pinMode(potPinBlue, INPUT);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinBlue, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  potValueRed = analogRead(potPinRed);
  potValueGreen = analogRead(potPinGreen);
  potValueBlue = analogRead(potPinBlue);

  ledValueRed = map(potValueRed, 0, 1023, 0, 255);
  ledValueGreen = map(potValueGreen, 0, 1023, 0, 255);
  ledValueBlue = map(potValueBlue, 0, 1023, 0, 255);

  printLedValues(ledValueRed, ledValueGreen, ledValueBlue);

  setLedColor(ledValueRed, ledValueGreen, ledValueBlue);
}

void printLedValues(int red, int green, int blue) {
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" Green: ");
  Serial.print(green);
  Serial.print(" Blue: ");
  Serial.println(blue);
}

void setLedColor(int red, int green, int blue) {
  analogWrite(ledPinRed, red);
  analogWrite(ledPinGreen, green);
  analogWrite(ledPinBlue, blue);
}
