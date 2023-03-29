long randNumber;

// Const for interrupt pin
const int interruptPin = 2;

// volatile int gamePoint = 0;
volatile state = HIGH;

// Const for checking interrupt
const int led1 = 12;
const int led2 = 11;
const int led3 = 10;
const int led4 = 9;

// Const for mole led
const int mole1 = 7;
const int mole2 = 6;
const int mole3 = 5;
const int mole2 = 4;

// Define state variables for the game
bool gameRunning = false;
bool moleActive = false; 

// Variables
int buttonValue; // stores analog value when button is pressed

// Mole variables
volatile int currentMole = -1;
unsigned long lastMoleTime = 0;
const int moleInterval = 2000;

void setup() {
  Serial.begin(9600); // Starts serial monitor
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);


  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPress, RISING);
}

void loop() {
  // Game setup
  if (!gameRunning) {
    // Game is not running, wait for button press to start
    // Waits for all the buttons to be released so interrupt cannot occur before game starts
    while (digitalRead(mole1) || digitalRead(mole2) || digitalRead(mole3) || digitalRead(mole4)) {}
    gameRunning = true;
    int score = 0;
    //startLights();
    Serial.println("Starting game");
  }
  else{
    // Game is running
    if(!moleActive && millis() - lastMoleTime > moleInterval) {
      // Checks whether mole is active AND within mole interval (while LED is on)
      int randNumber = random(2);
      moleActive = true;
      switch(randNumber) {
        case 0:
        digitalWrite(mole1, HIGH);
        digitalWrite(mole2, LOW);
      }
      lastMoleTime = millis();
    }
  }

}

// Interrupt code
void buttonPress() {
  if(gameRunning && moleActive) {
    
  }
}

// all the LEDs are OFF
 void buttonsOff() {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
 }

// all the LEDs are ON
 void buttonsOn() {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
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
