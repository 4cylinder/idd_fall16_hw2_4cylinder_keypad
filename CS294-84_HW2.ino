/*
 Name: T.S. (Tzuo Shuin) Yew 
 Student Number: 3032165826
 
 Homework Assignment 2 for Interactive Device Design - Text Entry Device
 
 Use five buttons connected to RedBear Duo microcontroller
 to make a text entry device.
 Buttons are set up as a directional pad and an enter button to 
 scroll through an on-screen keyboard like in video games.

 Blue LED on pin D7 blinks every time a button is pressed for feedback confirmation.
 */

#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL); 
#endif

// all buttons are connected to pins D0 to D4, and to ground
int upButton = D4;
int leftButton = D3;
int enterButton = D2;
int rightButton = D1;
int downButton = D0;
// internal LED on pin D7
int LED = D7;

// global vars to track cursor position and value of entered text
// updated as buttons are pressed
int row = 0, col = 0;
String enteredText = "";

// the onscreen keyboard to be printed on serial terminal
// note: the '<' in the bottom right is a BACKSPACE
char grid[6][5] = {{'a','b','c','d','e'},
  {'f','g','h','i','j'},
  {'k','l','m','n','o'},
  {'p','q','r','s','t'},
  {'u','v','w','x','y'},
  {'z',' ','.',',','<'}};

void setup() {
  Serial.begin(9600);
  printKeyboard();
  pinMode(LED, OUTPUT); // LED on pin D7 is output
  // all connected buttons use the built-in pullup resistor on the RedBear Duo
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(enterButton, INPUT_PULLUP);
}

void loop() {
  // listen for inputs on all four buttons
  // only one button press is recognized at any one time
  buttonListener(upButton);
  buttonListener(downButton);
  buttonListener(leftButton);
  buttonListener(rightButton);
  buttonListener(enterButton);
}

// helper function to process a button press
void buttonListener(int button) {
  // blink the LED as long as the button is pressed
  while(digitalRead(button) == LOW) {
    // turn on LED as feedback to show the button press was registered
    digitalWrite(LED, HIGH);
    // up button decrements row value, down button increments it
    // left button decrements column value, right button increments it
    // implement wraparound, i.e. pressing left when at the leftmost column will move cursor to rightmost column
    if (button==upButton) {
      row -= 1;
      if (row < 0) row = 5;
    } else if (button==downButton) {
      row += 1;
      if (row > 5) row = 0;
    } else if (button==leftButton) {
      col -= 1;
      if (col < 0) col = 4;
    } else if (button==rightButton) {
      col += 1;
      if (col > 4) col = 0;
    } else if (button==enterButton) {
      // if the backspace character was NOT chosen
      if (!(row==5 && col==4)) {
        enteredText = String(enteredText + grid[row][col]);
      } else { // otherwise, strip the last char
        if (enteredText.length()>=1)
          enteredText.remove(enteredText.length()-1,1);
      }
    }
    // clear the serial monitor output and redraw the keyboard, current selection, and entered text
    // gives illusion of real-time updates
    Serial.print("\033[0H\033[0J");
    printKeyboard();
    printSelected();
    printEntered();
    // 200 millisecond delay to compensate for debouncing
    delay(200);
    // turn off LED after button released
    digitalWrite(LED, LOW);
    // Another 200 millisecond delay to compensate for debouncing
    delay(200);
  }
}

// helper function to print onscreen keyboard
void printKeyboard() {
  Serial.print("###### KEYBOARD ######\r\n");
  for (int i=0;i<6;i++){
    for (int j=0;j<5;j++){
      Serial.print(grid[i][j]);
      Serial.print(" ");
    }
    Serial.print("\r\n");
  }
  Serial.print("#####################\r\n");
}

// can't actually shift a blinking cursor on serial terminal
// so this improvises by just printing the cursor's position in a separate line
void printSelected() {
  Serial.print("Current selection: ");
  if (row==5 && col==1) {
    Serial.print("SPACE\r\n");
  } else if (row==5 && col==4) {
    Serial.print("BACKSPACE\r\n");
  } else {
    Serial.print(grid[row][col]);
    Serial.print("\r\n");
  }
  Serial.print("#####################\r\n");
}

// show what text the user currently entered (updated in real time)
void printEntered() {
  Serial.print("You entered: ");
  Serial.print(enteredText);
  Serial.print("\r\n");
}

