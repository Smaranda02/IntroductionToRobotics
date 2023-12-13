void displayAlphabet() {
  if (!shown) {
    lcd.clear();
    shown = true;
  } 

    lcd.setCursor(0, 0);
    lcd.print(alphabet1);
    lcd.setCursor(0, 1);
    lcd.print(alphabet2);
    // lcd.setCursor(11, 1);
    // lcd.print("<");
    // lcd.setCursor(13, 1);
    // lcd.print("Y");
    // lcd.setCursor(14, 1);
    // lcd.print("/");
    // lcd.setCursor(15, 1);
    // lcd.print("N");

}


void setBrightness(byte value) {
  value = map(value, minBrightness, maxBrightness, minMappedValue, maxMappedValue);
  analogWrite(backLightPin, value);
  //EEPROM.update(10, value);
}

void readBrightnessValue() {
  if (shown == false) {
    shown = true;
  }
  if (Serial.available() > 0) {
    byte value = Serial.parseInt();
    setBrightness(value);
  }
}