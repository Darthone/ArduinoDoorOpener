//
//  Name:  Door Opener
//  By:    Dario Marasco
//  Date:  12/14/13
//
//	The purpose of this program is to communicate with a bluetooth application 
//  and upon recieving the correct passwork, turn the servo to open the door it 
//  attached to
//
//	This code has only been tested on and is currently work on the arduino uno
//	
//

#include <Servo.h>  // servo library
#include <SoftwareSerial.h> // bluetooth library 

Servo servo1;  // servo control object
int servoPosition;
//i/o pins on the arduino
int bluetoothTx = 3;
int bluetoothRx = 2;
boolean finishPass = false;
boolean changePass = false;
boolean acceptPass = false;
//this is the char that need to be sent for a password change
char changePassend = '!';
//This char is used to end the new password being sent
char passend = ';';
//The default password
//TODO add this to the permanent memory on the board
String password = "hello";
String recievePass;
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup(){BlueToothSetup(); //init bluetooth device
  servo1.attach(9); //init servo.  Digital connector for the servo
  servo1.write(0);     // Tell servo to go to 0 degrees
  delay(1000);         // Pause to get it time to move
}

//Main
void loop(){
  if (bluetooth.available()) {char recvChar = bluetooth.read();
    if(recvChar == passend){finishPass = true;}
    else if(recvChar == changePassend){changePass = true; 
      bluetooth.println("Entering Change Password Mode. Enter current Password to continue.");   
    }else{recievePass += recvChar; 
	  // this is just for debugging purposes
      bluetooth.println("The Password so far: " + recievePass); 
    }
  }
  if(finishPass){
    if(recievePass == password){
      if(changePass){bluetooth.println("Password accepted, accepting next password as new password.");
        changePass = false;
        acceptPass = true;
      }else{bluetooth.println("Password accepted, opening door.");
        OpenDoor();
        bluetooth.println("Door unlocked.");
      }
    }else if(acceptPass){bluetooth.println("The new password is: " + recievePass);
        password = recievePass;
        acceptPass = false;
    }else{bluetooth.println("Invalid Password");
      if(changePass){bluetooth.println("Change Password Mode Exited");
        changePass=false;
      }
    }
    recievePass = "";
    finishPass=false;
  }
  delay(500);
  //todo slow timer down when not recieving messages and speed up to process faster after the first one is recieved
} 

//Functions
void BlueToothSetup(){bluetooth.begin(115200);
  bluetooth.println("SM,0");
  delay(2000);
}

void OpenDoor() {
  for(servoPosition = 0; servoPosition < 180; servoPosition += 5){
    servo1.write(servoPosition);  // Move to next servoPosition
    delay(20);               // Short pause to allow it to move
  }
  delay(5000);
  servo1.write(0);     // Tell servo to go to 0 degrees
  delay(2000);         // 
}
