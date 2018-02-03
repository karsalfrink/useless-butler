// Set up sensors
const int sensorPins[] = {A0, A1};

// Set up RGB LED
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

// Variable to track RGB LED state
int rgbLedState = 0;

void setup() {
  // Set up the RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
  // Read out each photoresistor and print its value to the serial monitor
  for (int i = 0; i < 2; i++) {
    float input = (float)analogRead(sensorPins[i]);
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(input);
  }

  // Check the RGB LED state and set its color accordingly
  switch (rgbLedState) {
    case 0: // Red
      setRgbLed(255,0,0);
      break;
    case 1: // Green
      setRgbLed(0,255,0);
      break;
    case 2: // Blue
      setRgbLed(0,0,255);
      break;
    default: // White
      setRgbLed(0,0,0);
      break;    
  }

  // If we are at RGB LED state three, go back to state one
  if (rgbLedState == 2) {
    rgbLedState = 0;
  } else {
    rgbLedState++;
  }

  // Wait a second…
  delay(1000);
}

void setRgbLed(int r, int g, int b) {
  // For common anode RGB LEDs the input needs to be inverted
  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);
}
