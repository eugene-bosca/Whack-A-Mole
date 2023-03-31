// random number generator integer
long randNumber;

// declare const for interrupt pins
const int interruptPin = 2;
const int resetPin = 3;

// declare variable for counting game points
volatile int gamePoint = 0;

// flag for interrupt, volatile ensure that changes to the state variable are immediately visible in loop() 
volatile bool buttonPressed = false;

// initialize white LED pins
const int led1 = 12;
const int led2 = 11;
const int led3 = 10;
const int led4 = 9;

// initialize mole LED pins
const int mole1 = 7;
const int mole2 = 6;
const int mole3 = 5;
const int mole4 = 4;

// define state variables for the game
volatile bool gameRunning = false; // enable functions before game runs (flashy lights), set to TRUE after initial functions are done
bool moleActive = false; // true if a mole LED is set to HIGH
unsigned long lastMoleTime; // keeps track of when last mole was activated
unsigned long activeGameTime; // keeps track of how long the game has been running for

// stores resistor value when button is pressed from analog read (used to distinguish which button was pressed)
int buttonValue;

void setup() {
  // starts serial monitor
  Serial.begin(9600);

  // bit bashing to set prescalar to 256
  TCCR0B|= _BV(CS02);
  TCCR0B &= ~_BV(CS00);
  TCCR0B &= ~_BV(CS01); 

  // set white LED pins as output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);

  // initalize interrupt pins
  attachInterrupt(digitalPinToInterrupt(interruptPin), buttonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(resetPin), resetPress, RISING);
}

// performs really cool and totally not lame light sequence at beginning of game
void startLights() {
  Serial.println("really cool and totally not lame light sequence ON");

  // ensures all lights on breadboard are LOW
  molesOff();
  buttonsOff();
  delay(500);

  // run through LED pattern sequence
  digitalWrite(led1, HIGH);
  delay(100);
  buttonsOff();
  digitalWrite(led2, HIGH);
  delay(100);
  buttonsOff();
  digitalWrite(led3, HIGH);
  delay(100);
  buttonsOff();
  digitalWrite(led4, HIGH);
  delay(100);
  buttonsOff();
  delay(400);

  // flashing of lights part of seqeunce
  int i = 0;
  while(i <= 2) {
    buttonsOn();
    delay(200);   
    buttonsOff();
    delay(200);
    i++;
  }
}

// turns all buttons off
void buttonsOff() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}

// turns all buttons on
void buttonsOn() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
}

// turns all moles off
void molesOff() {
  digitalWrite(mole1, LOW);
  digitalWrite(mole2, LOW);
  digitalWrite(mole3, LOW);
  digitalWrite(mole4, LOW);
}

// mole button interrupt code
void buttonPress() {
  // set flag to true
  buttonPressed = true;

  // read value of resistance and store it
  buttonValue = analogRead(A0);

  // print to serial monitor resistance detected on the button press
  Serial.println("BUTTON VALUE IS:");
  Serial.println(buttonValue);
}

// interrupt for reseting game
void resetPress() {
  // button debounce variables
  static unsigned long lastInterruptTime = 0; // used to store last time interrupt occured
  unsigned long interruptTime = 4*millis(); // records the last time

  if (interruptTime - lastInterruptTime > 200) { //debounce
    Serial.println("RESETTING GAME");
    gameRunning = false; // set game flag to false to run through game intialization again in loop()
  }

  lastInterruptTime = interruptTime; // Update last time interrupt occured 
}

// initalize time in-between moles, speeds up as player gets game points
int moleInterval() {
  int speedUpInterval = 3000 - 50*gamePoint;
  return speedUpInterval;
}

// initalize time moles stay active (LED stays active), speeds up with number of game points
int activeMoleTime() {
  int speedUpActiveTime = 4000 - 100*gamePoint;
  return speedUpActiveTime;
}

