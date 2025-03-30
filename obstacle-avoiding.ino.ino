# include <Servo.h>
#define echoPin A0
#define trigPin A1

Servo scanServo;

// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

unsigned long startTime;


void setup() {
  startTime = millis(); // Record the start time

  scanServo.attach(A2); // A2 is the signal pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // put your setup code here, to run once:

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors: Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}


/*
1. Keep going straight. 
2. If obstacle found, then look left and right.
3. If left has more space than right, turn left and keep going straight
4. If right has more space than left, turn right and keep going straight
*/

void loop() {

  // Check if one minute has passed
  if (millis() - startTime >= 60000) {
    stopMotors();
    while(true) {
      // do nothing
    }
  }


  // keep scanning front
  long forwardDist = scanDirection(90);

  if (forwardDist > 40) {
    goStraight();
  } else {
    stopMotors(); // we stop the motors so that we can decide on which direction to go to.
    
    goBack();
    delay(500);
    stopMotors();
    
    // there is an obstacle in the front. So look left and right
    long leftDist = scanDirection(170); // Move servo to the left (150 degrees) and measure distance
    delay(500);

    long rightDist = scanDirection(10); // Move servo to the right (30 degrees) and measure distance
    delay(500);

    // turn left or right based on which has more distance.
    if (leftDist > rightDist) {
      turnLeft();
    } else {
      turnRight();
    }
  }
}


long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2; // convert time to distance in CM
  return distance;
}

long scanDirection(int angle) {
  scanServo.write(angle); // Move the servo to the specified angle
  delay(500); // wait for the servo to reach that angle
  return getDistanceCM(); // Return the distance measured by the ultrasonic sensor
}

void stopMotors() {
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  // Turn off motor A and B
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void goBack() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A and B
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void goStraight() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A and B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// to go left, the left wheels need to stop
void turnLeft() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A and B
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  delay(500);  // Let the robot turn for 500ms

  // Optionally, resume moving straight:
  goStraight();
}

void turnRight() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  // Turn on motor A and B
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  delay(500);  // Let the robot turn for 500ms

  // Optionally, resume moving straight:
  goStraight();
}







