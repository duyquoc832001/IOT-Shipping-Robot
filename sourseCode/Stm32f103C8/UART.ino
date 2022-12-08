void Send_Data() {
  // Test UART truyền dữ liệu qua esp sáng đèn
  if (timeCurrent - time_UART >= 200) {
    if (stateLED) {
      Serial.println("A1B");
    } else {
      Serial.println("A0B");
    }
    stateLED = !stateLED;
    time_UART = timeCurrent;

    if (package) {
      Serial.println("package_true");
    } else {
      Serial.println("package_false");
    }

    if (obstacle) {
      Serial.println("obstacle_true");
    } else {
      Serial.println("obstacle_false");
    }

    if (remote_local) {
      Serial.println("remote_local_true");
    } else {
      Serial.println("remote_local_false");
    }

    if (box_1) {
      Serial.println("box_1_true");
    } else {
      Serial.println("box_1_false");
    }

    if (box_2) {
      Serial.println("box_2_true");
    } else {
      Serial.println("box_2_false");
    }

    if (des_a) {
      Serial.println("des_a_true");
    } else {
      Serial.println("des_a_false");
    }

    if (des_b) {
      Serial.println("des_b_true");
    } else {
      Serial.println("des_b_false");
    }

    if (des_c) {
      Serial.println("des_c_true");
    } else {
      Serial.println("des_c_false");
    }

    if (s1) {
      Serial.println("s1_true");
    } else {
      Serial.println("s1_false");
    }

    if (s2) {
      Serial.println("s2_true");
    } else {
      Serial.println("s2_false");
    }
  }
}