// logic for the timer lights
void gameRunningTimerLights() {
  long howLong = 4*millis() - activeGameTime; // variable that gets how long the game has been running

  // makes one light turn off for every 10 seconds the game has been running for
  if (howLong<=10000){
    buttonsOn();
  }
  else if (howLong>=10001&& howLong<=20000) {
    digitalWrite(led4, LOW);
  }
  else if (howLong>=20001&& howLong<=30000) {
    digitalWrite(led3, LOW);
  }
  else if (howLong>=30001&& howLong<=40000) {
    digitalWrite(led2, LOW);
  }
  else{
    // time has run out in the game, final score is printed and game restarts
    gameRunning = false;
    Serial.print("YOUR FINAL GAME SCORE IS: ");
    Serial.println(gamePoint);
  }
  
}

void loop() {
  // Game setup
  if (!gameRunning) {
    gameRunning = true;
    gamePoint = 0;
    startLights();
    Serial.println("Starting game");
    activeGameTime = 4*millis();
  }
  else{
    // Game is running
    gameRunningTimerLights();
    
    if (!moleActive && 4*millis() - lastMoleTime > moleInterval()) { // note that moleInterval is how long mole stays OFF between two moles
      // Random number generator for random mole to come up
      randNumber = random(4);

      //Cases to activate the moles based on random number
      switch(randNumber) {
        case 0:
        digitalWrite(mole1, HIGH);
        digitalWrite(mole2, LOW);
        digitalWrite(mole3, LOW);
        digitalWrite(mole4, LOW);
        break;
        case 1:
        digitalWrite(mole2, HIGH);
        digitalWrite(mole1, LOW);
        digitalWrite(mole3, LOW);
        digitalWrite(mole4, LOW);
        break;
        case 2:
        digitalWrite(mole3, HIGH);
        digitalWrite(mole1, LOW);
        digitalWrite(mole2, LOW);
        digitalWrite(mole4, LOW);
        break;
        case 3:
        digitalWrite(mole4, HIGH);
        digitalWrite(mole1, LOW);
        digitalWrite(mole2, LOW);
        digitalWrite(mole3, LOW);
        break;
      }

      // Print game info to serial monitor
      Serial.print("Mole ");
      Serial.print(randNumber+1); // plus one because of indexing
      Serial.println(" is active ");

      // Set flag to active
      moleActive = true;

      // Timer for how long the mole has been active
      lastMoleTime = 4*millis();
     
    }
    else if (moleActive && 4*millis() - lastMoleTime > activeMoleTime()) {
      // Mole is active but player is out of time to hit it

      // Change flag to inactive and turn all moles off
      moleActive = false;
      molesOff();
      lastMoleTime = 4*millis();
    }
    // logic if mole interrupt is pressed
    if(buttonPressed && moleActive == true) {
      buttonValue = analogRead(A0);
  
      // logic for distinguishing between which button is pressed
      // For 1st button:
      if (buttonValue>=535 && buttonValue<=555){
        if(randNumber == 0) {gamePoint++;} // increase game point if correct button was pressed
        Serial.println("FIRST BUTTON gamepoint is:");
        Serial.println(gamePoint);
        molesOff(); // turn all moles off
        moleActive = false; // reset the moles
      }
      //For 2nd button:
      else if (buttonValue>=522&& buttonValue<=530){
        if(randNumber == 1) {gamePoint++;}
        Serial.println("SECOND BUTTON gamepoint is:");
        Serial.println(gamePoint);
        molesOff();
        moleActive = false;
      }
      //For 3rd button:
      else if (buttonValue>=515 && buttonValue<= 520){
        if(randNumber == 2) {gamePoint++;}
        Serial.println("THIRD BUTTON gamepoint is:");
        Serial.println(gamePoint);
        molesOff();
        moleActive = false;
      }
      //For 4th button:
      else if (buttonValue>=510 && buttonValue<=514){
        if(randNumber == 3) {gamePoint++;}
        Serial.println("FOURTH BUTTON gamepoint is:");
        Serial.println(gamePoint);
        molesOff();
        moleActive = false;
      }

      // turn flag off after button is identified
      buttonPressed = false;
    }
  }
}
