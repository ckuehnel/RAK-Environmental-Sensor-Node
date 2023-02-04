/*
 * File: RAK11200_HW.ino
 * 
 * Test of Core RAK11200 installed on Base RAK19001
 *  
 * 2023-02-25 Claus Kuehnel info@ckuehnel.ch
 */

// set pin number for WISBLOCK_BASE_19001
uint8_t ledGreenPin = LED_GREEN;  // set ledpin for Green Led
uint8_t ledBluePin  = LED_BLUE;	  // set ledpin for Blue Led
uint8_t inputPin    = WB_SW1;     // set inputPin to SW1

void setup()
{
  // initialize serial communication at 115200 bits per second
  Serial.begin(115200);
  delay(2000);
  Serial.println("RAKwireless Arduino Hello World");
  Serial.println("-------------------------------");
  Serial.println("Blue LED blinks");
  Serial.println("Green LED is controlled by SW1");
  // initialize the LED pins as an output  
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  // initialize digital pin "inputPin" as an input with the internal pull-up resistor enabled
  pinMode(inputPin, INPUT_PULLUP);
}

void loop()
{
  Serial.println("This is RAK11200 based on ESP32");

  int val = digitalRead(inputPin);	// read input pin value (HIGH or LOW)
 
  if (val == LOW)
    digitalWrite(ledGreenPin, HIGH);	// LED turn on when input pin value is LOW
  else
    digitalWrite(ledGreenPin, LOW);	// LED turn off when input pin value is HIGH 

  digitalWrite(ledBluePin, HIGH);     // turn the Blue LED on (HIGH is the voltage level)
  delay(50);                          // wait short
  digitalWrite(ledBluePin, LOW);      // turn the Blue LED off by making the voltage LOW
  delay(950);                         // wait a second approx.
}
