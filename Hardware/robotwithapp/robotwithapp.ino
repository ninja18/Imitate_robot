//#include <SoftwareSerial.h>
#include <Servo.h>
#include<Ticker.h>

Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;
Servo servo06;

Ticker logger;
//SoftwareSerial Bluetooth(3, 4); // Arduino(RX, TX) - HC-05 Bluetooth (TX, RX)

uint8_t servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos, servo6Pos; // current position
uint8_t servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos, servo6PPos; // previous position
uint8_t servo01SP[50] {60,60,90,60,60}, servo02SP[50] {150,90,90,120,120}, servo03SP[50] {35,35,35,35,35}, servo04SP[50] {140,140,40,40,40}, servo05SP[50] {85,85,85,85,85}, servo06SP[50] {80,80,80,80,120}; // for storing positions/steps

uint8_t servo01Act[100], servo02Act[100], servo03Act[100], servo04Act[100], servo05Act[100], servo06Act[100]; // for actions
uint8_t speedDelay = 20;
uint8_t ind = 5;
String dataIn = "";
uint8_t logger_i = 0;
bool isLog = false;
bool isRun = true;

void log_data(){
  if(isLog){
  servo01Act[logger_i] = servo1PPos;
  servo02Act[logger_i] = servo2PPos;
  servo03Act[logger_i] = servo3PPos;
  servo04Act[logger_i] = servo4PPos;
  servo05Act[logger_i] = servo5PPos;
  servo06Act[logger_i] = servo6PPos;
  logger_i++;
}
}

void setup() {
  servo01.attach(D5);
  servo02.attach(D6);
  servo03.attach(D7);
  servo04.attach(D8);
  servo05.attach(D3);
  servo06.attach(D4);
  //Bluetooth.begin(38400); // Default baud rate of the Bluetooth module
  //Bluetooth.setTimeout(1);
  delay(20);
  Serial.begin(115200);
  // Robot arm initial position
  servo1PPos = 60;
  servo01.write(servo1PPos);
  servo2PPos = 150;
  servo02.write(servo2PPos);
  servo3PPos = 35;
  servo03.write(servo3PPos);
  servo4PPos = 140;
  servo04.write(servo4PPos);
  servo5PPos = 85;
  servo05.write(servo5PPos);
  servo6PPos = 80;
  servo06.write(servo6PPos);
  Serial.println("Starting");
  logger.attach(0.1,log_data);
}

