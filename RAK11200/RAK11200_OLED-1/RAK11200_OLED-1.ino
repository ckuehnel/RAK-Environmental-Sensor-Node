#include <M5UnitOLED.h> // https://github.com/m5stack/M5GFX

M5UnitOLED display;
M5Canvas canvas(&display);

String text[] = { "Zeile1", "Zeile2", "Zeile3", "Zeile4"};


void setup() 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  display.begin();
  display.setRotation(3);
  canvas.setColorDepth(1); // mono color
  canvas.setFont(&fonts::FreeMono12pt7b);
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize(1);
  canvas.setTextScroll(true);

}

void loop() 
{
  static uint8_t i = 0;

  canvas.printf("%s\r\n", text[i % 4]);
  canvas.pushSprite(0, 0);
  ++i;
  delay(1000);

}
