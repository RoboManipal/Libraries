![Version 2.0.0 dev](https://img.shields.io/badge/version-2.0.0-orange.svg)
![Dev mode](https://img.shields.io/badge/mode-developer-ff69ff.svg)

# Introduction
This library is for defining any kind of wheeled base for a ground terrain robot.

~~**Note**: This library is still in developer beta, ask the developer before using.~~ <br>
Beta testing done :tada:

# Index
- [Introduction](#introduction)
- [Index](#index)
- [Users Guide](#users-guide)
    - [Downloading the library](#downloading-the-library)
    - [Using the library with Arduino](#using-the-library-with-arduino)
    - [Using the library](#using-the-library)
    - [Motor driver modes](#motor-driver-modes)
        - [Configuring the Mode](#configuring-the-mode)
        - [Sign Magnitude mode](#sign-magnitude-mode)
        - [Lock Anti-Phase Drive mode](#lock-anti-phase-drive-mode)
- [Developers Guide](#developers-guide)
    - [Library Details](#library-details)
        - [Files in the library](#files-in-the-library)
            - [BotBase.h](#botbaseh)
            - [BotBase.cpp](#botbasecpp)
            - [keywords.txt](#keywordstxt)
            - [README.md](#readmemd)
        - [Constants defined](#constants-defined)
        - [Class description](#class-description)
            - [Protected members](#protected-members)
                - [Variables](#variables)
                - [Member functions](#member-functions)
            - [Public Members](#public-members)
                - [Variables](#variables)
                - [Constructors](#constructors)
                - [Functions](#functions)
    - [Making you own custom BotBase](#making-you-own-custom-botbase)
        - [Guide](#guide)
            - [Examples](#examples)

# Users Guide

## Downloading the library
It is suggested that you download the entire repository and then select this folder, so that you can enjoy the benefits of VCS like git. It makes it simpler to update the contents whenever patch fixes are done. You can simply open a terminal (or gitbash on windows), go to the folder where you want to save this repository and type the following command.
```bash
git clone https://github.com/RoboManipal/Libraries.git -b dev
```
_You might want to omit the `-b <branch>` tag if you're downloading from the master version_.

**<font color="#AA0000">Not recommended</font>** : You can download just this folder by clicking [here](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/RoboManipal/Libraries/tree/dev/BotBase).

## Using the library with Arduino
Move this folder into the arduino libraries folder on your PC. If you don't know where the libraries folder of your arduino is, you can check out the README file of this entire repository for this, click [here](../README.md).<br>

## Using the library
You simply have to do the following to use this library
1. Create an object using one of the derived classes (you've already done this if you've been redirected here from a documentation page).
2. Call the function `AttachPins`. This function is used to initialize the connection terminals (pins) to the motor driver taking commands. You must pass it the following arguments<br>
> **\*PWM_pins**: The array pointer (name of the array) consisting the PWM pin numbers of the motor drivers.<br>
> **\*DIR_PINs**: The array pointer (name of the array) consisting the DIR pin numbers of the motor drivers.<br>
> (OPTIONAL) **\*reverseDIRs**: This is a boolean array of the same length as the number of wheels your bot has. Each element is defaulted to false. If you manually create this array and feed a true at some _index_ (numbering starts from 0 here), then it implies that the motors at those indices are connected in the reverse fashion (forward will actually mean that the particular motor moves in the -ve sense).<br>
3. Configure additional parameters, these are optional
    - Use the `SetScalingFactorsTo` function to configure scaling factors for the PWMs. Pass it the following
    > **\*scalingFactors**: This is an array of scaling factors (floating point values). It must include a scaling factor for every motor indexed in the right order.<br>
    - Use the `ConfigureModes` function to configure the modes of the individual motor drivers. Pass it the following
    > **\*modes**: An array consisting of the individual motor driver modes. One mode per motor driver in the right order. It's basically a list of mode identifiers. Currently, there's only `MODE_SM` and `MODE_LAP`. Check _motor driver modes_ section for more info.<br>
    - In case you've configured even a single motor to MODE_LAP, then set the default LAP PWM using the `setLAP_PWMto` function. Pass it the default PWM.
4. To move the base, call the `Move` function. This function handles the movement of the bot. You must pass it the following arguments<br>
> **PWM**: Velocity vector length.<br>
> **angle_degrees**: Angle the velocity vector makes with the reference (in degrees).<br>
> (OPTIONAL) **w** _value_: angular velocity with respect to the center.<br>

You can imagine the velocity vector as a point vector in the [polar coordinate system](https://en.wikipedia.org/wiki/Polar_coordinate_system).


## Motor driver modes

### Configuring the Mode
You can directly do it through a constructor of the `BotBase` class or call the `ConfigureModes` function. 

### Sign Magnitude mode
The _Sign Magnitude Drive_ is basically when we give the _PWM output to the PWM input pin of the motor driver_ and the _DIR output to the DIR input pin of the motor driver_. It's a direct connection and works as it sounds. A pin controls the direction of rotation and another pin controls the PWM of motor. You can know more from [here](http://www.modularcircuits.com/blog/articles/h-bridge-secrets/sign-magnitude-drive/).

The identifier for this mode is `MODE_SM`. This is the default mode, unless you override it by calling the `ConfigureModes` function.

### Lock Anti-Phase Drive mode
The _Lock Anti-Phase Drive_ is basically when we give the _PWM output to the DIR pin of the motor driver_ and the _PWM input of the motor driver gets a constant voltage_ (which in this library is given through the DIR output pin). Here's how it works:
- Say that we give 50% (duty cycle) as the PWM output into the DIR terminal of the motor driver. This means that 50 percent of the time the motor gets current in the forward direction and 50 percent of the time it gets current in the reverse direction, this gives the motor extraordinary braking capabilities.
- If we give more than 50% duty cycle, say 65%: Then the motor gets current in the forward direction 65 percent of the time and gets current in the reverse direction 35 percent of the time (100 - 65 = 35). This means that the forward current lasts longer and will dominate in the final effect. So we add something to the duty cycle if we want the motor to move forward (rotate in positive sense).
- If we give less than 50% duty cycle, say 25%: Then the motor gets current in the forward direction 25 percent of the time and gets current in the reverse direction 75 percent of the time. This means that the reverse direction gets the priority. So we subtract something from the duty cycle if we want the motor to move backward (rotate in negative sense).
- You could check out this [video on YouTube](https://www.youtube.com/watch?v=zu6k8kHkd9M), this [blog article](http://www.modularcircuits.com/blog/articles/h-bridge-secrets/lock-anti-phase-drive/) or run a simple [google search](https://www.google.co.in/search?q=locked+antiphase+motor+control&oq=locked+antiphase+motor+control&).

Therefore if we already have a PWM value (say _P_) and a DIR (_HIGH_ or _LOW_), we simply get the new MaxMode PWM (say _P<sub>n</sub>_) by simply doing the following
- If DIR is _HIGH_ (forward or positive sense): **P<sub>n</sub>** = 127 + **P** / 2.
- If DIR is _LOW_ (backward or negative sense): **P<sub>n</sub>** = 127 - **P** / 2.

You can configure the PWM value the PWM pin of the motor driver get by using the `setLAP_PWMto` function. The identifier used to describe this mode is `MODE_LAP`.

# Developers Guide
Here is the developers guide to the library. <br>
This markdown is best viewed in [Atom](https://atom.io/) or [VSCode](https://code.visualstudio.com/) editor.

## Library Details

### Files in the library
Let's first explore about all the files in this library

#### BotBase.h
This is the header file and contains the class blueprint (prototype). We will explore the details about the class soon.

#### BotBase.cpp
This is the file that contains the main code for the class. In the header file, only the function prototypes are mentioned, the code for the functions (definitions) are present in this file.

#### keywords.txt
This file contains the keywords that we want the Arduino IDE to identify. This provides syntax highlighting features for the library for convenience of the programmer.

#### README.md
The description file containing details about the library. The file that you are looking at right now

### Constants defined
The following constants are defined using `#define` directives
- `MODE_SM`: This is the identifier for the [Sign Magnitude mode](#sign-magnitude-mode). It's value is set to 0.
- `MODE_LAP`: This is the identifier for the [Locked anti-phase mode](#lock-anti-phase-drive-mode). It's value is set to 1.

### Class description
This library assumes the following :-
- You have a motor driver that requires only PWM and DIR (PWM and direction) signals to control the speed and direction of a motor.

The purpose of the *BotBase* class is just to act as a parent class, it's abstract and thus you cannot directly create objects for this class. It's job is only to define the necessary properties and methods for a robot base with wheels.

Let's inspect in detail what all the members of the class do

#### Protected members
##### Variables

- **<font color="#CD00FF">int</font> NUMBER_OF_WHEELS**: The number of motor powered wheels on the base of the bot.

- **<font color="#CD00FF">int</font> \*PWM_pins** : The pin numbers which are connected to the PWM terminal of the motor driver. These tell the motor driver the amount of voltage to be given to the motors.

- **<font color="#CD00FF">int</font> \*PWM_values** : The 8 bit (0 to 255) values of the PWM pin that specify the voltage on that pin. For example : If you're using a controller that operates on 5V logic level, then 0 signifies 0V, 127 signifies 2.4902V and 255 signifies 5V. In short N here specifies N * V/255 volts in real (where V is the logic level voltage).

- **<font color="#CD00FF">int</font> \*DIR_pins** : The pin numbers which are connected to the DIR terminal of the motor driver. These tell the motor driver which direction to rotate the motor in (clockwise or counter clockwise).

- **<font color="#CD00FF">int</font> \*DIR_values** : These are either HIGH or LOW. These are the directions of rotation for the motor. It's your wish to choose the directions (as per convenience).

- **<font color="#CD00FF">bool</font> \*reverseDIRs** : If the particular motor is connected in a reversed fashion, then pass it _true_, else it's defaulted to _false_. The directions are simply reversed while writing to the motor driver if they're true (only for the particular motor for which it is true).

- **<font color="#CD00FF">int</font> \*motorModes**: This is an array consisting of connection modes of the motor drivers connected to individual wheels. The array of identifiers, basically.

- **<font color="#CD00FF">bool</font> modesAttached**: Set to **true** if the `ConfigureModes` function is called, else `false`.

- **<font color="#CD00FF">int</font> LAP_PWM_value**: The PWM value that is to be written onto the DIR output pin, which is connected to the PWM input pin of the motor driver, in case the Locked anti phase mode is used.

- **<font color="#CD00FF">double</font> scalingFactors**: The scaling factors that are to be applied to the final PWM of all the motors. These can be configured using the `SetScalingFactorsTo` function.

- **<font color="#CD00FF">bool</font> scalingFactorsAttached**: This is set to `true` if the function `SetScalingFactorsTo` is called, else it's `false`.


##### Member functions
- **<font color="#CD00FF">void</font> <font color="#5052FF">setNumberOfWheelsTo</font>(<font color="#FF00FF">int</font> number)** : Sets the *NUMBER_OF_WHEELS* value to the passed *number*. It's a good idea to make a call to this in the constructor of the derived classes.

- **<font color="#CD00FF">void</font> <font color="#5052FF">VectorTo_PWM_DIR</font>(<font color="#FF00FF">int</font> \*vector)** : To convert an array of vector wheel rotations to DIR and PWM values to be given to the motors. Make note that the array must have the same length as the number of wheels. This function does **not** call the _MoveMotor_ function.

#### Public Members
##### Variables
- **<font color="#CD00FF">DebuggerSerial</font> debugger**: The debugger object for the class. In case you want to use it, you'll have to initialize it. Read the documentation about **DebuggerSerial** class [here](./../DebuggerSerial/).

##### Constructors
Though you'll never create any memory for objects of this class, it's advised to have a constructor anyways.

- **<font color="#5052FF">BotBase</font>()** : Empty constructor

- **<font color="#5052FF">BotBase</font>(<font color="#CD00FF">int</font> \*modes)** : Initialization constructor if you also want to initialize the modes of the motor drivers. It calls the *ConfigureModes* function.

##### Functions
- **<font color="#CD00FF">void</font> <font color="#5052FF">AttachPins</font>(<font color="#CD00FF">int</font> \*PWM_pins, <font color="#CD00FF">int</font> \*DIR_pins)** : To attach the pins of the motors connected to the motor drivers. Pass it the list of *PWM_pins* and *DIR_pins*.

- **<font color="#CD00FF">void</font> <font color="#5052FF">AttachPins</font>(<font color="#CD00FF">int</font> \*PWM_pins, <font color="#CD00FF">int</font> \*DIR_pins, <font color="#CD00FF">bool</font> \*reverseDIRs)** : To attach the pins of the motors connected to the motor drivers. Pass it the list of *PWM_pins* and *DIR_pins*. You can also initialize the array for _reverseDIRs_ through this.

- **<font color="#CD00FF">void</font> <font color="#5052FF">ConfigureModes</font>(<font color="#CD00FF">int</font> \*modes)**: Configuring the mode of motor drivers. You have to pass it an array of identifiers which describe the motor connection mode of each wheel. For example, if you have a 4 wheeled bot whose third wheel (2nd index in array) is configured to MODE_SM and rest are configured to MODE_LAP, then call it this way:
    ```
    int modeArray[4] = {MODE_LAP, MODE_LAP, MODE_SM, MODE_LAP};
    botBaseObject.ConfigureModes(modeArray);
    ```
    In case you do not call this function, all motors are configured to MODE_SM.

- **<font color="#CD00FF">void</font> <font color="#5052FF">setLAP_PWMto</font>(<font color="#CD00FF">int</font> PWM_value)** : This function must be called if you've configured a motor to MODE_LAP. It must be passed the PWM value that you want the PWM input pin of the motor driver to receive through the DIR pin.

- **<font color="#CD00FF">void</font> <font color="#5052FF">SetScalingFactorsTo</font>(<font color="#CD00FF">double</font> \*factors)** : This function is used to configure the scaling factors (the individual factors by which the PWMs are to be scaled while writing to the motors). Default value is 1 for all the motors.

- **_virtual_ <font color="#CD00FF">void</font> <font color="#5052FF">Move_PWM_Angle</font>(<font color="#CD00FF">int</font> PWM, <font color="#CD00FF">float</font> angle, <font color="#CD00FF">float</font> w = 0)** = 0 : An abstract function which you must implement in the derived classes. This function has the code to move your bot at a particular speed (*PWM*) and in a particular direction (*angle* in radians) with a particular angular velocity about the center (*w*, which is defaulted to 0). You needn't implement the actuation code (it's written in the _Move_ function for you).

- **<font color="#CD00FF">void</font> <font color="#5052FF">Move</font>(<font color="#CD00FF">int</font> PWM, <font color="#CD00FF">int</font> angle_degrees, <font color="#CD00FF">float</font> w = 0)** : This is function that the user will call. It simply calls the *Move_PWM_Angle* function with the angle converted from degrees to radians, then it actuates the motors by calling _MoveMotor_ onto the individual motors.

- **<font color="#CD00FF">void</font> <font color="#5052FF">MoveMotor</font>(<font color="#CD00FF">int</font> motor_number)** : Moves the motor indexed at *motor_number* giving values considering the mode of the motor driver.

## Making you own custom BotBase
You must extend the properties of the class *BotBase* through inheritance into your own class (for your custom *BotBase*). There is a library made for this demo purpose, it's the **FourSBase** folder in this repository.

### Guide
After you place this library folder in the libraries folder of the Arduino environment, you can include it in your codes. To know more about making your own library, you can read [this](../README.md) file.<br>
Perform the following to make use of this library in your BotBase :
- Include the library in your header file (\*.h file). You can check a sample header file [here](../FourSBase/FourSBase.h).
- Inherit the class *BotBase* publicly, so that the `Move` function does not fall under the _private_ or _protected_ scope.
- You can define properties just for your bot as well, in case you want some.
- You must implement the following function definitions:
    - **void Move_PWM_Angle(int PWM, float angle_radians, float w = 0)** : Contains code to move your bot at a particular PWM and at a particular angle (in radians). 
    - It is suggested that you have a function to attach pins only in case you have more properties that require Initialization. You may call the function **AttachPins** in the BotBase class from it.
    - Additionally, make sure that you've called the **setNumberOfWheelsTo** function to set the number of wheels (motors that you're actuating), it's suggested that you implement this in the constructors of the class. For example, if you're dealing with a four wheel bot, then it is 4.
- After that, include the library you're working on into the code and create an object for the class. 

#### Examples
We have already made some example libraries, just to show you how to make your own libraries using the BotBase class.
- **NWCHBase** library: This library is for an **N** wheel base made using omni wheels and all their axis coincide.
- **FourSBase** library: This library is for a four wheel omni base, you can click [here](../FourSBase) to check it out.

[![Developed using VSCode](https://img.shields.io/badge/developed%20using-VSCode-lightgrey.svg)](https://code.visualstudio.com/)
[![Developed using Atom](https://img.shields.io/badge/developed%20using-Atom-lightgrey.svg)](https://atom.io/)
[![Developer @TheProjectsGuy](https://img.shields.io/badge/Developer-TheProjectsGuy-blue.svg)](https://github.com/TheProjectsGuy)
