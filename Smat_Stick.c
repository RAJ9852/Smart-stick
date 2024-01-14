#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set up LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Replace with the address of your LCD

// Set up ADXL335 pins
int xpin = 0;
int ypin = 1;
int zpin = 2;
int BUZZER=9;
int irSensorPin=2;

// Set up fall detection thresholds (adjust these as needed)
const float FALL_THRESHOLD = 2.0;
const float MIN_THRESHOLD = 0.5;

void setup() {
  Serial.begin(9600);
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize I2C communication
  Wire.begin();

  // Initialize ADXL335 pins as inputs
  pinMode(xpin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(zpin, INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(irSensorPin,INPUT);
}

void loop() {
  // Read analog values from ADXL335 pins
  int x_val = analogRead(xpin);
  int y_val = analogRead(ypin);
  int z_val = analogRead(zpin);
  int irSensorPin = digitalRead(irSensorPin);

  // Convert analog values to acceleration in g's
  float x_acc = (x_val - 512) / 102.4;
  float y_acc = (y_val - 512) / 102.4;
  float z_acc = (z_val - 512) / 102.4;

  // Check if a fall has been detected
  bool fall_detected = false;
  if (x_acc < -FALL_THRESHOLD && y_acc < -FALL_THRESHOLD && z_acc < -FALL_THRESHOLD) {
    // Fall detected
    fall_detected = true;
  }
  else if (x_acc > -MIN_THRESHOLD && x_acc < MIN_THRESHOLD &&
           y_acc > -MIN_THRESHOLD && y_acc < MIN_THRESHOLD &&
           z_acc > -MIN_THRESHOLD && z_acc < MIN_THRESHOLD) {
    // No motion detected (sensor is stationary)
  }
  else {
    // Motion detected, but not a fall
  }

  // Display fall detection status on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  if (fall_detected) {
    lcd.print("NO FALL DETECTED!");
      digitalWrite(BUZZER,LOW);
      delay(500);
    }
  else {
    lcd.print("fall detected");
    digitalWrite(BUZZER,HIGH);
  }
  delay(500); // Wait for 500ms
}
