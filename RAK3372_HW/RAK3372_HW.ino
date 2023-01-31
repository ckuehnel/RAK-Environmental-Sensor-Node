/*
 * File: RAK3372_HW.ino
 * 
 * Test of Core RAK3372 installed on Base RAK19001
 * Connect IO1 and SW1 on RAP19001
 * 
 * 2023-01-25 Claus Kuehnel info@ckuehnel.ch
 */
// set pin number for WISBLOCK_BASE_19001
uint8_t ledGreenPin = GREEN_LED;  // set ledpin for Green Led
uint8_t ledBluePin  = BLUE_LED;	  // set ledpin for Blue Led
uint8_t inputPin    = WB_IO1;	    // set inputPin to IO1  CK: external connection between IO1 and SW1, SW1 not available on RAK3372

void setup()
{
  // initialize serial communication at 115200 bits per second
  Serial.begin(115200);

  Serial.println("RAKwireless Arduino Hello World");
  Serial.println("-------------------------------");
  // initialize the LED pins as an output  
  pinMode(ledBluePin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  // initialize digital pin "inputPin" as an input with the internal pull-up resistor enabled
  pinMode(inputPin, INPUT_PULLUP);
}

void loop()
{
  Serial.println("This is RAK3372 based on STM32WLE5CC");

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
