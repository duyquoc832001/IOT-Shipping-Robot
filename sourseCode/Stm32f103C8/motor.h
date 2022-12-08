#include "definePin.h"

void M1(int out)
{
  if (out > 0.0) {
    analogWrite(M1_A, out);
    analogWrite(M1_B, 0);
  } else if (out < 0.0) {
    analogWrite(M1_A, 0);
    analogWrite(M1_B, abs(out));
  } else {
    analogWrite(M1_A, 0);
    analogWrite(M1_B, 0);
  }
}

void M2(int out)
{
  if (out > 0.0) {
    analogWrite(M2_A, out);
    analogWrite(M2_B, 0);
  } else if (out < 0.0) {
    analogWrite(M2_A, 0);
    analogWrite(M2_B, abs(out));
  } else {
    analogWrite(M2_A, 0);
    analogWrite(M2_B, 0);
  }
}

void M3(int out)
{
  if (out > 0.0) {
    analogWrite(M3_A, out);
    analogWrite(M3_B, 0);
  } else if (out < 0.0) {
    analogWrite(M3_A, 0);
    analogWrite(M3_B, abs(out));
  } else {
    analogWrite(M3_A, 0);
    analogWrite(M3_B, 0);
  }
}

void M4(int out)
{
  if (out > 0.0) {
    analogWrite(M4_A, out);
    analogWrite(M4_B, 0);
  } else if (out < 0.0) {
    analogWrite(M4_A, 0);
    analogWrite(M4_B, abs(out));
  } else {
    analogWrite(M4_A, 0);
    analogWrite(M4_B, 0);
  }
}
