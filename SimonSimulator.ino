/*
  JOHN SEONG'S SIMON SIMULATOR 2021
  COPYRIGHT (C) 2021 STUDIO HORIZON. ALL RIGHTS RESERVED.
*/

// The Star-Spangled Banner
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784

// Define melody
int melody[] = {
  NOTE_G4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4,
  NOTE_G4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_E4, NOTE_FS4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4,
  NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_E4, NOTE_FS4, NOTE_G4,
  NOTE_G4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_D5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_D5, NOTE_C5
};

// Define the durations between notes
int noteDurations[] = {
  6, 8, 4, 4, 4, 2, 6, 8, 4, 4, 4, 2, 8, 8, 3, 8, 4, 2, 6, 8, 4, 4, 4, 4, 4,
  6, 8, 4, 4, 4, 2, 6, 8, 4, 4, 4, 2, 8, 8, 3, 8, 4, 2, 6, 8, 4, 4, 4, 4, 4,
  8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 4, 3, 8, 4, 2, 6, 8, 4, 4, 4, 2,
  4, 4, 4, 8, 8, 4, 4, 4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 3, 8, 8, 8, 2, 8, 8, 3, 8, 4, 2
};

// Set pins
const int buzzer = 2;

const int latchPin = 11;      // (11) ST_CP [RCK] on 74HC595
const int clockPin = 9;      // (9) SH_CP [SCK] on 74HC595
const int dataPin = 12;     // (12) DS [S1] on 74HC595

const int button1 = 8;
const int button2 = 7;
const int button3 = 6;
const int button4 = 5;

/*
  Each LED output and button input has its own ID, which is a single digit of number:
  GREEN LED/BUTTON 1
  RED LED/BUTTON 2
  YELLOW LED/BUTTON 3
  BLUE LED/BUTTON 4
*/

// Variables that store 1 if the button is pressed and released
int button1Count = 0;
int button2Count = 0;
int button3Count = 0;
int button4Count = 0;

// How many times the button was pressed
int buttonSequence = 0;

// output[]: the pattern of LEDs that the user has to input
// input[]: this is where the user input is stored
int output[] = {0, 0, 0, 0};
int input[] = {0, 0, 0, 0};

// Check if the output and input are the same
int inputConditions[] = {0, 0, 0, 0};

byte leds = 0;

// Update the shift register and send a signal to it
void updateShiftRegister() {

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, leds); // MSBFIRST starts from the most significant byte
  digitalWrite(latchPin, HIGH);
}

