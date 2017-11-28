void setupWifi() {
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
  while (status != WL_CONNECTED) {
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
}

void printWiFiData() {
  // Print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(ip);

  // Print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
  Serial.println();
}

void printCurrentNet() {
  // Print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // Print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.println(rssi);

  // Print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type: ");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printPorts() {
  Serial.print("Listening on port: ");
  Serial.println(localPort);
  Serial.println();

  // Print some stuff about where we're sending to
  Serial.print("Sending to IP address: ");
  Serial.print(outIp);
  Serial.print(" on port: ");  
  Serial.println(outPort);
  Serial.println();
}
