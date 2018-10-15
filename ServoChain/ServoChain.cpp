/* ServoChain.h- Library to deal with n dynamixels linked in a daisy chain
Copyright (c) 2018 RoboManipal. All rights reserved
 File created by : Akshatha Kamath
*/
#include "ServoChain.h"
#include <DynamixelSerial1.h>
#include <XYZrobotServo.h>
//########### Parent class: ServoChain #############
// Set number of servos in daisy chain
/*void ServoChain::setNumberOfDynamixelsTo(int dynamixel_number)
{
  // Number of sevos passed
  this->NUMBER_OF_DYNAMIXELS=number;
}
void ServoChain::setNumberOfPololusTo(int pololu_number)
{
  // Number of sevos passed
  this->NUMBER_OF_POLOLUS=number;
}*/
//Constructors
ServoChain::ServoChain(){
  // Blank Constructor
}
ServoChain::ServoChain(HardwareSerial *dynamixel_serial, Stream *pololu_serial,HardwareSerial *debugger_serial,int Level)
{
  // Dynamixel serial
  this->dynamixelSerial=dynamixel_serial;
  // Pololu Serial
  this->pololuSerial=pololu_serial;
  // Debugger serial of the daisy chain
  this->servoDebuggerSerial=debugger_serial;
  // Priority level of the debugger
  this->debuggerPriorityLevel=Level;

}
// Debugger prioriy level adjustment
void ServoChain::SetDebuggerPriorityToLevel(int minLevel) {
    this->debuggerPriorityLevel = minLevel;
}
void ServoChain::Initialize(int chain_length,int *select_array,int *servo_IDs)
{
  this->select=select_array;
  this->chain_length=chain_length;
  for(int i=0;i<chain_length;i++)
  {
    this->servo_IDs[i]=servo_IDs;
    if(select_array[i]==0)
    {
      pololu[i].InitializePololuServo(this->pololuSerial, this->servo_IDs[i]);
    }
  }
}
// Configure servo selection pins
void ServoChain::AddSelectionPin(int selection_pin)
{
  this->selection_pin=selection_pin;

                // Debugger level is 1 (lowest priority)
        String message = "Dynamixels ";
        message.concat(" attached to selection pin: ");
        message.concat(selection_pin);
        // Example = Dynamixel attached to selection pin: 27
        this->DebuggerOutput(2, message);

}
void ServoChain:: AddPullupPin(int pull_pin)
{
  this->pullup_pin=pull_pin;
                // Debugger level is 1 (lowest priority)
        String message = "Pololu ";
        message.concat(" attached to input pullup pin: ");
        message.concat(pullup_pin);
        // Example = Pololu attached to input pull up pin: 27
        this->DebuggerOutput(2, message);
}
void ServoChain::changeDynamixelID(int current_ID, int new_ID)
{
  //calls the DynamixelSeriali library function to change the IDs of all servos in the daisy chain to new_ID
  Dynamixel.setID(current_ID,new_ID);
}
//  To set a constant speed for all the Dynamixels in the chain
void ServoChain::setDynamixelSpeed(int speed_value)
{
  this->dynamixel_speeds=speed_value;
}
// To actuate the dynamixels to a position at the same speed for all
void ServoChain::ActuateToPosition(int *position_degrees)
{
for (int i=0; i<(NUMBER_OF_DYNAMIXELS+NUMBER_OF_POLOLUS);i++)
{ if(select==1){
  // To convert angle in degrees (0-300) to a scale of 0-1023
  this->positions[i]=position_degrees[i]*1023/300;
  Dynamixel.moveSpeed(this->servo_IDs[i],this->positions[i],this->dynamixel_speeds);
  delay(200);
}
else{
  this->positions[i]=position_degrees[i]*1023/330;
  pololu_servo[i].setPosition(this->positions[i],this->pololu_playtimes);
}
}
}
// Function to read Pololu position
int ServoChain:: pololuReadPosition(XYZrobotServo &servo)
{
  XYZrobotServoStatus status=servo.readStatus();
  return(status.position);
}
// Prints the value of the position on a range of 0-1023
void ServoChain::readPositions()
{
  for (int i=0; i<(NUMBER_OF_DYNAMIXELS+NUMBER_OF_POLOLUS);i++)
  {
    if(select==1)
    {
    Serial.print("Dynamixel position");
    Serial.print(i+1);
    Serial.println(Dynamixel.readPosition(this->servo_IDs[i]));
   }
    else
    {
      Serial.print("Pololu position");
      Serial.print(i+1);
      Serial.println(pololuReadPosition(this->pololu_servo[i]);
    }
  }
}
void ServoChain::readPositions_degrees()
{
  int position_in_degrees;
  for (int i=0; i<chain_length;i++)
  {
    if(select==1)
    {
      Serial.print("Dynamixel position");
      Serial.print(i+1);
      position_in_degrees=Dynamixel.readPosition(this->servo_IDs[i])*300/1023;
    }
    else
    {
      Serial.print("Pololu position");
      Serial.print(i+1);
      position_in_degrees=pololuReadPosition(this->pololu_servo[i]);
    }
    Serial.println(position_in_degrees);
  }
}

// Used to test if we can read/write position to Dynamixels using broadcast ID
void ServoChain::testActuatetoPositions(int position_degrees)
{
  Dynamixel.moveSpeed(254, position_degrees,60);
  for (int i=0; i<NUMBER_OF_DYNAMIXELS;i++)
  {
    Serial.print("Dynamixel position");
    Serial.print(i+1);
    Serial.println(Dynamixel.readPosition(this->servo_IDs[i]));

  }
}
