#include <Servo.h>

// Number of servos/fingers
#define NUM_SERVOS 5

// Servo objects
Servo servos[NUM_SERVOS];

// Servo pins
const int servoPins[NUM_SERVOS] = {0, 2, 4, 6, 8};

// Finger states (0 = open, 1 = closed)
int fingerStates[NUM_SERVOS] = {0};

// Serial communication
String receivedString;

void setup() {
  Serial.begin(9600);
  
  // Attach servos to pins
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servos[i].write(0);  // Initialize to open position
  }
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    
    // Start of data marker
    if (c == '$') {
      receivedString = "";
      while (true) {
        if (Serial.available() > 0) {
          c = Serial.read();
          if (c == '\n') break;  // End of message
          receivedString += c;
        }
      }
      
      // Parse the received string
      parseData(receivedString);
    }
  }
  
  // Update servo positions
  updateServos();
}

void parseData(String data) {
  int index = 0;
  int startPos = 0;
  
  for (int i = 0; i < NUM_SERVOS; i++) {
    int commaPos = data.indexOf(',', startPos);
    if (commaPos == -1) commaPos = data.length();
    
    String valueStr = data.substring(startPos, commaPos);
    fingerStates[i] = valueStr.toInt();
    
    startPos = commaPos + 1;
  }
}

void updateServos() {
  // Thumb (may need different angles than other fingers)
  servos[0].write(fingerStates[0] ? 180 : 0);
  
  // Index finger
  servos[1].write(fingerStates[1] ? 180 : 0);
  
  // Middle finger
  servos[2].write(fingerStates[2] ? 180 : 0);
  
  // Ring finger
  servos[3].write(fingerStates[3] ? 180 : 0);
  
  // Pinky finger
  servos[4].write(fingerStates[4] ? 180 : 0);
}