long randNumber;

// const for interrupt pin
const int interruptPin = 2;

volatile int gamePoint = 0;

// const for checking interrupt
const int led1 = 12;
const int led2 = 11;
const int led3 = 10;
const int led4 = 9;

// const for mole led
const int mole1 = 7;
const int mole2 = 6;

// variables
int buttonValue; // stores analog value when button is pressed

void setup() {
  Serial.begin(9600); // Starts serial monitor
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPress, RISING);

  //startLights();
  
}

void loop() {

}

void buttonPress() {
  
}


// Performs really cool and totally not lame light sequence at beginning of game
void startLights() {
  digitalWrite(led1, HIGH);
  delay(800);
  buttonsOff();
  digitalWrite(led2, HIGH);
  delay(800);
  buttonsOff();
  digitalWrite(led3, HIGH);
  delay(800);
  buttonsOff();
  digitalWrite(led4, HIGH);
  delay(800);
  buttonsOff();
  delay(800);
  int i = 0;
  while(i <= 2) {
    buttonsOn();
    delay(1000);   
    buttonsOff();
    delay(500);
    i++;
  }
}
