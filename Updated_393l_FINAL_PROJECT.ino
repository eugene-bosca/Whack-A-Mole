// random number generator integer
long randNumber;

// declare const for interrupt pin
const int interruptPin = 2;

// declare variable for counting game point
volatile int gamePoint = 0;

// flag for interrupt, volatile ensure that changes to the state variable are immediately visible in loop() 
volatile bool buttonPressed = false;

// const for checking interrupt
const int led1 = 12;
const int led2 = 11;
const int led3 = 10;
const int led4 = 9;

// const for mole led
const int mole1 = 7;
const int mole2 = 6;
//const int mole3 = 5;
//const int mole4 = 4;

// Define state variables for the game
bool gameRunning = false; // enable functions before game runs (flashy lights), set to TRUE after initial functions are done
bool moleActive = false;
unsigned long lastMoleTime; // keeps track of when last mole was activated

// variables
int buttonValue; // stores resistor value when button is pressed (used to indicate which button was pressed)

// how long the mole is active for
const int moleInterval = 1000; // interval BETWEEN two moles
const int activeMoleTime = 10000; // how long a mole LED is ON before turning off

void setup() {
  Serial.begin(9600); // Starts serial monitor
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPress, RISING);
}

// Interrupt code
void buttonPress() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200) {
    buttonPressed = true;
    Serial.print("Interrupt pressed");
  }
  lastInterruptTime = interruptTime;
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

// Turns all buttons off
void buttonsOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}

// Turns all buttons on
void buttonsOn() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
}

// Turns all moles off
void molesOff() {
  digitalWrite(mole1, LOW);
  digitalWrite(mole2, LOW);
  //digitalWrite(mole3, LOW);
  //digitalWrite(mole4, LOW);
}

void checkWhichButtonPressed (){
  buttonValue = analogRead(A0); //Read analog value from A0 pin
  
  //For 1st button:
  if (buttonValue>=500 && buttonValue<=550 && mole1 == HIGH){
    gamePoint++;
    moleActive = false;
    Serial.print("FIRST BUTTON gamepoint is:");
    Serial.println(gamePoint);
  }
  //For 2nd button:
  else if (buttonValue>=300 && buttonValue<=350 && mole2 == HIGH){
    gamePoint++;
    moleActive = false;
    Serial.print("SECOND BUTTON gamepoint is:");
    Serial.println(gamePoint);
  }
  /*else {
    moleActive = false;
    Serial.print("ELSE STATEMENT:");
    Serial.println(gamePoint);
  } */
}

void loop() {
  if (buttonPressed){
    checkWhichButtonPressed();
    buttonPressed = false;
    Serial.print("In button pressed statement");
  }
  // Game setup
  if (!gameRunning) {
    // Waits for all the buttons to be released so interrupt cannot occur before game starts
    //while (digitalRead(led1) || digitalRead(led2) || digitalRead(led3) || digitalRead(led4)) {}
    gameRunning = true;
    gamePoint = 0;
    //startLights();
    Serial.println("Starting game");
  }
  else{
    // Game is running
    if (!moleActive && millis() - lastMoleTime > moleInterval) { // note that moleInterval is how long mole stays OFF between two moles
      // Random number generator for random mole to come up
      Serial.println(millis());
      Serial.println(lastMoleTime);
      Serial.println(moleInterval);
      int randNumber = random(2);

      //Cases to activate the mole
      switch(randNumber) {
        case 0:
        digitalWrite(mole1, HIGH);
        digitalWrite(mole2, LOW);
        break;
        case 1:
        digitalWrite(mole2, HIGH);
        digitalWrite(mole1, LOW);
        break;
      }

      // Print game info to serial monitor
      Serial.print("Mole ");
      Serial.print(randNumber+1);
      Serial.println(" is active ");

      // Set flag to active
      moleActive = true;

      // Timer for how long the mole has been active
      lastMoleTime = millis();
      Serial.print(millis());
    }
    else if (moleActive && millis() - lastMoleTime > activeMoleTime) {
      // Mole is active but player is out of time to hit it

      // Change flag to inactive and turn all moles off
      moleActive = false;
      molesOff();
      lastMoleTime = millis();
      //Serial.print(lastMoleTime);
    }
  }
}
