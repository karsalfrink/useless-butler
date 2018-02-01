# Prototyping the Useless Butler: Machine Learning for IoT Designers

Code and documentation for a workshop developed by [Péter Kun](http://peterkun.com) and [Kars Alfrink](http://leapfrog.nl) to teach designers of internet-of-things products to make interactive supervised machine learning part of their prototyping workflow.

## Prerequisites

* Download and install [Wekinator](http://www.wekinator.org/downloads/)
* Download and install [Arduino desktop IDE](https://www.arduino.cc/en/Main/Software)
* Add [Atmel SAMD Core](https://www.arduino.cc/en/Guide/MKR1000#toc2) to Arduino IDE boards manager
* Install [Arduino MKR1000 drivers](https://www.arduino.cc/en/Guide/MKR1000#toc3) (if on Windows)
* Download and install the [Arduino OSC library](https://github.com/CNMAT/OSC)
* Install [Wifi101 library](https://www.arduino.cc/en/Reference/WiFi101) through Arduino IDE

## How-To

### Building the circuit

Using an Arduino MKR1000, build the circuit as included in the `/Fritzing` directory.

* One RGB LED on digital PWM pins 2, 3, 4
* Two photoresistors on analog pins A0 and A1

![Circuit](Fritzing/circuit.png?raw=true)

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

#### Setup

Create a new project in Wekinator with the settings below.

![Regression settings](Screenshots/wekinator-regression-settings.png?raw=true)

* Two inputs (one for each photoresistor)
* Three outputs (one each for red, green and blue)
* All continuous (default)
* Don't forget to set your MKR1000's IP under 'host'
* Leave ports as they are

Hit 'next' to start training.

#### Training and running

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

> These are discrete categories, such as “Position 1”, “Position 2,” “Position 3.” You’ll need to tell Wekinator how many categories to use. Wekinator will send outputs as numbers, such as “1,” “2,” “3” for categories 1, 2, and 3. Wekinator will attempt to categorize every new input you send it. ([source](http://www.wekinator.org/detailed-instructions/#The_three_output_types))

Think of this as a smart switch.

#### Setup

Create a new project in Wekinator with the settings below.

![Classification settings](Screenshots/wekinator-classification-settings.png?raw=true)

* Two inputs (one for each photoresistor)
* One output (for the RGB LED)
* All classifiers
* Four classes (one each for a specific RGB LED state controlled by the Arduino sketch—cyan, magenta, yellow and white)
* Don't forget to set your MKR1000's IP under 'host'
* Leave ports as they are

Hit ‘next’ to start training.

#### Training and running

Training is identical to regression as described above. The only difference is that the outputs are one of the four classes for the single output. These can be selected with the drop down box. Recording one example for each class should suffice.

### 3. Dynamic Time Warping (DTW)

> Use this output type when you want Wekinator to recognize patterns over time. For instance, you might want to play one note every time you draw a circle in the air with your hand, and another note every time you draw a square. If you’re not drawing either one, or if you’re in the middle of drawing, you don’t want anything to happen. That is, you want Wekinator to look for a particular pattern (or multiple patterns) of how the inputs are changing over time, and tell you when a pattern is spotted and which one it was. ([source](http://www.wekinator.org/detailed-instructions/#The_three_output_types))

Think of this as a smart button.

#### Setup

Create a new project in Wekinator with the settings below.

![DTW settings](Screenshots/wekinator-dtw-settings.png?raw=true)

* Two inputs (one for each photoresistor)
* One output (for the RGB LED)
* All DTW
* Three gesture types (one each for a specific RGB LED state controlled by the Arduino sketch—red, green or off)
* Don't forget to set your MKR1000's IP under 'host'
* Leave ports as they are

Hit ‘next’ to start training.

### Editing OSC messages

**Note:** Before you can start training you need to rename your output OSC messages to "output/1", "output/2" and "output/3". (The reason for this is related to how the Arduino OSC library processes messages.) To do this, hit the 'edit' button at the bottom of the main Dynamic Time Warping screen, then hit the 'edit' button for the output group name. Adjust the gesture OSC messages, then hit 'OK' to close the 'names and OSC messages' window. Hit 'apply changes' to close the 'editing output' window. We are now ready to train.

![Editing DTW OSC messages](Screenshots/wekinator-dtw-edit-osc-messages.png?raw=true)

#### Training and running

## Reports

* [Write-up of the first attempt at TU Delft IDE Academy](https://leapfrog.nl/blog/archives/2017/03/10/machine-learning-for-designers-workshop/)
* [Write-up of the workshop at ThingsCon Amsterdam 2017](https://leapfrog.nl/blog/archives/2018/01/02/prototyping-the-useless-butler-machine-learning-for-iot-designers/)
