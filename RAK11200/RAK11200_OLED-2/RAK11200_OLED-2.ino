#include <M5UnitOLED.h> // https://github.com/m5stack/M5GFX

M5UnitOLED display;
M5Canvas canvas(&display);

String text[] = { "T 22.2 *C", "H 66 %rH", "CO2 440 ppm", "iVOC 100"};


void setup() 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  initOLED();

}

void loop() 
{
  static uint8_t i = 0;

  if (millis() % 2000 == 1000)  // simple one-second timer
  {
    canvas.printf("%s\r\n", text[i % 4]);
    canvas.pushSprite(0, 0);
    ++i;
  }
}
