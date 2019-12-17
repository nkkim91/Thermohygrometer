
void SSD1306_Setup()
{
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
}
