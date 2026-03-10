#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3Y41Fc77i"
#define BLYNK_TEMPLATE_NAME "Ultrasonic Distance Detector"
#define BLYNK_AUTH_TOKEN "VpkKV0HmbsX5vTtsWm2zBejbnljPBO4g"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define TRIG_PIN D5
#define ECHO_PIN D6
#define LED1_PIN D0
#define LED2_PIN D1
#define LED3_PIN D2
#define LED4_PIN D3

char ssid[] = "Q1"; // Replace with your WiFi SSID
char pass[] = "Hotspot1"; // Replace with your WiFi password

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
}

void loop() {
  long duration, distance;

  // Trigger the ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echo
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Distance in cm

  // Control LED intensity based on distance
  if (distance < 2) {
    // If very close, turn off LEDs
    analogWrite(LED1_PIN, 0);
    analogWrite(LED2_PIN, 0);
    analogWrite(LED3_PIN, 0);
    analogWrite(LED4_PIN, 0);
  } else {
    // Map distance to PWM value (inverse relation)
    int pwmValue = map(distance, 2, 400, 0, 255); // Adjust max distance as needed
    pwmValue = constrain(pwmValue, 0, 255); // Ensure value is within range
    
    // Set all LEDs to the calculated intensity
    analogWrite(LED1_PIN, pwmValue);
    analogWrite(LED2_PIN, pwmValue);
    analogWrite(LED3_PIN, pwmValue);
    analogWrite(LED4_PIN, pwmValue);
    
    // Send distance to Blynk as a value from 0 to 100
    int displayValue = map(distance, 2, 400, 0, 100); // Scale distance to 0-100
    displayValue = constrain(displayValue, 0, 100); // Ensure value is within range
    Blynk.virtualWrite(V0, displayValue); // Send to virtual pin V0
  }

  Blynk.run(); // Run Blynk
  delay(100); // Adjust the delay as needed
}
