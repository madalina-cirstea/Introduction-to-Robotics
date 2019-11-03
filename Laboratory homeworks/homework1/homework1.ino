const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;

const int ledRed = 9;
const int ledGreen = 10;
const int ledBlue = 11;

int potValueRed = 0;
int potValueGreen = 0;
int potValueBlue = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  pinMode(potPinRed,INPUT);
  pinMode(potPinGreen,INPUT);
  pinMode(potPinBlue,INPUT);

  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
  pinMode(ledBlue,OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  potValueRed = analogRead(potPinRed);
  potValueGreen = analogRead(potPinGreen);
  potValueBlue = analogRead(potPinBlue);

  redValue = map(potValueRed, 0, 1023, 0, 255);
  greenValue = map(potValueGreen, 0, 1023, 0, 255);
  blueValue = map(potValueBlue, 0, 1023, 0, 255);

  Serial.print("Red: ");
  Serial.print(redValue);
  Serial.print(" Green: ");
  Serial.print(greenValue);
  Serial.print(" Blue: ");
  Serial.println(blueValue);

  analogWrite(ledRed,redValue);
  analogWrite(ledGreen,greenValue);
  analogWrite(ledBlue,blueValue);

  delay(10);

}
