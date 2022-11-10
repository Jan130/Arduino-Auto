#include <Servo.h>

// Motor pins
int rb_front = 2;
int rb_back = 3;
int rf_back = 5;
int rf_front = 4;
int lb_front = 6;
int lb_back = 7;
int lf_front = 9;
int lf_back = 8;

// Ultraschall-Messung
int echo_pin = 22;
int trigger_pin = 23;
long distance;

int echo_pin_l = 30;
int trigger_pin_l = 31;
long distance_l;

int echo_pin_r = 32;
int trigger_pin_r = 33;
long distance_r;

// Servo
int servo_pin = 24;
Servo servo;
const int middle_angle = 67;
const int left_angle = 20;
const int left_left_angle = 0;
const int right_angle = 114;
const int right_right_angle = 145;

// Speeds
int drive_speed = 255;
int turn_speed = 255;

// Times
int turn_duration = 400;
int short_turn_duration = 200;

void setup() {
  pinMode(rb_front, OUTPUT);
  pinMode(rb_back, OUTPUT);
  pinMode(rf_back, OUTPUT);
  pinMode(rf_front, OUTPUT);
  pinMode(lb_back, OUTPUT);
  pinMode(lb_front, INPUT);
  pinMode(lf_front, OUTPUT);
  pinMode(lf_back, OUTPUT);

  pinMode(echo_pin, INPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin_l, INPUT);
  pinMode(trigger_pin_l, OUTPUT);
  pinMode(echo_pin_r, INPUT);
  pinMode(trigger_pin_r, OUTPUT);

  servo.attach(servo_pin);
  servo.write(middle_angle);

  Serial1.begin(9600);
}

void loop() {
  servo.write(middle_angle);
  unsigned long driveStart = millis();

  // Vorwärts fahren, solange nichts im Weg ist
  while (measureDistance() > 20) {
    driveFront(drive_speed);

    // Seitlich auf Wände kontrollieren
    /*if (measureDistance_l() < 10) {
      turnRight(turn_speed);
      delay(short_turn_duration);
    } else*/ if (measureDistance_r() < 10) {
      turnLeft(turn_speed);
      delay(short_turn_duration);
    }
    
    // Nach einer zeit umschauen
    if (millis() - driveStart > 4000) {
      stop();

      servo.write(left_left_angle);
      delay(500);
      long distance_left_left = measureDistance();
      servo.write(right_right_angle);
      delay(500);
      long distance_right_right = measureDistance();

      servo.write(middle_angle);

      if (distance_left_left < 20) {
        turnRight(turn_speed);
      } else if (distance_right_right < 20) {
        turnLeft(turn_speed);
      }
      delay(turn_duration);
      driveStart = millis();
    }
  }

  // Wenn etwas im Weg ist...
  stop();

  servo.write(left_left_angle);
  delay(500);
  long distance_left_left = measureDistance();
  //delay(500);
  servo.write(left_angle);
  delay(500);
  long distance_left = measureDistance();
  //delay(500);
  servo.write(right_angle);
  delay(500);
  long distance_right = measureDistance();
  //delay(500);
  servo.write(right_right_angle);
  delay(500);
  long distance_right_right = measureDistance();
  //delay(500);

  servo.write(middle_angle);

  if ((distance_right + distance_right_right) / 2 > (distance_left + distance_left_left) / 2) {
    turnRight(turn_speed);
  } else {
    turnLeft(turn_speed);
  }
  delay(turn_duration);
}

void driveFront(int speed) {
  stop();
  if (speed > 0 || speed <= 255) {
    analogWrite(rf_front, speed);
    analogWrite(rb_front, speed);
    analogWrite(lf_front, speed);
    analogWrite(lb_front, speed);
  }
}

void driveBack(int speed) {
  stop();
  if (speed > 0 || speed <= 255) {
    analogWrite(rf_back, speed);
    analogWrite(rb_back, speed);
    analogWrite(lf_back, speed);
    analogWrite(lb_back, speed);
  }
}

void stop() {
  digitalWrite(rf_back, LOW);
  digitalWrite(rb_back, LOW);
  digitalWrite(lf_back, LOW);
  digitalWrite(lb_back, LOW);
  digitalWrite(rf_front, LOW);
  digitalWrite(rb_front, LOW);
  digitalWrite(lf_front, LOW);
  digitalWrite(lb_front, LOW);
}

void turnLeft(int speed) {
  stop();
  analogWrite(rb_front, speed);
  analogWrite(rf_front, speed);
  analogWrite(lb_back, speed);
  analogWrite(lf_back, speed);
}

void turnRight(int speed) {
  stop();
  analogWrite(rb_back, speed);
  analogWrite(rf_back, speed);
  analogWrite(lb_front, speed);
  analogWrite(lf_front, speed);
}

long measureDistance() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);

  long t = 0;
  t = pulseIn(echo_pin, HIGH);
  distance = (t / 2) * 0.03432;

  Serial1.print("Vorne: ");
  Serial1.println(distance);
  return distance;
}

long measureDistance_l() {
  digitalWrite(trigger_pin_l, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_l, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_l, LOW);

  long t = 0;
  t = pulseIn(echo_pin_l, HIGH);
  distance = (t / 2) * 0.03432;

  Serial1.print("Links: ");
  Serial1.println(distance);
  return distance;
}

long measureDistance_r() {
  digitalWrite(trigger_pin_r, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin_r, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin_r, LOW);

  long t = 0;
  t = pulseIn(echo_pin_r, HIGH);
  distance = (t / 2) * 0.03432;

  Serial1.print("Rechts: ");
  Serial1.println(distance);
  return distance;
}
