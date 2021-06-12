![Version Tag](https://img.shields.io/badge/Version-2.0.0-blue.svg)

# Introduction
The **FourSBase** library is to control a four wheel holonomic drive for omni wheels. This class is derived from [BotBase](../BotBase/) class, so it's suggested that you go through it's documentation first.<br>

# Index
- [Introduction](#introduction)
- [Index](#index)
    - [Information](#information)
        - [Testing](#testing)
    - [Conventions](#conventions)
- [Users Guide](#users-guide)
    - [Downloading the Library](#downloading-the-library)
    - [Using the library with Arduino](#using-the-library-with-arduino)
    - [Using the library](#using-the-library)
    - [Examples](#examples)
        - [Serial_Test](#serial_test)
- [Developers Guide](#developers-guide)
    - [Library Details](#library-details)
        - [Files in the library](#files-in-the-library)
            - [FourSBase.h](#foursbaseh)
            - [FourSBase.cpp](#foursbasecpp)
            - [keywords.txt](#keywordstxt)
            - [README.md](#readmemd)
        - [Class description](#class-description)
            - [Protected members](#protected-members)
                - [Member functions](#member-functions)
            - [Public members](#public-members)
                - [Constructors](#constructors)
- [Debugger notifications](#debugger-notifications)
    - [Debug level](#debug-level)

## Information
### Testing
This library has been tested on the wheel configuration shown below
![FourSBase image](../.DATA/Images/FourSBase_Wheels.png)
The image above is taken from the top view.
## Conventions
Please refer to the image below for the conventions used in this library
![FourSBase conventions](../.DATA/Images/FourSBase_WheelConfigurations.png)
The image above is taken from the top view.
- The *wheels* are numbered in a counter clockwise sense from 1 to 4.
- The *axis of rotation* is shown in light blue axis lines.
- The *sense of rotation* that is considered positive is shown by golden arrows for each wheel. Note that the axis of rotation is decided using the right hand rule.
    - For instance, the first wheel (top right) rotates in such a way that it's top point moves towards left and the bottom point moves towards right as seen from top view. Only then the rotation on the wheel is considered positive, else it's negative.
- The *zero axis* (the axis considered at 0<sup>o</sup>) is shown in red. The +ve sense of rotational measurement is shown in light red.

# Users Guide

## Downloading the Library
It's suggested that you download the entire repository and then select the folders `FourSBase` and `BotBase` (in case you haven't already imported it). This library depends upon the **BotBase** library click [here](../BotBase/). Also, the Library makes use of the DebuggerSerial Library. For More Information on this, click [here](../DebuggerSerial).

## Using the library with Arduino
Move the folders into the *arduino libraries* folder on your PC. If you don't know where the libraries folder of your arduino installation is, you can check out the README file of this entire repository (click [here](../README.md)).

## Using the library
- Include the header file in the program.
- Create arrays for the PWM pins, DIR pins and reverse directions (if needed). Check the [BotBase documentation](./../BotBase/) for more.
- Create an object of the **FourSBase** class.
- Call the _AttachPins_ function (it's there in [BotBase documentation](./../BotBase/)). Pass it the PWM pins, DIR pins and reverseDIR values.
- Call the **Move** function to apply motion to the bot. (The description about this is in the [BotBase documentation](./../BotBase/))

## Examples
After moving the library to the correct location, you can check the following examples.
### Serial_Test

Before we get started with the example walkthrough, let's start with assumptions
- You are using motor drivers that require only two control inputs, PWM (PWM voltage) and DIR (Direction, 1 is +ve sense rotation and 0 is -ve sense rotation).
- Wheels are connected in the following manner to the microcontroller (Arduino Mega in our example).<br>

| Wheel number | PWM Pin | Direction pin | Direction Connection Reversed<sup>\*</sup> |
| :---------: | :-------: | :---------: | :-----------: |
| 1  | 7  | 53 | No |
| 2  |  6 | 51 | No  |
| 3  |  5 | 49 | No  |
| 4  |  4 | 47 | No  |

<sup>\*</sup> *Direction connection reversed*: When you give HIGH to DIR pin of that particular motor, it rotates in the -ve sense instead of the +ve sense. You can simply set up a boolean array as shown in code to handle this problem, no need of changing electrical connections.<br>
*You can have different connections as well, these are just for this example.*

- Simply, upload this code to Arduino Mega, make the connections as mentioned in the table and then you're ready to give serial commands to move the bot.

- To Set the bot parameters at some magnitude (a measure of speed) (say 200), at an angle (say 45<sup>o</sup>) and angular velocity (say 50), give it the following command:

```
P200A45W50
```
- To Move the bot at the set parameters for some time  (say 5 secondds), give it the following command:

```
M5
```
You can even give it in individual pieces as well (or even have spaces), `P60` sets the PWM to 60 and `A45` sets the angle of motion to 45<sup>o</sup>. Note the positive sense of rotational measurement from conventions section. For now, the control commands are as follows<br>

| Command | Arguments | Comments |
| ------- | --------- | -------- |
|P   | VAL_LEN  | Set the length of the motion vector (-ve values are allowed) |
|A   | VAL_ANG_DEG  | Sets the angle (in degrees) of the motion vector (-ve values are allowed) |
|W   | VAL_ANG_VEL  | Sets the angular velocity of the motion vector (-ve values are allowed) |
|M   | VAL_DELAY  | Sets the Bot in Motion for the Given Duration  |

File: [./examples/Serial_Test/Serial_Test.ino](./examples/Serial_Test/Serial_Test.ino)<br>
Upon startup, you must see this on the DebuggerSerial (your timestamps might differ).

```
[0] > Debugger enabled
[0] > DebuggerSerial attached
[1 INFO] $FourSBase$ Motor number 1 attached to (PWM, DIR): 7, 53 Reverse DIR: FALSE
[79 INFO] $FourSBase$ Motor number 2 attached to (PWM, DIR): 6, 51 Reverse DIR: FALSE
[169 INFO] $FourSBase$ Motor number 3 attached to (PWM, DIR): 5, 49 Reverse DIR: FALSE
[262 INFO] $FourSBase$ Motor number 4 attached to (PWM, DIR): 4, 47 Reverse DIR: FALSE
[353 DEBUG] $FourSBase$ All pins attached

```
After the Move function is called, you must see something like this (again, your timestamps might differ)

```
[48913 DEBUG] $FourSBase$ Calculated wheel vectors : [ 0 -200 0 200 ]
[48920 DEBUG] $FourSBase$ Vector[0] -> 0 = 0, HIGH
[48973 DEBUG] $FourSBase$ Vector[1] -> -200 = 200, LOW
[49032 DEBUG] $FourSBase$ Vector[2] -> 0 = 0, HIGH
[49086 DEBUG] $FourSBase$ Vector[3] -> 200 = 200, HIGH
[49144 DEBUG] $FourSBase$ Mode: Sign Magnitude
[49195 DEBUG] $FourSBase$ Motor 1 status = [7: 0, 53: 1]
[49254 DEBUG] $FourSBase$ Mode: Sign Magnitude
[49305 DEBUG] $FourSBase$ Motor 2 status = [6: 200, 51: 0]
[49367 DEBUG] $FourSBase$ Mode: Sign Magnitude
[49417 DEBUG] $FourSBase$ Motor 3 status = [5: 0, 49: 1]
[49477 DEBUG] $FourSBase$ Mode: Sign Magnitude
[49527 DEBUG] $FourSBase$ Motor 4 status = [4: 200, 47: 1]
[55588 DEBUG] $FourSBase$ Delay Over
```
Notice the values that are written to the motors. If you get something similar, then you're good to go.

Try passing different values to the Move function and observe the different results.

# Developers Guide
You're suggested to go through the **BotBase** documentation first, link [here](../BotBase/).<br>
## Library Details
### Files in the library
Let's first explore about all the files in this library

#### FourSBase.h
This is the header file and contains the class blueprint (prototype). We will explore the details about the class soon.

#### FourSBase.cpp
This is the file that contains the main code for the class. In the header file, only the function prototypes are mentioned, the code for the functions (definition) are present in this file.

#### keywords.txt
This file contains the keywords that we want the Arduino IDE to identify. This provides syntax highlighting features for the library for convenience of the programmer.

#### README.md
The description file containing details about the library. The file that you are looking at right now

### Class description
This library assumes the following :-
- You have a motor driver that requires only PWM and DIR (PWM and direction) signals to control the speed and direction of a motor.

This class is derived directly from the BotBase class (public inheritance).

#### Protected members

##### Member functions
- **<font color="#CD00FF">void</font> <font color="#5052FF">Move_PWM_Angle</font>(<font color="#FF00FF">int</font> PWM, <font color="#FF00FF">float</font> angle_radians)**: This function takes the velocity or _PWM_ (basically the **V** value) and the header direction angle (in radians), which is basically **θ** value. It also takes the angular velocity needed about the center of the bot (**w** or **ω** value). It calculates the velocity vector, which is all the **V<sub>i</sub>** values. It then calls the *VectorTo_PWM_DIR* function from the `BotBase` class. <br>

**NOTE**: The user only needs to call the `Move` function of the **BotBase** class.

#### Public members
##### Constructors
- **<font color="#5052FF">FourSBase</font>()**: The empty constructor for the class.

# Debugger notifications
## Debug level
1. **Move_PWM_Angle** functions<br>
    Prints the wheel vectors.
    ```
    [%TIMESTAMP% DEBUG] $%name%$ Calculated wheel vectors : [ %val1% %val2% %val3% %val4% ]
    ```
    For example:
    - After 18000 milliseconds, `FourSBase` (name) develops vectors `[ 100 0 -100 0 ]`.
        ```
        [18000 DEBUG] $FourSBase$ Calculated wheel vectors : [ 100 0 -100 0 ]
        ```


[![Image](https://img.shields.io/badge/Developer-TheProjectsGuy-blue.svg)](https://github.com/TheProjectsGuy) <t>
[![Image](https://img.shields.io/badge/Developer-shashank3199-red.svg)](https://github.com/shashank3199) <t> [![Image](https://img.shields.io/badge/issue-%2316-green.svg)](https://github.com/RoboManipal/Libraries/issues/16)
