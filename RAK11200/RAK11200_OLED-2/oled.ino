void initOLED(void)
{
  display.begin();
  display.setRotation(3);
  canvas.setColorDepth(1); // mono color
  canvas.setFont(&fonts::FreeMono9pt7b);
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize(1);
  canvas.setTextScroll(true);
}