void loop() {
  // Check for incoming data
  /*if (Bluetooth.available() > 0) {
    dataIn = Bluetooth.readString();  // Read the data as string
    
    // If "Waist" slider has changed value - Move Servo 1 to position
    if (dataIn.startsWith("s1")) {
      String dataInS = dataIn.substring(2, dataIn.length()); // Extract only the number. E.g. from "s1120" to "120"
      servo1Pos = dataInS.toInt();  // Convert the string into integer
      // We use for loops so we can control the speed of the servo
      // If previous position is bigger then current position
      if (servo1PPos > servo1Pos) {
        for ( int j = servo1PPos; j >= servo1Pos; j--) {   // Run servo down
          servo01.write(j);
          delay(20);    // defines the speed at which the servo rotates
        }
      }
      // If previous position is smaller then current position
      if (servo1PPos < servo1Pos) {
        for ( int j = servo1PPos; j <= servo1Pos; j++) {   // Run servo up
          servo01.write(j);
          delay(20);
        }
      }
      servo1PPos = servo1Pos;   // set current position as previous position
    }
    
    // Move Servo 2
    if (dataIn.startsWith("s2")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      servo2Pos = dataInS.toInt();

      if (servo2PPos > servo2Pos) {
        for ( int j = servo2PPos; j >= servo2Pos; j--) {
          servo02.write(j);
          delay(50);
        }
      }
      if (servo2PPos < servo2Pos) {
        for ( int j = servo2PPos; j <= servo2Pos; j++) {
          servo02.write(j);
          delay(50);
        }
      }
      servo2PPos = servo2Pos;
    }
    // Move Servo 3
    if (dataIn.startsWith("s3")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      servo3Pos = dataInS.toInt();
      if (servo3PPos > servo3Pos) {
        for ( int j = servo3PPos; j >= servo3Pos; j--) {
          servo03.write(j);
          delay(30);
        }
      }
      if (servo3PPos < servo3Pos) {
        for ( int j = servo3PPos; j <= servo3Pos; j++) {
          servo03.write(j);
          delay(30);
        }
      }
      servo3PPos = servo3Pos;
    }
    // Move Servo 4
    if (dataIn.startsWith("s4")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      servo4Pos = dataInS.toInt();
      if (servo4PPos > servo4Pos) {
        for ( int j = servo4PPos; j >= servo4Pos; j--) {
          servo04.write(j);
          delay(30);
        }
      }
      if (servo4PPos < servo4Pos) {
        for ( int j = servo4PPos; j <= servo4Pos; j++) {
          servo04.write(j);
          delay(30);
        }
      }
      servo4PPos = servo4Pos;
    }
    // Move Servo 5
    if (dataIn.startsWith("s5")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      servo5Pos = dataInS.toInt();
      if (servo5PPos > servo5Pos) {
        for ( int j = servo5PPos; j >= servo5Pos; j--) {
          servo05.write(j);
          delay(30);
        }
      }
      if (servo5PPos < servo5Pos) {
        for ( int j = servo5PPos; j <= servo5Pos; j++) {
          servo05.write(j);
          delay(30);
        }
      }
      servo5PPos = servo5Pos;
    }
    // Move Servo 6
    if (dataIn.startsWith("s6")) {
      String dataInS = dataIn.substring(2, dataIn.length());
      servo6Pos = dataInS.toInt();
      if (servo6PPos > servo6Pos) {
        for ( int j = servo6PPos; j >= servo6Pos; j--) {
          servo06.write(j);
          delay(30);
        }
      }
      if (servo6PPos < servo6Pos) {
        for ( int j = servo6PPos; j <= servo6Pos; j++) {
          servo06.write(j);
          delay(30);
        }
      }
      servo6PPos = servo6Pos; 
    }
    // If button "SAVE" is pressed
    if (dataIn.startsWith("SAVE")) {
      servo01SP[index] = servo1PPos;  // save position into the array
      servo02SP[index] = servo2PPos;
      servo03SP[index] = servo3PPos;
      servo04SP[index] = servo4PPos;
      servo05SP[index] = servo5PPos;
      servo06SP[index] = servo6PPos;
      index++;                        // Increase the array index
    }
    // If button "RUN" is pressed
    if (dataIn.startsWith("RUN")) {*/
     if(isRun){
     isLog = true;
      runservo(); // Automatic mode - run the saved steps 
      isRun = false;
      isLog = false;
      Serial.print("Actions collected is ");
      Serial.println(logger_i);
      for(uint8_t i = 0; i < logger_i;i++)
      {
        Serial.print(i);
        Serial.print(" : ");
        Serial.print(servo01Act[i]);
        Serial.print(", ");
        Serial.print(servo02Act[i]);
        Serial.print(", ");
        Serial.print(servo03Act[i]);
        Serial.print(", ");
        Serial.print(servo04Act[i]);
        Serial.print(", ");
        Serial.print(servo05Act[i]);
        Serial.print(", ");
        Serial.print(servo06Act[i]);
        Serial.println("    OVer...."); //,servo02Act[i],servo03Act[i],servo04Act[i],servo05Act[i],servo06Act[i]);
      }
     }
    /*}
    // If button "RESET" is pressed
    if ( dataIn == "RESET") {
      memset(servo01SP, 0, sizeof(servo01SP)); // Clear the array data to 0
      memset(servo02SP, 0, sizeof(servo02SP));
      memset(servo03SP, 0, sizeof(servo03SP));
      memset(servo04SP, 0, sizeof(servo04SP));
      memset(servo05SP, 0, sizeof(servo05SP));
      memset(servo06SP, 0, sizeof(servo06SP));
      index = 0;  // Index to 0
    }
  }*/
}

