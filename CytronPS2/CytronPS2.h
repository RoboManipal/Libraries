/*
Original written by:
            Cytron Technologies

Modified:
  03/12/18
*/

#ifndef CytronPS2_h
#define CytronPS2_h

#include "Arduino.h"
#include "DebuggerSerial.h"

// Arduino Leonardo
#if defined (__AVR_ATmega32U4__)
  #define Serial Serial1
#else
  #define Serial Serial
#endif

// Define PS2 button to number
enum {
  // Digital button
  PS2_SELECT,
  PS2_JOYSTICK_LEFT,
  PS2_JOYSTICK_RIGHT,
  PS2_START,
  PS2_UP,
  PS2_RIGHT,
  PS2_DOWN,
  PS2_LEFT,
  PS2_LEFT_2,
  PS2_RIGHT_2,
  PS2_LEFT_1,
  PS2_RIGHT_1,
  PS2_TRIANGLE,
  PS2_CIRCLE,
  PS2_CROSS,
  PS2_SQUARE,
  // Analog button
  PS2_JOYSTICK_LEFT_X_AXIS,
  PS2_JOYSTICK_LEFT_Y_AXIS,
  PS2_JOYSTICK_RIGHT_X_AXIS,
  PS2_JOYSTICK_RIGHT_Y_AXIS,
  PS2_JOYSTICK_LEFT_UP,
  PS2_JOYSTICK_LEFT_DOWN,
  PS2_JOYSTICK_LEFT_LEFT,
  PS2_JOYSTICK_LEFT_RIGHT,
  PS2_JOYSTICK_RIGHT_UP,
  PS2_JOYSTICK_RIGHT_DOWN,
  PS2_JOYSTICK_RIGHT_LEFT,
  PS2_JOYSTICK_RIGHT_RIGHT,
  // Check connection status
  PS2_CONNECTION_STATUS,
  // Read all button
  PS2_BUTTON_JOYSTICK
};

class CytronPS2
{
  private:
    Stream *ps2Serial;
    void write(uint8_t data);
    uint8_t read(void);

  public:

    // Hardware Serial
    CytronPS2();
    void AttachDebugSerial();
    void AttachSerial(Stream *ps2_serial);
    uint8_t readButton(uint8_t key);
    //####################### Debugger #################
    DebuggerSerial debugger;

    float left_x;
    float left_y;
    // The desired speed and angle values calculated from the PS2
    int speeds;
    float angle;

    // #################### PS2 ###################################
    //To obtain the analog values from the left side of the joystick
    // To convert the coordinates from -128 to 128 and map the space into a square
    //To calculate angle value and the speed from the analog input obtained from the PS2
    void ReadPS2Values(bool select, float scaling_factor);
};

#endif
