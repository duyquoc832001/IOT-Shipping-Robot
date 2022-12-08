
// pulse and direction, direct port reading to save cycles
void encoder1()
{
  if (digitalRead(encoder1PinB) == HIGH)   encoderPos1++;
  if (digitalRead(encoder1PinB) == LOW)   encoderPos1--;
  positionCurrent = float(((2 * pi * R ) / N) * float(encoderPos1) * n);
}
// pulse and direction, direct port reading to save cycles
void encoder2()  {
  if (digitalRead(encoder2PinB) == HIGH)   encoderPos2++;
  if (digitalRead(encoder2PinB) == LOW)   encoderPos2--;
}
// pulse and direction, direct port reading to save cycles
void encoder3()  {

  if (digitalRead(encoder3PinB) == HIGH)   encoderPos3++;
  if (digitalRead(encoder3PinB) == LOW)   encoderPos3--;
}
void encoder4()  {
  if (digitalRead(encoder4PinB) == HIGH)   encoderPos4++;
  if (digitalRead(encoder4PinB) == LOW)   encoderPos4--;
}
