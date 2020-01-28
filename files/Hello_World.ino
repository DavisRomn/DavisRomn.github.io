// SOCIALLY ANXIOUS DEPRESSED HELLO WORLD
#include <LiquidCrystal.h>

// Declare the pins needed for the lcd screen
LiquidCrystal lcd(22, 23, 35, 34, 33, 32);

// Declare the LED pins
int LEDpins[] = {2, 3, 4, 5, 6};

// Declare the pin used for the trig portion of the UT sensor
const int trigPin = 10;
// Declare the pin used for the echo portion of the UT sensor
const int echoPin = 11;

// Declare the pins that contain buttons
int buttonPins[] = {7};

// Global variable to save whether the system has been 'turned on' or not
bool toggledOn = false;

// Morse Code letters for A - Z
char* morseletters[] = {
".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", // A-I
".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", // J-R 
"...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.." // S-Z
};

// Sentence to be translated to morse code and printed to the lcd
char morseCodeSentence[] = {
  'H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D', '!', ' ', 'I', ' ', 'A', 'M', ' ', 'G', 'A', 'R', 'Y', ' ', 'T', 'H', 'E', ' ',
  'F', 'R', 'I', 'E', 'N', 'D', 'L', 'Y', ' ', 'R', 'O', 'B', 'O', 'T', ' ', 'A', 'N', 'D', ' ', 'I', ' ', 'C', 'O', 'M', 'E', ' ', 'I', 'N', ' ',
  'P', 'E', 'A', 'C', 'E', '!'
};

// Hidden sentence to be translated to morse code and printed to the lcd if the UT sensor works
char evilMorseCodeSentence[] = {
  'S', 'O', 'S', ' ', 'D', 'O', ' ', 'N', 'O', 'T', ' ', 'C', 'O', 'M', 'E', ' ', 'C', 'L' ,'O', 'S', 'E', 'R', '!' , ' ', 'G', 'A', 'R', 'Y', ' ', 'D', 'O', 'E', 'S', ' ', 'N', 'O', 'T', ' ',
  'L', 'I', 'K', 'E', ' ', 'T', 'O', ' ', 'B', 'E', ' ', 'T', 'O', 'O', ' ', 'C', 'L', 'O', 'S', 'E', ' ', 'T', 'O', ' ', 'F', 'L', 'E', 'S', 'H', 'B', 'A', 'G', 'S'
};

// Variable to save the current line that is being printed to
int currentLCDLine = 0;

// current position in the sentences being printed/translated
int morseCounter = 0;
int evilMorseCounter = 0;

void setup() {
  // Initialize the LED pins
  for (int i = 0; i < sizeof(LEDpins); i++) {
    pinMode(LEDpins[i], OUTPUT);
  }

  // Initialize the button pins
  for (int i = 0; i < sizeof(buttonPins); i++) {
    pinMode(buttonPins[i], INPUT);
  }

  // Initialize the pins for the UT sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize the lcd screen
  lcd.begin(10, 2);
  lcd.setCursor(0, 1);
}

void loop() {
  digitalWrite(trigPin, LOW); // clear Trig
  delayMicroseconds(2);

  //Take a UT Sample:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = (duration * 0.034) / 2;
  
  int toggleOn = digitalRead(7);
  
  // If the morse code has been finished, restart
  if (morseCounter == sizeof(morseCodeSentence)) {
    delay(2000);
    morseCounter = 0;
    lcd.clear();
  }

  // If the hidden morse code has been finished, restart
  if (evilMorseCounter == sizeof(evilMorseCodeSentence)) {
    delay(2000);
    evilMorseCounter = 0;
    lcd.clear();
  }

  // If the button has been pressed
  if (toggleOn == HIGH) {
    // If the system is already 'on', turn it off
    if (toggledOn) {
      toggledOn = false;
      morseCounter = 0;
      delay (1000);
    } else { // else turn it on
      toggledOn = true;
    }
  }

  // If the system is 'On'
  if (toggledOn){
    // If the UT sensor distance value is greater than 10 ( ~10 cm)
    if (distance > 10) {
      // IF the hidden morse code was previously being printed, reset the screen and morseCounter
      if (evilMorseCounter > 0) {
        lcd.clear();
        evilMorseCounter = 0;
      }
      // Get the current morseCounter letter (Char - Ascii difference)
      int letter = morseCodeSentence[morseCounter] - 65;
      // If the letter value is greater than zero (pertains to a real morse value)
      if (letter > 0) {
        // Grab the morse value of the current letter
        char* morseLetter = morseletters[letter];
        // Iterate through the char* of morseLetter
        for (int j = 0; j < strlen(morseletters[letter]); j++) {
          // If the current value is a dash, turn on all LEDS
          if (morseLetter[j] == '-') {
            LEDDash();
            delay(200);
            allLEDOff();
            delay(200);
          } else { // IF the current value is a dot, turn on only the middle LED
            LEDDot();
            delay(200);
            allLEDOff();
            delay(200);
          }
        }
        allLEDOff();
        delay(200);
      }

      // If 16 character have been printed to the lcd
      if (morseCounter % 16 == 0) {
        // If the lcd is on the bottom line, clear the screen, start at the top
        if (currentLCDLine == 1) {
          currentLCDLine = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
        } else { // else go to the next line
          currentLCDLine = 1;
          lcd.setCursor(0, 1);
        }
      }
      // Print the letter to the lcd
      lcd.print(morseCodeSentence[morseCounter]);  
      // Increment the counter
      morseCounter++;
    } else { // If the distance is less than 10
      // If the regular morse was previously being displayed, reset
      if (morseCounter > 0) {
        lcd.clear();
        morseCounter = 0;
      }

      // Get the current evilMorseCodeLetter
      int letter = evilMorseCodeSentence[evilMorseCounter] - 65;
      // If the letter is a valid morse value
      if (letter > 0) {
        // Translate and show proper LEDS
        char* morseLetter = morseletters[letter];
        for (int j = 0; j < strlen(morseletters[letter]); j++) {
          if (morseLetter[j] == '-') {
            LEDDash();
            delay(200);
            allLEDOff();
            delay(200);
          } else {
            LEDDot();
            delay(200);
            allLEDOff();
            delay(200);
          }
        }
        allLEDOff();
        delay(200);
      }

      // Check to ensure printing is only done on screen
      if (evilMorseCounter % 16 == 0) {
        if (currentLCDLine == 1) {
          currentLCDLine = 0;
          lcd.clear();
          lcd.setCursor(0, 0);
        } else {
          currentLCDLine = 1;
          lcd.setCursor(0, 1);
        }
      }
      lcd.print(evilMorseCodeSentence[evilMorseCounter]);  
      evilMorseCounter++;
    }
  }
}

// Function to turn on only middle LED
void LEDDot() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

// Function to turn off all LEDs
void allLEDOff() {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}

// Function to turn on all LEDs
void LEDDash() {
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
}
