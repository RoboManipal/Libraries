![Version tag](https://img.shields.io/badge/version-1.2.5-green.svg)
# Introduction
This library helps interface a PS2 Controller with Arduino and calculate normalised paramters such as the angle
and speed of the bot motion.

# User's Guide
## Conventions
Please refer to the image below for the conventions followed in the library.
![Joystick conventions](../.DATA/Images/JoystickConventions.png)
The image is taken from top view.
- The same convention is followed for both the joystick values, i,e right and left.
- The *zero axis* is shown in red. The cartesian coordinate frame is as marked in the figure.
- The maximum values of the speed of motion are recorded along the circumference of the circle.

## Downloading the Library
It's suggested that you download the entire repository and then select the folder`CytronPS2` (in case you haven't already imported it).

## Using the library with Arduino
Move the folders into the *arduino libraries* folder on your PC.

## Example
Check the link for the [PS2 Controller_Test](https://github.com/RoboManipal-9-0/Libraries/edit/akshatha-PS2-lib/CytronPS2/PS2controller_test)

- Create a SoftwareSerial object if using software serial
- Create an object of CytronPS2 class.
- Pass the software serial object or the Hardware serial as a parameter to AttachSerial
eg:
```
SoftwareSerial myserial(A11,A12);

myPS2.AttachSerial(&myserial);
```
- Pass true as an argument if using right joystick, else if using left joystick pass false
- Simply, make the required connections of the PS2Shield and the Arduino and upload this code to Arduino Mega/Uno etc. Play around with the joystick and the position , required angle and speed are displayed on your serial monitor.

Note:
- Set the baudrate of the PS2 Serial that matches with the jumper setting on the PS2 Shield.

# Developers guide

## Library Details
### Files in the Library
The files in the library are :

#### CytronPS2.h
This is the header file and contains the class blueprint (prototype). We will explore the details about the class soon.

#### CytronPS2.cpp
This is the file that contains the main code for the class. In the header file, only the function prototypes are mentioned, the code for the functions (definition) are present in this file.

#### README.md
The description file containing details about the library. The file that you at right now.

###  Class Description
This library assumes that you have a Cytron PS2 Shield and have established the connection with the PS2 Controller.

#### Private Members
#### Variables

- **<font color="#CD00FF">Stream</font> ps2Serial**: The RX pin for initialising Software Serial.

- **<font color="#CD00FF">float</font> left_x** : Analog values read from the joystick. ( Value of the X coordinate from 0-255, that is later mapped from -127 to 128)

- **<font color="#CD00FF">float</font> left_y** : Analog values read from the joystick. ( Value of the X coordinate from 0-255, that is later mapped from -127 to 128)

- **<font color="#CD00FF">CytronPS2</font> ps2** : Creates an object of the Cytron_PS2Shield class

- **<font color="#CD00FF">DebuggerSerial</font> debugger** : Creates an object of the DebuggerSerial class

#### Public Members

##### Variables
- **<font color="#CD00FF">int</font> speeds** : It is the speed at which the bot is required to move. This has been kept intentionally public to enable arduino codes to access and modify it

- **<font color="#CD00FF">float</font> angle** : It is the angle at which the bot is required to move. This has been kept intentionally public to enable arduino codes to access and modify it

##### Member Functions
- **<font color="#CD00FF">void</font> <font color="#5052FF">AttachDebugSerial</font>()** : To assign the default Serial as the Debugger Serial. Refer to [DebuggerSerial library](https://github.com/RoboManipal-9-0/Libraries/tree/master/DebuggerSerial) for more details.

- **<font color="#CD00FF">void</font> <font color="#5052FF">ReadPS2Values</font>()** :
Calculates the raw analog values returned by the joystick. ( Values range from 0-255 )

- Adjust the range of coordinates from 0-255 to -127-128. This is done in order to have an origin ( centre of joystick ) correspond to (0,0). It also maps the circular space of a joystick such that every point on its circumference corresponds to max. speed.
The scaling_factor here is ratio of the max speed of motion desired to 127.
eg. if we require a maximum speed of 230, scaling_factor would be 230/127.

```
this->speeds=pow(pow(this->left_x,2)+pow(this->left_y,2),0.5);
```
The values have been mapped as shown below:
![here](../.DATA/Images/map.png)

The values obtained form a rectangular coordinate system with X and Y values ranging from -128 to 127. We first map it from -127 to 127 and then use simple stretch to map the values to a circle.
For the derivation , have a look at the research paper below. ( We have used the simple stretch method)
[map](../.DATA/ResearchPaper/Mapping.pdf)

- The angle and speed of the desired bot motion is calculated from the position of the joystick.


### Additional References
EduBot Cytron Tutorial with PS2 Shield : - [Cytron-PS2-Shield Tutorial](https://tutorial.cytron.io/2015/07/23/using-cytron-ps2-shield-with-arduino-edubot-2/)
Original Cytron PS2 Shield library : [Cytron-PS2-Shield](https://github.com/CytronTechnologies/Cytron_PS2Shield)

[![Developed using Atom](https://img.shields.io/badge/developed%20using-Atom-lightgrey.svg)](https://atom.io/)
