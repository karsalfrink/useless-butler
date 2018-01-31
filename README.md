# Prototyping the Useless Butler: Machine Learning for IoT Designers

This example code is part of a workshop developed by [Péter Kun](http://peterkun.com) and [Kars Alfrink](http://leapfrog.nl) for teaching designers of internet of things products to make interactive supervised machine learning part of their prototyping workflow.

## Prerequisites

* Download and install [Wekinator](http://www.wekinator.org/downloads/)
* Download and install [Arduino desktop IDE](https://www.arduino.cc/en/Main/Software)
* Add [Atmel SAMD Core](https://www.arduino.cc/en/Guide/MKR1000#toc2) to Arduino IDE boards manager
* Install [Arduino MKR1000 drivers](https://www.arduino.cc/en/Guide/MKR1000#toc3) (if on Windows)
* Download and install the [Arduino OSC library](https://github.com/CNMAT/OSC)
* Install [Wifi101 library](https://www.arduino.cc/en/Reference/WiFi101) through Arduino IDE

## How-To

### Building the circuit

Assuming an Arduino MKR1000, build the circuit as included in the `/Fritzing` directory.

![Circuit](Fritzing/circuit.png?raw=true)

* One RGB LED on digital PWM pins 2, 3, 4
* Two photoresistors on analog pins A0 and A1

**Note:** The circuit diagram shows a common anode RGB LED. If using a common cathode RGB LED, connect the long pin to ground instead of 5V.

### Using the code

The `/Arduino` directory contains three examples, designed to be used with Wekinator's three output types:

1. Regression
2. Classification
3. Dynamic Time Warping (DTW)

For all examples, go through the steps below to get communication between the MKR1000 and Wekinator up and running.

1. Copy `config_sample.h` and rename it to `config.h`
2. Specify your wireless network's SSID and password in `config.h`
3. Get your computer's IP address from its network settings and specify it in the Arduino sketch under `outIp`
4. Upload the sketch to your MKR1000
5. Make note of the MKR1000's IP address in the Arduino IDE serial monitor (we will need this when setting up Wekinator)

Read on below for the specifics of each example.

### Regression
### Classification
### Dynamic Time Warping (DTW)

## Reports

* [Write-up of the workshop at ThingsCon Amsterdam 2017](http://leapfrog.nl/blog/archives/2018/01/02/prototyping-the-useless-butler-machine-learning-for-iot-designers/)
