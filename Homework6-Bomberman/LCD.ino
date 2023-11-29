void displayAlphabet() {
  lcd.print("ABCDEFGHIJKLMNOP");
  lcd.setCursor(0, 1);
  lcd.print("QRSTUVWXYZ");
}

bool shown = false;

void setBrightness(byte value) {
  value = map(value, minBrightness, maxBrightness, minMappedValue, maxMappedValue);
  analogWrite(backLightPin, value);
  EEPROM.update(10, backLightValue);
}

void readBrightnessValue(){
    if(shown==false){
      Serial.println("Enter");
    shown=true;
    }
    if(Serial.available() > 0){
      byte value = Serial.parseInt();
      setBrightness(value);
    }
}