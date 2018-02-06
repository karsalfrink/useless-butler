#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#include "config.h" // Copy config_sample.h and rename it

char ssid[] = MYSSID;                 // Your network SSID (name)
char pass[] = MYPASS;                 // Your network password

int status = WL_IDLE_STATUS;          // The WiFi radio's status

unsigned int localPort = 12000;       // Local port to listen on

// Values below should be those of the machine running Wekinator
IPAddress outIp(999, 999, 999, 999);  // Remote IP to send to
unsigned int outPort = 6448;          // Remote port to send to

#include "wifiHelpers.h" // Helper functions for wifi

WiFiUDP Udp; // For sending and receiving UDP messages

// Set up sensors
const int sensorPins[] = {A0, A1};

// Set up RGB LED
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

// Set up onboard LED
const int onboardLed = 6;

const bool anode = true; // Set this to false if using a common cathode RGB LED

void setup() {
  // Set up wifi
  setupWifi();

  // Print some details
  printCurrentNet();
  printWiFiData();

  // Start listening for stuff over UDP
  Udp.begin(localPort);

  // Print some more details
  printPorts();

  // Set up the RGB LED
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Turn on onboard LED to indicate setup has completed
  pinMode(onboardLed, OUTPUT);
  digitalWrite(onboardLed, HIGH);
}

void loop() {
  // Receive message
  OSCMessage msgIn;

  int size;
  if ( (size = Udp.parsePacket()) > 0) {
    while (size--)
      msgIn.fill(Udp.read());

    if (!msgIn.hasError())
      msgIn.dispatch("/wek/outputs", dispatchAddress);
  }

  // Send message
  OSCMessage msgOut("/wek/inputs");
  for (int i = 0; i < 2; i++) {
    float input = (float)analogRead(sensorPins[i]);
    msgOut.add(input);
  }

  Udp.beginPacket(outIp, outPort);
  msgOut.send(Udp); // Send the bytes to the SLIP stream
  Udp.endPacket();  // Mark the end of the OSC Packet
  msgOut.empty();   // Free space occupied by message

  delay(20);
}

void dispatchAddress(OSCMessage &msg) {
  // Even though we are classifying, wekinator sends outputs as floats
  if (msg.isFloat(0)) {
    // Convert output to int and store it
    int val = (int)msg.getFloat(0);
    // Handle each case, we are assuming 4 in total
    switch (val) {
      case 1:
        setRgbLed(255, 255, 0);   // Set rgb led to yellow
        break;
      case 2:
        setRgbLed(0, 255, 255);   // Set rgb led to cyan
        break;
      case 3:
        setRgbLed(255, 0, 255);   // Set rgb led to magenta
        break;
      case 4:
        setRgbLed(255, 255, 255); // Set rgb to white
        break;
      default:
        setRgbLed(0, 0, 0);       // Set rgb led off
    }
  }
}

void setRgbLed(int r, int g, int b) {
  // For common anode RGB LEDs the input needs to be inverted
  if (anode) {
    analogWrite(redPin, 255 - r);
    analogWrite(greenPin, 255 - g);
    analogWrite(bluePin, 255 - b);
  } else {
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);    
  }
}
