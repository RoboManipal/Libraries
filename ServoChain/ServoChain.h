/* ServoChain.h- Library to deal with dynamixels and pololus linked in a daisy chain each
Copyright (c) 2018 RoboManipal. All rights reserved
 File created by : Akshatha Kamath
*/
#ifndef SERVOCHAIN
#define SERVOCHAIN

#include "Arduino.h"
#include <XYZrobotServo.h>
// Include the library as per requirement
// Library for using the dynamixel chain on Serial1
#include <DynamixelSerial1.h>
//#include <DynamixelSerial2.h>
//#include <DynamixelSerial3.h>

class ServoChain {
protected:
  int *positions;
  int *servo_IDs;
  int *select;
  int chain_length;
  //########### Debugger #########################
  HardwareSerial *servoDebuggerSerial;
  int debuggerPriorityLevel;  //To set the verbocity of the debugger

  //######## Features of the dynamixels ###########
  int NUMBER_OF_DYNAMIXELS;
  int selection_pin;
  int dynamixel_speeds; // to set a constant speed for all dynamixels in the chain
  HardwareSerial *dynamixelSerial;

    //######### Features of the Pololus #############
  int NUMBER_OF_POLOLUS;
  int pullup_pin; // input pullup pin for pololus
  int pololu_playtimes;
  XYZrobotServo *pololu;
  Stream *pololuSerial;

  //######Debugger ########
  void DebuggerOutput(int Level, String output);
public:
  //######## Initializer functions ########
  //Constructors
  ServoChain();
  ServoChain(HardwareSerial *dynamixel_serial, Stream *pololu_serial, HardwareSerial *debugger_serial,int Level);
  // Setting the debugger priority, messages less than this level are not displayed
  void SetDebuggerPriorityToLevel(int minLevel);
  // User inputs a binary chain where 1 represents dynamixel and 0 -pololu
  void ServoChain::Initialize(int chain_length,int *select_array,int *servo_IDs);
  // Configure Dynamixel selection pin
  void AddSelectionPin(int selection_pin);
  //To change Dynamixel ID
  void changeDynamixelID(int current_ID, int new_ID);
  // configure input pull_up pin for pololus
  void AddPullupPin(int pull_pin);


  //########### Servo move functions ################
  //  To set a constant speed for all the Dynamixels in the chain
  void setDynamixelSpeed(int speed_value);
  // To set a constant playtime to all the pololus
  void setPololuPlaytime(int playtime_value);
  // To actuate the dynamixels to a position at the same speed for all
  void ActuateToPosition(int *position_degrees);

//############ Functions to read joint positions ##########
  int pololuReadPosition(XYZrobotServo &servo);
  // returns position values of the servos for 0-1023
  void readPositions();
  // Reads the positions of all the servos linked in the chain
  void readPositions_degrees();

  //####### Functions to test the working of Dynamixels ##########
  // Uses broadcast ID to move servos
  void testActuatetoPositions(int position_degrees);
};

#endif
