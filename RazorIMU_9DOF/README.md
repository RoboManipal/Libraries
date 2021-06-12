![Version tag](https://img.shields.io/badge/version-1.0.1-orange.svg)
# Introduction
This library is for the receiver end of the Razor 9 DOF IMU (reference  [here](https://github.com/Razor-AHRS/razor-9dof-ahrs/tree/master/Arduino/Razor_AHRS)).

# Table of contents
- [Introduction](#introduction)
- [Table of contents](#table-of-contents)
- [User guide](#user-guide)
    - [Downloading the library](#downloading-the-library)
    - [Using the library with Arduino](#using-the-library-with-arduino)
    - [Prerequisites](#prerequisites)
    - [Usage of the library](#usage-of-the-library)
    - [Examples](#examples)
        - [Get_RPY_values](#get_rpy_values)
- [Developers Guide](#developers-guide)
    - [Library Details](#library-details)
        - [Files in the library](#files-in-the-library)
            - [RazorIMU_9DOF.h](#razorimu_9dofh)
            - [RazorIMU_9DOF.cpp](#razorimu_9dofcpp)
            - [keywords.txt](#keywordstxt)
            - [README.md](#readmemd)
        - [Class contents](#class-contents)
            - [Protected members](#protected-members)
                - [Variables](#variables)
                - [Member functions](#member-functions)
        - [Public members](#public-members)
            - [Members](#members)
            - [Constructors](#constructors)
            - [Member functions](#member-functions)
- [Debugger Notifications](#debugger-notifications)
    - [INFO Level](#info-level)
    - [DEBUG Level](#debug-level)

# User guide
## Downloading the library
It is suggested that you download the entire repository and then select this folder, so that you can enjoy the benifits of VCS like git. It makes it simpler to update the contents whenever patch fixes are done. You can simply open a terminal (or gitbash on windows), go to the folder where you want to save this repository and type the following command.
```
git clone https://github.com/RoboManipal/Libraries.git -b dev
```
_You might want to omit the `-b <branch>` tag if you're downloading from the master branch_.

**Not recommended**: You can download _only_ this folder by clicking [here](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/RoboManipal/Libraries/tree/Branch-Avneesh/RazorIMU_9DOF)

## Using the library with Arduino
Move this folder into the arduino libraries folder on your PC. If you don't know where the libraries folder of your arduino is, you can check out the README file of this entire repository for this, click [here](../README.md).<br>

## Prerequisites
Please follow the following steps before working with this library:
- Download the official code folder from [here](https://minhaskamal.github.io/DownGit/#/home?url=https://github.com/Razor-AHRS/razor-9dof-ahrs/tree/master/Arduino/Razor_AHRS), you'll be uploading this to the IMU.
- Extract folder *Razor_AHRS* to an appropriate location, open the *Razor_AHRS* file in Arduino IDE.
- Under the tools dropdown, select the following:
    - **Board**: Arduino Pro or Pro Mini
    - **Processor**: Atmega 328P (3.3V, 8MHz)
    - **Port**: The connected port (/dev/ttyUSB\* (or ttyACM\*) for Ubuntu, COM\* for windows).
- In the code of *Razor_AHRS* file, go to **USER SETUP AREA** and uncomment the correct #define HW\_\_VERSION\_CODE. It's 10736 in our examples, check the version from the correct hardware datasheet.
- Set the OUTPUT\_\_STARTUP\_STREAM\_ON to *false*. This will disable the continuous output stream.
- Upload the code.
- Open serial monitor
- Set baud rate to 57600
- There must be output

More info about setting up software  [here](https://github.com/Razor-AHRS/razor-9dof-ahrs/wiki/Tutorial#setting-up-the-software).

## Usage of the library
In order to use this library, you must do the following:
- Include the header file `RazorIMU_9DOF.h` (the *RazorIMU_9DOF* library folder must be in your arduino libraries folder).
- Create an object of class `RazorIMU_9DOF`. You can pass the constructor parameters to initialize here itself, or leave it as a simple object declaration.
- Initialize the Serial stream on which the IMU is attached using the `AttachIMUSerial` function. You may optionally initialize and attach a debugger serial as well (using `debugger.Initialize` function on the object), more info about debugger [here](./../DebuggerSerial/). You can access the debugger using the `debugger` variable of the class.
- You can optionally set the reference values of the IMU at this stage, you must call the `ResetReference` function for that.
- To avoid wastage of resources, the library doesn't continuously poll the serial. You must call the function `UpdateData` for the library to fetch the values from the serial.
- To get the values, you can use one of the getter functions, for example if you want to access the YAW value, then you could use any of the following functions:
    - **GetYaw** will give you the continuous YAW value (which is 0 to 360).
    - **GetRaw_YAW** will give you the raw value as passed by the IMU (which is -180 to 180).
    - **GetRef_YAW** will give you the deviation in the YAW value from the reference that you've set. By default, references are 0. You must call the `ResetReference` function before calling this. 

## Examples

### Get_RPY_values
This example is to show you how to fetch **YAW**, **PITCH** and **ROLL** values from the IMU using the `RazorIMU_9DOF` library.<br>
File: [RazorIMU_9DOF/examples/Get_RPY_values/Get_RPY_values.ino](./examples/Get_RPY_values/Get_RPY_values.ino)<br>
We simply follow the following steps:
1. Include library
2. Create object
3. Initialize Serials. IMU on **Serial1** and debugger on **Serial**. Keep in mind to match the baud rates.
4. Initialize debugger. Name it `IMU_9DOF`.
5. Attach the serial of the IMU
6. Grab a reference reading
7. Then start a loop
    1. Update the data readings from IMU
    2. Get YAW, PITCH and ROLL values
8. Re-run the loop

We tested this on an Arduino Mega (Atmega 2560 microcontroller). If everything goes fine, you must see a similar result on the debugger screen
<!-- TODO: Update this -->
```
[0] > Debugger enabled
[0] > DebuggerSerial attached
[1] > Debugger set to SENSOR_FEED
[2 DEBUG] $IMU_9DOF$ Sending a '#f' to IMU serial
[26 DEBUG] $IMU_9DOF$ Values Y: -107.03 P: 17.25 R: -0.35
[27 DEBUG] $IMU_9DOF$ Full scale RPY values: 359.65 17.25 252.97 
YAW = 252.97
[33 DEBUG] $IMU_9DOF$ Sending a '#f' to IMU serial
[67 DEBUG] $IMU_9DOF$ Values Y: -106.94 P: 17.28 R: -0.43
[68 DEBUG] $IMU_9DOF$ Full scale RPY values: 359.57 17.28 253.06 
YAW = 253.06
[74 DEBUG] $IMU_9DOF$ Sending a '#f' to IMU serial
[108 DEBUG] $IMU_9DOF$ Values Y: -106.91 P: 17.20 R: -0.37
[110 DEBUG] $IMU_9DOF$ Full scale RPY values: 359.63 17.20 253.09 
YAW = 253.09
```

# Developers Guide
This library has a single class named `RazorIMU_9DOF`. Let's explore all the contents in detail.

## Library Details

### Files in the library

#### RazorIMU_9DOF.h
The header file for the `RazorIMU_9DOF` class. It only has function declarations, not definitions.

#### RazorIMU_9DOF.cpp
This file consists the code for all the functions declared in the `RazorIMU_9DOF` class.

#### keywords.txt
This file consists the list of keywords and their types to be recognized by the Arduino IDE.

#### README.md
The description file that you're currently reading. All documentation here.

### Class contents
Let's explore the contents of the class, but first, we also have literals defined for general purpose use (using `#define`). They are:

| Name | Value | Purpose |
|:----:| :----: | :----- |
| ROLL| 0 | The index of roll values in arrays |
| PITCH | 1 | The index of pitch values in arrays |
| YAW | 2 | The index of yaw values in arrays |

Let's explore the class now

#### Protected members

##### Variables
- **<font color="#CD00FF">float</font> RPY_raw_values[3]**: Stores the raw values (-180 to 180, as output by the module) of Roll, Pitch and Yaw. 
- **<font color="#CD00FF">float</font> RPY_full_scale[3]**: Stores the continuous values (0 to 360, converted from raw values).
- **<font color="#CD00FF">float</font> RPY_full_scale_ref[3]**: The reference values recorded when the `GrabReference` function is executed.
- **<font color="#CD00FF">Stream</font> \*IMU\_Serial**: This is the serial on which IMU operates. The parent **Stream** class allows any kind of serial, **Hardware** or **Software**.

##### Member functions
- **<font color="#CD00FF">void</font> <font color="#5052FF">Calculate_fullScales</font>()**: Calculates the full scale values (which is stored in *RPY\_full\_scale*) from the raw values (which are read from *RPY\_raw\_values*).
- **<font color="#CD00FF">void</font> <font color="#5052FF">GrabReference</font>()**: Update reference, the current values of *RPY\_full\_scale* are stored in *RPY\_full\_scale\_ref*.
- **<font color="#CD00FF">void</font> <font color="#5052FF">GrabData</font>()**: Get data from IMU through the *IMU\_Serial*.

### Public members
#### Members
- **<font color="#CD00FF">DebuggerSerial</font> debugger**: The debugger for the class. Check the [DebuggerSerial documentation](./../DebuggerSerial/) for more on this.

#### Constructors
- **<font color="#5052FF">RazorIMU_9DOF</font>()**: Empty constructor for the class.
- **<font color="#5052FF">RazorIMU_9DOF</font>(Stream \*AttachedSerial)**: To attach a pre-initialized serial to the IMU. This function calls the _AttachIMUSerial_ member function.

#### Member functions
- **<font color="#CD00FF">void</font> AttachIMUSerial(<font color="#FF00FF">Stream</font> \*AttachedSerial)**: Connect the IMU Serial.
- **<font color="#CD00FF">void</font> UpdateData()**: Updates the IMU readings stored in variables of the class.
- **<font color="#CD00FF">void</font> ResetReference()**: Resets the reference values of the IMU.
- **<font color="#CD00FF">float</font> GetRaw_ROLL()**: To get the RAW value of ROLL.
- **<font color="#CD00FF">float</font> GetRaw_PITCH()**: To get the RAW value of PITCH.
- **<font color="#CD00FF">float</font> GetRaw_YAW()**: To get the RAW value of YAW.
- **<font color="#CD00FF">float</font> GetRoll()**: To get the continuous ROLL values.
- **<font color="#CD00FF">float</font> GetPitch()**: To get the continuous PITCH values.
- **<font color="#CD00FF">float</font> GetYaw()**: To get the continuous YAW values.
- **<font color="#CD00FF">float</font> GetRel\_YAW()**: To get the deviation from reference for YAW.
- **<font color="#CD00FF">float</font> GetRel\_PITCH()**: To get the deviation from reference for PITCH.
- **<font color="#CD00FF">float</font> GetRel\_ROLL()**: To get the deviation from reference for ROLL.

<!-- TODO: Update examples -->
# Debugger Notifications
## INFO Level
1. **AttachIMUSerial** function: Gives a notification about the serial attachment.
    ```
    IMU Serial attached
    ```
2. **GrabReference** function: Gives a notification about the reference values begin updated. It shows messages in the following format
    ```
    RPY reference set to: %PITCH_REF% %ROLL_REF% %YAW_REF%
    ```
    For example:
    ```
    ```


## DEBUG Level
1. **GrabData** function: Tells you about sending a `#f` on the IMU serial.
    ```
    Sending a '#f' to IMU serial
    ```
    It also has debugger information about the values received.
    ```
    Y: %YAW% P: %PITCH% R: %ROLL%
    ```
2. **Calculate_fullScales** function: Gives you the result of full scale conversions.
    ```
    Full scale RPY values: %FS_PITCH% %FS_ROLL% %FS_YAW%
    ```

[![Image](https://img.shields.io/badge/developed%20using-VSCode-lightgrey.svg)](https://code.visualstudio.com/)
[![Image](https://img.shields.io/badge/Developer-TheProjectsGuy-blue.svg)](https://github.com/TheProjectsGuy)
