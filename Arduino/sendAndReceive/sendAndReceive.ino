#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#include "config.h" // copy config_sample.h and rename it
#include "wifiHelpers.h" // helper functions for wifi

char ssid[] = MYSSID;             // your network SSID (name)
char pass[] = MYPASS;             // your network password
int status = WL_IDLE_STATUS;      // the WiFi radio's status

unsigned int localPort = 12000;   // local port to listen on

IPAddress outIp(172, 20, 10, 2);  // remote IP to send to
unsigned int outPort = 6448;      // remote port to send to

WiFiUDP Udp;

// Set up sensors
const int sensorPin = A0; // Sensor 1

// Set up RGB LED
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

// Set up onboard LED
const int led = 6;                    // the PWM pin the LED is attached to
const int initialBrightness = 0;      // how bright the LED is (number between 0 and 255)

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  // What this does: http://arduino.stackexchange.com/a/4557/31607
  // Remove this if you want your sketch to run without a serial connection
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // Don't continue:
    while (true);
  }

  // Attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // Wait 10 seconds for connection:
    delay(10000);
  }

  // You're connected now, so print out the data:
  Serial.println();
  Serial.println("You're connected to the network!");
  Serial.println();
  printCurrentNet();
  printWiFiData();

  // Start listening for stuff over UDP
  Udp.begin(localPort);
  Serial.print("Listening on port: ");
  Serial.println(localPort);
  Serial.println();

  // Print some stuff about where we're sending to
  Serial.print("Sending to IP address: ");
  Serial.print(outIp);
  Serial.print(" on port: ");  
  Serial.println(outPort);
  Serial.println();

  // Set up the LED
  pinMode(led, OUTPUT);
  analogWrite(led, initialBrightness);
}

void loop() {
//  Serial.println(analogRead(sensorPin));

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
  msgOut.add((float)analogRead(sensorPin));
  
  Udp.beginPacket(outIp, outPort);
    msgOut.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msgOut.empty(); // free space occupied by message

  delay(20);
}

void dispatchAddress(OSCMessage &msg) {
  // Returns true if the data in the first position is a float
  if (msg.isFloat(0)) {
    float val = msg.getFloat(0);
    Serial.print("Message is float: ");
    Serial.println(val);
    Serial.println();

    // Multiply float so we can map it to an int
    // http://electronics.stackexchange.com/a/83462
    float m = 3200.0f;
    val = val * m;    

    // Map value to range suitable for analogWrite()
    // Call function to set LED
    int mappedVal = map(val, 0, 1 * m, 0, 255);
    setLed(mappedVal);
  }
}

void setLed(int b) {
  // Set globally defined LED to specified brightness
  analogWrite(led, b);
  Serial.print("Set LED brightness to: ");
  Serial.println(b);
  Serial.println();
}
