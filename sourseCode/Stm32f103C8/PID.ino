
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

  // if (temp1 <= w1) { temp1 = temp1 + 1; }
  // if (temp2 <= w2) { temp2 = temp2 + 1; }
  // if (temp3 <= w3) { temp3 = temp3 + 1; }
  // if (temp4 <= w4) { temp4 = temp4 + 1; }

  // if (temp1 < 0) {
  //   encoderPos1 = 0;
  //   temp1 = 0;
  // }
  // if (temp2 < 0) {
  //   encoderPos2 = 0;
  //   temp2 = 0;
  // }
  // if (temp3 < 0) {
  //   encoderPos3 = 0;
  //   temp3 = 0;
  // }
  // if (temp4 < 0) {
  //   encoderPos4 = 0;
  //   temp4 = 0;
  // }

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