void setup() {

  // Begin serial monitor
  Serial.begin(9600);

  // Set pinMode
  pinMode(buzzer, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  // Display welcome message
  leds = B1001;
  updateShiftRegister();
  delay(300);

  leds = B0110;
  updateShiftRegister();
  delay(300);

  leds = B1001;
  updateShiftRegister();
  delay(300);

  leds = B0110;
  updateShiftRegister();
  delay(300);

  leds = B0110;
  updateShiftRegister();
  delay(300);

  leds = B1001;
  updateShiftRegister();
  delay(300);

  leds = B0000;
  updateShiftRegister();
  delay(500);
}

void loop() {

  // Define levels and the state that the program will start on
  static enum {INTRO, LEVEL1, POST_LEVEL1, LEVEL2, POST_LEVEL2, LEVEL3, POST_LEVEL3, LEVEL4, POST_LEVEL4, LEVEL5, POST_LEVEL5, LEVEL6, POST_LEVEL6, CONGRATS} state = INTRO;

  // Read the buttons' states
  int button1State = digitalRead(button1);
  int button2State = digitalRead(button2);
  int button3State = digitalRead(button3);
  int button4State = digitalRead(button4);

  switch (state) {

    case INTRO:

      // Turn off all the LEDs
      allLedsOff();

      // If the button is pressed then set button1Count to 1
      if (button1State == 0) {
        button1Count = 1;
      }

      // If button1Count is 1 then turn off all the LEDs and set the state to LEVEL1
      if (button1Count == 1) {
        allLedsOff();
        state = LEVEL1;

        // If the button1Count is 0 then blink the green LED
      } else if (button1Count == 0) {
        greenLedOn(500);
        delay(500);
      }

      break;

    // This is where a certain pattern of LEDs flash
    case LEVEL1:

      // reset all the stored values so when the program moves on to the next level, everything starts afresh
      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      button1Count = 0;

      // Turn off all the LEDs
      allLedsOff();

      delay(1000);

      // The LEDs are going to flash with the delay of 1000 milliseconds
      yellowLedOn(1000); // 3
      greenLedOn(1000); // 1
      redLedOn(1000); // 2
      blueLedOn(1000); // 4

      // Store the pattern of LEDs in the output[] array
      output[0] = 3;
      output[1] = 1;
      output[2] = 2;
      output[3] = 4;

      // Move on to the next state
      state = POST_LEVEL1;

      break;

    // This is where the program reads user's input
    case POST_LEVEL1:

      // Set the buttonCount to 1 if the buttons are pressed
      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      // If any of the buttons are pressed then add 1 to the buttonSequence variable
      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      // If the button has ever been pressed then turn on the LED that is represented by the button, reset the buttonCount variable, and send the ID of the button to the enterButtonValue(int value) function
      if (button1Count == 1) {
        greenLedOn(1000);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(1000);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(1000);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(1000);
        button4Count = 0;
        enterButtonValue(4);
      }

      // Compare two arrays: input[] and output[] ; Check if the user has entered the correct pattern
      for (int i = 0; i < 4; i++) {
        // Compare every single value in two arrays and store 1 if they match
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      // If all the values in two arrays match, move on to the next level
      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = LEVEL2;
      }

      break;

    /*
      The codes below this line are essentially the same, except for the speed and the pattern of the LEDs that flash.
      The remarks are only added in the functions below all the case statements and the last state, which lets the user know that they have completed all the levels.
    */

    case LEVEL2:

      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      allLedsOff();

      delay(1000);

      greenLedOn(1000); // 1
      blueLedOn(1000); // 4
      redLedOn(1000); // 2
      yellowLedOn(1000); // 3

      output[0] = 1;
      output[1] = 4;
      output[2] = 2;
      output[3] = 3;

      state = POST_LEVEL2;

      break;

    case POST_LEVEL2:

      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      if (button1Count == 1) {
        greenLedOn(1000);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(1000);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(1000);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(1000);
        button4Count = 0;
        enterButtonValue(4);
      }

      for (int i = 0; i < 4; i++) {
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = LEVEL3;
      }

      break;

    case LEVEL3:

      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      allLedsOff();

      delay(800);

      redLedOn(800); // 2
      yellowLedOn(800); // 3
      greenLedOn(800); // 1
      blueLedOn(800); // 4

      output[0] = 2;
      output[1] = 3;
      output[2] = 1;
      output[3] = 4;

      state = POST_LEVEL3;

      break;

    case POST_LEVEL3:

      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      if (button1Count == 1) {
        greenLedOn(800);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(800);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(800);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(800);
        button4Count = 0;
        enterButtonValue(4);
      }

      for (int i = 0; i < 4; i++) {
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = LEVEL4;
      }

      break;

    case LEVEL4:

      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      allLedsOff();

      delay(800);

      blueLedOn(800); // 4
      yellowLedOn(800); // 3
      greenLedOn(800); // 1
      redLedOn(800); // 2

      output[0] = 4;
      output[1] = 3;
      output[2] = 1;
      output[3] = 2;

      state = POST_LEVEL4;

      break;

    case POST_LEVEL4:

      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      if (button1Count == 1) {
        greenLedOn(800);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(800);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(800);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(800);
        button4Count = 0;
        enterButtonValue(4);
      }

      for (int i = 0; i < 4; i++) {
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = LEVEL5;
      }

      break;

    case LEVEL5:

      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      allLedsOff();

      delay(600);

      yellowLedOn(600); // 3
      greenLedOn(600); // 1
      blueLedOn(600); // 4
      redLedOn(600); // 2

      output[0] = 3;
      output[1] = 1;
      output[2] = 4;
      output[3] = 2;

      state = POST_LEVEL5;

      break;

    case POST_LEVEL5:

      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      if (button1Count == 1) {
        greenLedOn(600);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(600);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(600);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(600);
        button4Count = 0;
        enterButtonValue(4);
      }

      for (int i = 0; i < 4; i++) {
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = LEVEL6;
      }

      break;

    case LEVEL6:

      resetInputConditions();
      resetOutput();
      resetInput();

      buttonSequence = 0;

      allLedsOff();

      delay(400);

      greenLedOn(400); // 1
      redLedOn(400); // 2
      blueLedOn(400); // 4
      yellowLedOn(400); // 3

      output[0] = 1;
      output[1] = 2;
      output[2] = 4;
      output[3] = 3;

      state = POST_LEVEL6;

      break;

    case POST_LEVEL6:

      if (button1State == 0) {
        button1Count = 1;
      }

      if (button2State == 0) {
        button2Count = 1;
      }

      if (button3State == 0) {
        button3Count = 1;
      }

      if (button4State == 0) {
        button4Count = 1;
      }

      if (button1Count == 1 || button2Count == 1 || button3Count == 1 || button4Count == 1) {
        buttonSequence++;
      }

      if (button1Count == 1) {
        greenLedOn(400);
        button1Count = 0;
        enterButtonValue(1);
      }

      if (button2Count == 1) {
        redLedOn(400);
        button2Count = 0;
        enterButtonValue(2);
      }

      if (button3Count == 1) {
        yellowLedOn(400);
        button3Count = 0;
        enterButtonValue(3);
      }

      if (button4Count == 1) {
        blueLedOn(400);
        button4Count = 0;
        enterButtonValue(4);
      }

      for (int i = 0; i < 4; i++) {
        if (input[i] == output[i]) {
          inputConditions[i] = 1;
        }
      }

      if (inputConditions[0] == 1 && inputConditions[1] == 1 && inputConditions[2] == 1 && inputConditions[3] == 1) {
        state = CONGRATS;
      }

      break;

    // CONGRATS! You have completed all the levels.
    case CONGRATS:

      // Flash all the LEDs
      leds = B1111;
      updateShiftRegister();

      // Play The Star-Spangled Banner
      for (int thisNote = 0; thisNote < 101; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(buzzer, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.40;
        delay(pauseBetweenNotes);
        noTone(buzzer);
      }

      // Turn off all the LEDs
      allLedsOff();

      delay(3000);

      // Move on to the first state
      state = INTRO;

      break;
  }

  // This delay is for the simulation to run flawlessly.
  delay(50);
}

// Turn on the green LED and make a sound
void greenLedOn(int interval) {

  leds = B0001;
  updateShiftRegister();
  tone(buzzer, 500);
  delay(interval);

  leds = B0000;
  updateShiftRegister();
  noTone(buzzer);
}

// Turn on the red LED and make a sound
void redLedOn(int interval) {

  leds = B0010;
  updateShiftRegister();
  tone(buzzer, 1000);
  delay(interval);

  leds = B0000;
  updateShiftRegister();
  noTone(buzzer);
}

// Turn on the yellow LED and make a sound
void yellowLedOn(int interval) {

  leds = B0100;
  updateShiftRegister();
  tone(buzzer, 1500);
  delay(interval);

  leds = B0000;
  updateShiftRegister();
  noTone(buzzer);
}

// Turn on the blue LED and make a sound
void blueLedOn(int interval) {

  leds = B1000;
  updateShiftRegister();
  tone(buzzer, 2000);
  delay(interval);

  leds = B0000;
  updateShiftRegister();
  noTone(buzzer);
}

// Turn off all the LEDs
void allLedsOff() {

  leds = B0000;
  updateShiftRegister();
}

// This function determines if the button is pressed for the first time or how many times the user has pressed other buttons or the button the user just pressed before
void enterButtonValue(int value) {

  if (buttonSequence == 1) {
    input[0] = value;
  }

  if (buttonSequence == 2) {
    input[1] = value;
  }

  if (buttonSequence == 3) {
    input[2] = value;
  }

  if (buttonSequence == 4) {
    input[3] = value;

    // If the button is pressed more than four times, display an error message and reset the values that the user has input
  } else if (buttonSequence >= 5) {
    displayError();
    resetInputConditions();
    resetInput();

    buttonSequence = 0;
  }
}

// This function is for displaying an error message
void displayError() {

  // Turn on all the LEDs and make a sound
  leds = B1111;
  updateShiftRegister();
  tone(buzzer, 2500);
  delay(1000);

  // Turn off all the LEDs and cut the sound
  allLedsOff();
  noTone(buzzer);
}

// Reset all the values in the output[] array
void resetOutput() {

  output[0] = 0;
  output[1] = 0;
  output[2] = 0;
  output[3] = 0;
}

// Reset all the values in the input[] array
void resetInput() {

  input[0] = 0;
  input[1] = 0;
  input[2] = 0;
  input[3] = 0;
}

// reset all the values in the inputConditions[] array
void resetInputConditions() {

  inputConditions[0] = 0;
  inputConditions[1] = 0;
  inputConditions[2] = 0;
  inputConditions[3] = 0;
}
