
void PID() {
  if (timeCurrent - time_counter >= 100) {
    fb1 = ((float(encoderPos1 - precount1) / 70) * 60);
    fb2 = ((float(encoderPos2 - precount2) / 70) * 60);
    fb3 = ((float(encoderPos3 - precount3) / 70) * 60);
    fb4 = ((float(encoderPos4 - precount4) / 70) * 60);
    precount1 = encoderPos1;
    precount2 = encoderPos2;
    precount3 = encoderPos3;
    precount4 = encoderPos4;
    time_counter = timeCurrent;
  }

  setpoint1 = w1;
  setpoint2 = w2;
  setpoint3 = w3;
  setpoint4 = w4;

  input1 = map(fb1, -150, 150, -40, 40);  // data from encoder
  input2 = map(fb2, -150, 150, -40, 40);  // data from encoder
  input3 = map(fb3, -150, 150, -40, 40);  // data from encoder
  input4 = map(fb4, -150, 150, -40, 40);  // data from encoder

  M1PID.Compute();
  M2PID.Compute();
  M3PID.Compute();
  M4PID.Compute();
  
  M1(output1);
  M2(output2);
  M3(output3);
  M4(output4);
}
