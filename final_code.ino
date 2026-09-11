#include <NewPing.h>
#include <Servo.h>
#define VCC_PIN 13
#define TRIGGER_PIN 12
#define ECHO_PIN 11
#define GROUND_PIN 10
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

#define SERVO 9
Servo myservo;

#define SERVO_OPEN 100
#define SERVO_CLOSE 0
#define SERVO_THRESHOLD 10
#define RESET_THRESHOLD 20
#define SERVO_SPEED 30
#define INTERVAL 1500


#define SONAR_INTERVAL 50

int isOpen = 1;
int isReady = 1;

void setup() {
  Serial.begin(9600);
  pinMode(ECHO_PIN, INPUT);  
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(GROUND_PIN, OUTPUT);
  pinMode(VCC_PIN, OUTPUT);
  digitalWrite(GROUND_PIN,LOW);
  digitalWrite(VCC_PIN, HIGH);

  myservo.attach(SERVO);
  myservo.write(SERVO_OPEN);

  Serial.println("System initialized");
}

void loop() {
  delay(SONAR_INTERVAL);

  int distance = readDistance();

  if(isOpen == 1 && isReady == 1) {
    if(distance > 0 && distance < SERVO_THRESHOLD) {
      Serial.println("Claw closing slowly");
      slowClose();
      isOpen = 0;
      isReady = 0;
    }
  }

  if(isOpen == 0 && isReady == 0) {
    if(distance > RESET_THRESHOLD) {
      isReady = 1;
      Serial.println("Ready to drop");
    }
  }

  if(isOpen == 0 && isReady == 1) {
    if(distance > 0 && distance < SERVO_THRESHOLD) {
      myservo.write(SERVO_OPEN);
      isOpen = 1;
      isReady = 0;
      Serial.println("Object dropped");
    }
  }

  if(isOpen == 1 && isReady == 0) {
    if(distance > RESET_THRESHOLD) {
      isReady = 1;
      Serial.println("Reset complete. Ready to grab");
    }
  }
}


int readDistance() {
  int DISTANCE_IN_CM = sonar.ping_cm();
  // Serial.print("Ping: ");
  // Serial.print(DISTANCE_IN_CM);
  // Serial.println("cm"); 

  return DISTANCE_IN_CM;

}


int slowClose () {
  for(int pos=SERVO_OPEN; pos >= SERVO_CLOSE; pos--) {
    myservo.write(pos);
    delay(SERVO_SPEED);
  }
}