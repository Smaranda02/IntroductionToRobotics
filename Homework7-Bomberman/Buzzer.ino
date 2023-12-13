
void playMelody(Sound melody[], int length) {
  for (int i = 0; i < length; i++) {
    tone(buzzerPin, melody[i].pitch, melody[i].duration);
    delay(melody[i].duration + 50);
  }
  noTone(buzzerPin);  
}