// Automatic mode custom function - run the saved steps
void runservo() {
  //while (dataIn != "RESET") {  // Run the steps over and over again until "RESET" button is pressed
    for (uint8_t i = 0; i <= ind - 2; i++) { // Run through all steps(index)
      /*if (Bluetooth.available() > 0) {      // Check for incomding data
        dataIn = Bluetooth.readString();
        if ( dataIn == "PAUSE") {           // If button "PAUSE" is pressed
          while (dataIn != "RUN") {         // Wait until "RUN" is pressed again
            if (Bluetooth.available() > 0) {
              dataIn = Bluetooth.readString();
              if ( dataIn == "RESET") {     
                break;
              }
            }
          }
        }
        // If speed slider is changed
        if (dataIn.startsWith("ss")) {
          String dataInS = dataIn.substring(2, dataIn.length());
          speedDelay = dataInS.toInt(); // Change servo speed (delay time)
        }
      }*/
      // Servo 1
      if (servo01SP[i] == servo01SP[i + 1]) {
        
      }
      if (servo01SP[i] > servo01SP[i + 1]) {
        for (servo1PPos = servo01SP[i]; servo1PPos >= servo01SP[i + 1]; servo1PPos--) {
          
          servo01.write(servo1PPos);
          delay(speedDelay);
        }
      }
      if (servo01SP[i] < servo01SP[i + 1]) {
        for ( servo1PPos = servo01SP[i];servo1PPos <= servo01SP[i + 1]; servo1PPos++) {
         
          servo01.write(servo1PPos);
          delay(speedDelay);
        }
      }

      // Servo 2
      if (servo02SP[i] == servo02SP[i + 1]) {
        
      }
      if (servo02SP[i] > servo02SP[i + 1]) {
        for ( servo2PPos = servo02SP[i]; servo2PPos >= servo02SP[i + 1]; servo2PPos--) {
          
          servo02.write(servo2PPos);
          delay(speedDelay);
        }
      }
      if (servo02SP[i] < servo02SP[i + 1]) {
        for ( servo2PPos= servo02SP[i]; servo2PPos <= servo02SP[i + 1]; servo2PPos++) {
         
          servo02.write(servo2PPos);
          delay(speedDelay);
        }
      }

      // Servo 3
      if (servo03SP[i] == servo03SP[i + 1]) {
       
      }
      if (servo03SP[i] > servo03SP[i + 1]) {
        for ( servo3PPos = servo03SP[i]; servo3PPos >= servo03SP[i + 1]; servo3PPos--) {
          
          servo03.write(servo3PPos);
          delay(speedDelay);
        }
      }
      if (servo03SP[i] < servo03SP[i + 1]) {
        for (servo3PPos = servo03SP[i]; servo3PPos <= servo03SP[i + 1]; servo3PPos++) {
          
          servo03.write(servo3PPos);
          delay(speedDelay);
        }
      }

      // Servo 4
      if (servo04SP[i] == servo04SP[i + 1]) {
        
      }
      if (servo04SP[i] > servo04SP[i + 1]) {
        for ( servo4PPos = servo04SP[i]; servo4PPos >= servo04SP[i + 1]; servo4PPos--) {
          
          servo04.write(servo4PPos);
          delay(speedDelay);
        }
      }
      if (servo04SP[i] < servo04SP[i + 1]) {
        for ( servo4PPos = servo04SP[i]; servo4PPos <= servo04SP[i + 1]; servo4PPos++) {
          
          servo04.write(servo4PPos);
          delay(speedDelay);
        }
      }

      // Servo 5
      if (servo05SP[i] == servo05SP[i + 1]) {
       
      }
      if (servo05SP[i] > servo05SP[i + 1]) {
        for (servo5PPos = servo05SP[i]; servo5PPos >= servo05SP[i + 1]; servo5PPos--) {
          
          servo05.write(servo5PPos);
          delay(speedDelay);
        }
      }
      if (servo05SP[i] < servo05SP[i + 1]) {
        for (servo5PPos = servo05SP[i]; servo5PPos <= servo05SP[i + 1]; servo5PPos++) {
          
          servo05.write(servo5PPos);
          delay(speedDelay);
        }
      }

      // Servo 6
      if (servo06SP[i] == servo06SP[i + 1]) {
        
      }
      if (servo06SP[i] > servo06SP[i + 1]) {
        for ( servo6PPos = servo06SP[i]; servo6PPos >= servo06SP[i + 1]; servo6PPos--) {
         
          servo06.write(servo6PPos);
          delay(speedDelay);
        }
      }
      if (servo06SP[i] < servo06SP[i + 1]) {
        for ( servo6PPos = servo06SP[i];servo6PPos <= servo06SP[i + 1]; servo6PPos++) {
        
          servo06.write(servo6PPos);
          delay(speedDelay);
        }
      }
    }
  //}
}


