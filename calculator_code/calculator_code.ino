#include <LiquidCrystal.h>

// Define pin numbers for rows and columns of the keypad matrix
#define r1 0
#define r2 1
#define r3 2
#define r4 3
#define c1 4
#define c2 5
#define c3 6
#define c4 7

// Define the number of rows and columns in the keypad matrix
const byte ROWS = 4;
const byte COLS = 4;

// Define the keys in the keypad matrix
char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};

// Initialize the LiquidCrystal library with the appropriate pins
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// Arrays to store operands and the current operation
char operand1[16] = "";
char operand2[16] = "";
char operation = '\0';

// Flag to track whether user input is in progress
bool inputInProgress = false;

// Setup function runs once at the beginning
void setup() {
  // Set the row pins as OUTPUT
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);

  // Set initial state of row pins to HIGH
  digitalWrite(r1, HIGH);
  digitalWrite(r2, HIGH);
  digitalWrite(r3, HIGH);
  digitalWrite(r4, HIGH);

  // Set the column pins as INPUT_PULLUP
  pinMode(c1, INPUT_PULLUP);
  pinMode(c2, INPUT_PULLUP);
  pinMode(c3, INPUT_PULLUP);
  pinMode(c4, INPUT_PULLUP);

  // Initialize the LCD with 16 columns and 2 rows
  lcd.begin(16, 2);
  // Display a welcome message
  lcd.print("  Nabil Sherif");
}

// Function to check if a key is pressed
char getKeyPressed(void) {
  int Read;
  int flag = 0;

  for (int i = 0; i < ROWS; i++) {
    digitalWrite(i, LOW);

    for (int j = 0; j < COLS; j++) {
      Read = digitalRead(j + 4);

      // Wait until the key is released
      while (Read == LOW) {
        Read = digitalRead(j + 4);
        flag = 1;
      }

      if (flag == 1) {
        // Release the row pin
        digitalWrite(i, HIGH);
        return keys[i][j]; // Return the pressed key
      }
    }
    // Release the row pin
    digitalWrite(i, HIGH);
  }
  return 0; // Return 0 if no key is pressed
}

// Loop function runs repeatedly
void loop() {
  char key = getKeyPressed(); // Get the current key pressed

  if (key != '\0') {
    handleInput(key); // Handle the input based on the key
  }
}

// Function to handle user input based on the key pressed
void handleInput(char key) {
  if (isdigit(key)) {
    handleDigitInput(key); // If the key is a digit, handle digit input
  } else if (key == '=') {
    handleEqualInput(); // If the key is '=', handle equal input
  } else if (key == 'C') {
    handleClearInput(); // If the key is 'C', handle clear input
  } else {
    handleOperationInput(key); // Otherwise, handle operation input
  }
}

// Function to handle digit input
void handleDigitInput(char key) {
  if (!inputInProgress) {
    lcd.clear(); // Clear the LCD if input is not in progress
    operand1[0] = key;
    operand1[1] = '\0';
    lcd.setCursor(0, 0);
    lcd.print(operand1); // Display the current operand1
    inputInProgress = true; // Set input in progress flag to true
  } else {
    strcat(operand1, &key); // Concatenate the digit to the current operand1
    lcd.setCursor(0, 0);
    lcd.print(operand1); // Display the updated operand1
  }
}

// Function to handle equal input
void handleEqualInput() {
  if (inputInProgress && *operand1 != '\0' && operation != '\0') {
    float num1 = atof(operand1); // Convert operand1 to a floating-point number
    float num2 = atof(operand2); // Convert operand2 to a floating-point number

    float result = performOperation(num1, num2, operation); // Perform the operation

    displayResult(result); // Display the result on the LCD
  }
}

// Function to handle clear input
void handleClearInput() {
  lcd.clear(); // Clear the LCD
  reset(); // Reset all variables and display the welcome message
}

// Function to handle operation input
void handleOperationInput(char key) {
  operation = key; // Set the current operation
  inputInProgress = false; // Reset input in progress flag
  strcpy(operand2, operand1); // Copy operand1 to operand2
  operand1[0] = '\0'; // Reset operand1

  lcd.setCursor(0, 1);
  lcd.print(key); // Display the current operation on the second line of the LCD
}

// Function to perform the specified operation
float performOperation(float num1, float num2, char op) {
  switch (op) {
    case '+':
      return num2 + num1; // Addition
    case '-':
      return num2 - num1; // Subtraction
    case '*':
      return num2 * num1; // Multiplication
    case '/':
      return num2 / num1; // Division
    default:
      return 0; // Default result for unknown operation
  }
}

// Function to display the result on the LCD
void displayResult(float result) {
  lcd.clear(); // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("result:");

  lcd.setCursor(0, 1);
  lcd.print(result); // Display the result on the second line of the LCD
}

// Function to reset all variables and display the welcome message
void reset() {
  operand1[0] = '\0'; // Reset operand1
  operand2[0] = '\0'; // Reset operand2
  operation = '\0'; // Reset operation
  inputInProgress = false; // Reset input in progress flag

  lcd.print("  Nabil Sherif"); // Display the welcome message
}
