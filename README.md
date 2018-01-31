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

### 1. Regression

> Real-valued ("continuous") numeric outputs can take on any number value (possibly limited to a certain range). For example, you might want to control "audio gain" with a real-valued output limited between 0 and 1. This is the default output type in Wekinator. ([source](http://www.wekinator.org/detailed-instructions/#The_three_output_types))

Think of this as a smart slider.

#### Wekinator setup

Create a new project in Wekinator with the settings below.

![Regression settings](Screenshots/wekinator-regression-settings.png?raw=true)

* Two inputs (one for each photoresistor)
* Three outputs (one each for red, green and blue)
* All continuous (default)
* Don't forget to set your MKR1000's IP under 'host'
* Leave ports as they are

Hit 'next' to start training

#### Wekinator training and running

(Instructions below adapted from the [walkthrough](http://www.wekinator.org/walkthrough/) on the Wekinator website.)

1. Use the sliders, text boxes, and/or the 'Randomize' button to send new control values to your output (the MKR1000's RGB LED). You should see changes in your output as you do this. (If you don't see this change, make sure you don't have any other outputs open; if you did, you may have to restart your output again.)
2. Once you have a color you like, start demonstrating which input values you want to use to achieve this color. That is to say, change the intensity of light falling on one or both of the photoresistors. For example, you can cover them with your hands or shine on them with your phone's flashlight.
3. Hit 'Start Recording,' wait about 1/2 second, then hit 'Stop Recording.' You should see that the number of examples recorded for each output is now something greater than 0. (If it's still 0, make sure your input is still running, and that you don't have multiple inputs or Wekinator projects open at once.)
4. Repeat steps 1-3 at least one more time: Find a color you like, adjust your inputs to tell Wekinator how you want to get to that color, and then briefly record some examples.
5. Hit 'Train' to build a model from all the training examples you just recorded.
6. Hit 'Run.' Wekinator will now use the model you've just built to compute new output values for every new input value it sees. As you change your inputs (change the photoresistor lighting conditions), you should see the outputs change.
7. You've got it! If you like it, you can save your project and re-load it later to use with the same input and output. Or, you can keep refining your models:

* You can keep adding training examples by repeating steps 1-3 above, then re-training. This will add those examples to all the examples you've recorded previously.
* If you don't like what's happened when you've added examples, you can hit 'Delete last recording' to remove them from your training set. You'll have to re-train in order for this to have an effect.
* If you want to start over, you can delete all examples for a single output by clicking the 'X' button in that output's row. Or you can delete absolutely everything by clicking the 'X' on top of your project window.

### 2. Classification
### 3. Dynamic Time Warping (DTW)

## Reports

* [Write-up of the workshop at ThingsCon Amsterdam 2017](http://leapfrog.nl/blog/archives/2018/01/02/prototyping-the-useless-butler-machine-learning-for-iot-designers/)
