
float mapfloat(float x, float inMin, float inMax, float outMin, float outMax)
{
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}
void base1()
{
  w1 = mapfloat(((1 / r) * (Vx - Vy - (lx + ly) * Wz)), -200, 200, -40, 40);
  w2 = mapfloat(((1 / r) * (Vx + Vy + (lx + ly) * Wz)), -200, 200, -40, 40);
  w3 = mapfloat(((1 / r) * (Vx + Vy - (lx + ly) * Wz)), -200, 200, -40, 40);
  w4 = mapfloat(((1 / r) * (Vx - Vy + (lx + ly) * Wz)), -200, 200, -40, 40);
}