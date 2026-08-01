#include <Servo.h>

const int buttonPin = 2;
const int servoPin = 7;
const int trigPin = 8;
const int echoPin = 9;
const int buzzerPin = 10;
const int ledPin = 13;

Servo feederServo;

long duration;
int distance;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  feederServo.attach(servoPin);
  feederServo.write(0); 

  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { 
    distance = getDistance();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance > 10) {
      runFeeder();
    } else {
      blockedWarning();
    }

    delay(1000); // recovery pause before it can run again
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int cm = duration * 0.034 / 2;

  return cm;
}

void runFeeder() {
  Serial.println("FeederBot starting.");

  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 1000);

  feederServo.write(90);   
  delay(3000);             

  feederServo.write(0);    
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);

  Serial.println("FeederBot cycle complete. Recovering.");
}

void blockedWarning() {
  Serial.println("Object detected. Feeding cancelled.");

  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 500);
  delay(500);
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);
}
