/*
Original written by:
            Cytron Technologies

Modified:
  29/06/15  Idris, Cytron Technologies    - Point to IDE SoftwareSerial
                                          - Restructure the code style to follow standard Arduino library
*/

#include "CytronPS2.h"

CytronPS2::CytronPS2()
{


}
void CytronPS2:: AttachDebugSerial()
{
	this->debugger.AttachSerial(&Serial);
	this->debugger.print("Debugger Serial attached", INFO);
}
void CytronPS2:: AttachSerial(Stream *ps2_serial)
{
	this->ps2Serial=ps2_serial;
}
void CytronPS2::write(uint8_t data)
{
		while(ps2Serial->available() > 0) {
			ps2Serial->read();
		}
		ps2Serial->write(data);
		ps2Serial->flush();		// Wait for all data transmitted

}

uint8_t CytronPS2::read(void)
{
	uint8_t rec_data;
	long waitcount = 0;


		while(true)
		{
			if(ps2Serial->available() > 0)
			{
				rec_data = ps2Serial->read();
				//SERIAL_ERR = false;
				return(rec_data);
			}
			waitcount++;
			if(waitcount > 50000)
			{
				//SERIAL_ERR = true;
				return(0xFF);
			}
		}

}

uint8_t CytronPS2::readButton(uint8_t key)
{
	//if(!hardwareSerial) PS2Serial->listen();
	write(key);
	return read();
}

//To obtain the analog values from the left side of the joystick
void CytronPS2::ReadPS2Values(bool select, float scaling_factor)
{
	  float temp;
		if(select)
		{
  // Read analog values from the right side of the joystick
  this->left_x = 255 - readButton(PS2_JOYSTICK_RIGHT_X_AXIS);
  this->left_y = readButton(PS2_JOYSTICK_RIGHT_Y_AXIS);
		}
		else
		{
			// Read analog values from the left side of the joystick
		  this->left_x = 255 - readButton(PS2_JOYSTICK_LEFT_X_AXIS);
		  this->left_y = readButton(PS2_JOYSTICK_LEFT_Y_AXIS);
		}
	// Values mapped from 0 - 255 to -127- 128 to emulate Cartesian coordinates
  //Also the values are mapped from -127 to 127 to ensure a square which will later be used in the mapping
  this->left_x=-(this->left_x-127);
  if(this->left_x==-128)
    {
      this->left_x=-127;
    }
  this->left_y=-(this->left_y-127);
  if(this->left_y==-128){
    this->left_y=-127;
  }


// SIMPLE STRETCHING
// The following equations map the sqaure formed to a circle . Refer to the README for documentation
  if(pow(this->left_x,2)>=pow(this->left_y,2))
  {
    temp=pow(this->left_x,3)/(sqrt(pow(this->left_x,2)+pow(this->left_y,2))*abs(this->left_x));
    this->left_y=(this->left_y*pow(this->left_x,2))/(sqrt(pow(this->left_x,2)+pow(this->left_y,2))*abs(this->left_x));
    this->left_x=temp;
  }

  else
  {
    temp=(this->left_x*pow(this->left_y,2))/(sqrt(pow(this->left_x,2)+pow(this->left_y,2))*abs(this->left_y));
    this->left_y=pow(this->left_y,3)/(sqrt(pow(this->left_x,2)+pow(this->left_y,2))*abs(this->left_y));
    this->left_x=temp;

  }

	// CAlculate the angle of desired motion
	temp=(float)this->left_y/ this->left_x;
	this->angle=atan2(this->left_x,this->left_y);
	// To keep angle range 2pi radians
	if(this->angle<0)
	{
		this->angle=3.14+(3.14+this->angle);
	}
	// The scaling factor is present to modify the maximum permissible speed for the bot
	this->speeds=scaling_factor* (pow(pow(this->left_x,2)+pow(this->left_y,2),0.5));

  // Debugger message (Level: DEBUG)
  // X: %X_COORDINATE%  Y:%Y_COORDINATE%
  // For example: X: 0  Y: 127
  String msg="X: ";
  msg.concat(this->left_x);
  msg.concat("   Y: ");
  msg.concat(this->left_y);
  this->debugger.print(msg,DEBUG);

	String message="Angle: ";
  message.concat(this->angle);
  message.concat("      Speed : ");
  message.concat(speeds);
  this->debugger.print(message,DEBUG);

}
