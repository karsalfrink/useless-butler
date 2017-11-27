#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#include "config.h" // copy config_sample.h and rename it

char ssid[] = MYSSID;               // your network SSID (name)
char pass[] = MYPASS;               // your network password

int status = WL_IDLE_STATUS;        // the WiFi radio's status

unsigned int localPort = 12000;     // local port to listen on

IPAddress outIp(192, 168, 1, 120);  // remote IP to send to
unsigned int outPort = 6448;        // remote port to send to

#include "wifiHelpers.h" // helper functions for wifi

WiFiUDP Udp; // For sending and receiving UDP messages

// Set up sensors
const int sensorPins[] = {A0, A1};

// Set up RGB LED
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

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
}

void loop() {
  // Receive message
  OSCMessage msgIn;
  
  int size;  
  if( (size = Udp.parsePacket())>0) {
    while(size--)
      msgIn.fill(Udp.read());
    
    if(!msgIn.hasError())
      msgIn.dispatch("/wek/outputs", dispatchAddress);
  }

  // Send message
  OSCMessage msgOut("/wek/inputs");
  for (int i = 0; i < 2; i++) {
    float input = (float)analogRead(sensorPins[i]);
    msgOut.add(input);
  }
  
  Udp.beginPacket(outIp, outPort);
    msgOut.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOut.empty(); // free space occupied by message

  delay(20);
}

void dispatchAddress(OSCMessage &msg) {
  int vals[] = {};
  for (int i = 0; i < 3; i++) {
    // Returns true if the data in the indicated position is a float
    if (msg.isFloat(i)) {
      float val = msg.getFloat(i);

      // Multiply float so we can map it to an int
      // http://electronics.stackexchange.com/a/83462
      float m = 3200.0f;
      val = val * m;    
  
      // Map value to range suitable for analogWrite()
      int mappedVal = map(val, 0, 1 * m, 0, 255);
      vals[i] = mappedVal;
    }
  }
  // Call function to set RGB LED
  setRgbLed(vals[0], vals[1], vals[2]);
}

void setRgbLed(int r, int g, int b) {
  // For common anode RGB LEDs the input needs to be inverted
  analogWrite(redPin, 255 - r);
  analogWrite(greenPin, 255 - g);
  analogWrite(bluePin, 255 - b);
}
