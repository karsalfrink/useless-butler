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

  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);

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
      // Route allows for partial matches
      // Gesture outputs need to be named "/output/1"
      // Underscores (e.g. "/output_1") won't work!
      msgIn.route("/output", routeAddress);
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

void routeAddress(OSCMessage &msg, int addrOffset) {
  char addrArray[2]; // Character array to hold address after offset
  msg.getAddress(addrArray, addrOffset); // Get the address after offset and store
  String addr(addrArray); // Convert character array to string

  if (addr == "/1") setRgbLed(255, 0, 0); // Set RGB LED to red
  if (addr == "/2") setRgbLed(0, 255, 0); // Set RGB LED to green
  if (addr == "/3") setRgbLed(0, 0, 0);   // Turn RGB LED off
}

void setRgbLed(int r, int g, int b) {
  // For common anode RGB LEDs the input needs to be inverted
  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);
}
