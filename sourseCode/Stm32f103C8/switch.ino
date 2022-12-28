void switch1() {
  if (digitalRead(CBHT) == 0) {
    package = 1;
  } else {
    package = 0;
  }

  if (digitalRead(RE_LO) == 0) {
    remote_local = 0;
  } else {
    remote_local = 1;
  }

  if ((digitalRead(AU_MA) == 0 && digitalRead(RE_LO) == 0) || (digitalRead(AUTO_MAN) == 0 && digitalRead(RE_LO) == 1)) {
    auto_man = 1;
  } else {
    auto_man = 0;
  }

  if (digitalRead(START_STOP) == 0) {
    start_stop = 1;
  } else {
    start_stop = 0;
  }

  if ((digitalRead(DES_1) == 0 && digitalRead(DES_2) == 0)) {
    selection_des = 3;
  } else if ((digitalRead(DES_1) == 0 && digitalRead(DES_2) == 1)) {
    selection_des = 2;
  } else {
    selection_des = 1;
  }

  if (digitalRead(OBSTACLE1) == 0) {
    obstacle = 1;
  } else {
    obstacle = 0;
  }
}
