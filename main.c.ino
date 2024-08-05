#include <HardwareSerial.h>
#include <LiquidCrystal_I2C.h>

#define Sober 600   // Define max value that we consider sober
#define Drunk 1000   // Define min value that we consider drunk

#define MQ3pin 14   // Analog pin for MQ3 on ESP32
HardwareSerial gsmSerial(2); 
float sensorValue;

int buzzerPin = 23;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motor A
int motorPin1 = 26;  // Output 1 of L298N
int motorPin2 = 27;  // Output 2 of L298N
int enablePinA = 4;  // PWM Pin for speed control

// Motor B
int motorPin3 = 15;  // Output 3 of L298N
int motorPin4 = 19;  // Output 4 of L298N
int enablePinB = 5;  // PWM Pin for speed cont

void setup() {


  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(enablePinB, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize LCD
  lcd.init();
  lcd.backlight();  // Turn on the backlight
  lcd.clear();

  // Start Serial for debugging
  //Serial.begin(115200);

  // Start GSM communication
  gsmSerial.begin(115200);
gsmSerial.begin(115200, SERIAL_8N1, 16, 17);
   gsmSerial.println("AT");
}

void loop() {
  sensorValue = analogRead(MQ3pin);
  //Serial.print("Sensor Value: "); 
  //Serial.println(sensorValue);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sensor Value:");
  lcd.setCursor(0, 1);
  lcd.print(sensorValue);

  if (sensorValue < Sober) {
    moveForward(255);
  } 
  else if (sensorValue >= Sober && sensorValue < Drunk) 
  {
    moveForward(127);
    digitalWrite(buzzerPin, HIGH);
   
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  
  gsmSerial.println("AT+CMGS=\"9353792842\""); // Replace with the recipient's phone number
  delay(1000);
  
  gsmSerial.println("alert"); // Your message
  delay(1000);
  
  gsmSerial.println((char)26); // Ctrl-Z to indicate the end of the message
  delay(1000);

  
  delay(5000);
  } 
  else 
  {
    digitalWrite(buzzerPin, HIGH);
    moveForward(50);
    stop();
   
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  
  gsmSerial.println("AT+CMGS=\"9353792842\""); // Replace with the recipient's phone number
  delay(1000);
  
  gsmSerial.println("Drunk"); // Your message
  delay(1000);
  
  gsmSerial.println((char)26); // Ctrl-Z to indicate the end of the message
  delay(1000);

  
  delay(5000);
  }

  delay(1000); 
}

void moveForward(int speed) {
  analogWrite(enablePinA, speed);
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePinB, speed);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

void stop() {
  analogWrite(enablePinA, 0);
  analogWrite(enablePinB, 0);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}
