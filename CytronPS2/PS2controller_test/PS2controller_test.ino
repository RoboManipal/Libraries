/* Example code to Test the PS2 Controller in Analog mode 
and use it to retrieve speed and angle from PS2 analog values*/

#include <CytronPS2.h>
// SoftwareSerial myserial(A11,A12); // Uncomment it and modify pins as per requirement
CytronPS2 myPS2;

void setup() {
  // put your setup code here, to run once:

Serial.begin(57600);
myPS2.AttachDebugSerial();
//myPS2.AttachSerial(&myserial); // Uncomment for Software Serial 
myPS2.AttachSerial(&Serial2);
Serial2.begin(115200);
Serial.println("Going into loop ... ");

//myPS2.InitializeDebugger(&Serial,0);
}

void loop() {

myPS2.ReadPS2Values(false,1);
delay(40);
}
