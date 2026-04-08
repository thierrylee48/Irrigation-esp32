#include <ESP32Servo.h>

// Define pins
const int trigPin = 27;   // Ultrasonic trig pin (D27). Send sound waves
const int echoPin = 34;   // Ultrasonic Echo pin (D34). Receives reflected wave
const int servoPin = 25;  // Servo control pin

// Variables
long duration; // Time for echo to return
float distance; // Calculate distance in cm

Servo gateServo; // Create servo object

// Change angle of servo when open and close
int openAngle = 20;
int closedAngle = 200;

// See if gate opened or closed
bool isGateClosed = false;

// Trig and echo pin output and read signal
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // To Start serial monitor
  Serial.begin(9600);

  // Servo signal
  gateServo.setPeriodHertz(50);
  gateServo.attach(servoPin, 1000, 2000);
  delay(500);
  gateServo.write(openAngle);
}

void loop() {
  
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Receive echo
  duration = pulseIn(echoPin, HIGH, 30000);

  // When no echo detected
  if (duration == 0) {
    Serial.println("No echo");
    delay(300);
    return; // Skip rest of loop
  }

  // Speed of sound = 0.034
  // Calculate to cm
  distance = duration * 0.034 / 2.0;

  // Print distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If water 7cm away from ultrasonic sensor, it will close
  if (distance <= 7 && !isGateClosed) {
    gateServo.write(closedAngle);
    isGateClosed = true;
    Serial.println("Object close -> Gate CLOSED");
    delay(1200);
  } 

  // If water farther than 7cm to ultrasonic sensor, It will open
  else if (distance > 7 && isGateClosed) {
    gateServo.write(openAngle);
    isGateClosed = false;
    Serial.println("Object far -> Gate OPEN");
    delay(1200);
  }

  // Delay before reading next 
  delay(300);
}