void System() {

  if (!remote_local || (remote_local && start_stop)) {
    if (auto_man) {
      if (!obstacle) {
        Sys_auto();
      } else {
        Vx = 0;
        Vy = 0;
        Wz = 0;
      }
    } else {
      if (!obstacle) {
        Sys_man();
      } else {
        Vx = 0;
        Vy = 0;
        Wz = 0;
      }
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 0;
    }
  } else {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 0;
  }
}

void Sys_man() {
  Vx = speed_setpoint;
  Vy = 0;
  Wz = -z * 1.6;
}

// Chế độ auto
void Sys_auto() {
  // Quy trình chạy tự động của xe

  // Bước 0: Xe chạy thẳng 320cm tuyến đường 1
  if (step == 0 && (positionCurrent - newPosition < 320) && !package) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 1;
    s2 = 0;
  }
  if (step == 0 && (positionCurrent - newPosition < 320) && package) {
    Wz = -z * 1.6;
    Vx = speed_setpoint;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 0;
    s2 = 0;
  }
  if (step == 0 && (positionCurrent - newPosition >= 320)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 1;
  }

  // Bước 1: Xe dừng trong vòng 2 giây
  if (step == 1 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 1 && (timeCurrent - time >= restTime)) {
    step = 2;
    newPosition = positionCurrent;
  }

  // Bước 2: Xe dừng trong vòng 2 giây và quay trái 1 góc 90 deg
  if (step == 2 && (timeCurrent - time >= restTime)) {
    if (z >= 90) {
      Wz = 0;
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 3;
    } else {
      Wz = rotate_setpoint;
    }
  }

  // Bước 3: Xe dừng trong vòng 2 giây và đi thẳng
  if (step == 3 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 90) * 1.6;
    Vx = speed_setpoint;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 4;
  }

  // Bước 4: Xe đi thẳng 90cm tuyến đường 2
  if (step == 4 && (positionCurrent - newPosition < 90)) {
    Wz = -(z - 90) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 4 && (positionCurrent - newPosition >= 90)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 5;
  }

  // Bước 5: Xe dừng trong vòng 2 giây và xoay trái 90 deg
  if (step == 5 && (timeCurrent - time >= restTime)) {
    if (z >= 180) {
      Wz = 0;
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 6;
    } else {
      Wz = rotate_setpoint;
    }
  }

  // Bước 6: Xe dừng trong vòng 2 giây và đi thẳng
  if (step == 6 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 7;
  }

  // =========================================================
  // Tới destination

  // Bước 7: Xe đi tới destination
  if (step == 7 && (positionCurrent - newPosition < position_des)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 7 && (positionCurrent - newPosition >= position_des)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    switch (selection_des) {
      // đến destination A
      case 1:
        {
          des_a = 1;
          des_b = 0;
          des_c = 0;
          break;
        }
      // đến destination B
      case 2:
        {
          des_a = 0;
          des_b = 1;
          des_c = 0;
          break;
        }
      // đến destination C
      case 3:
        {
          des_a = 0;
          des_b = 0;
          des_c = 1;
          break;
        }
      default:
        {
          des_a = 1;
          des_b = 0;
          des_c = 0;
          break;
        }
    }
    s1 = 0;
    s2 = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 8;
  }
  // Bước 8: Xe dừng tại destination, xoay 360 độ để đi về lại tuyến đường 2 và gửi dữ liệu lên app
  // *Lưu ý: Chương trình cập nhập dữ liệu trên app !!!!

  if (step == 8 && (timeCurrent - time >= restTime)) {
    if (z >= 360) {
      Wz = 0;
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 9;
    } else {
      Wz = rotate_setpoint;
    }
  }
  // Bước 9: Xe đi thẳng về lại tuyến đường 2
  if (step == 9 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 9 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 360) * 1.6;
    Vx = speed_setpoint;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 0;
    s2 = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 10;
  }

  // Bước 10: Xe hoàn thành tuyến đường 3
  if (step == 10 && (positionCurrent - newPosition < position_completion)) {
    Wz = -(z - 360) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 10 && (positionCurrent - newPosition >= position_completion)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 11;
  }
  // =========================================================

  // Bước 11: Xe dừng trong vòng 2 giây và xoay phải 90 deg
  if (step == 11 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 11 && (timeCurrent - time >= restTime)) {
    if (z <= 270) {
      Wz = 0;
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 12;
    } else {
      Wz = -rotate_setpoint;
    }
  }

  // Bước 12: Xe dừng trong vòng 2 giây và đi thẳng
  if (step == 12 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 12 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 270) * 1.6;
    Vx = speed_setpoint;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 13;
  }

  // Bước 13: Xe đi thẳng 15cm tuyến đường 4
  if (step == 13 && (positionCurrent - newPosition < 80)) {
    Wz = -(z - 270) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 13 && (positionCurrent - newPosition >= 80)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 14;
  }

  // Bước 14: Xe dừng trong vòng 2 giây và xoay phải 90 deg

  if (step == 14 && (timeCurrent - time >= restTime)) {
    if (z <= 180) {
      Wz = 0;
      newPosition = positionCurrent;
      time = timeCurrent;
      step = 15;
    } else {
      Wz = -rotate_setpoint;
    }
  }

  // Bước 15: Xe dừng trong vòng 2 giây và đi thẳng
  if (step == 15 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 15 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 16;
  }

  // Bước 16: Xe đi thẳng 15cm tới station 2
  if (step == 16 && (positionCurrent - newPosition < 245)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 16 && (positionCurrent - newPosition >= 245)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 0;
    s2 = 1;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 17;
  }

  // Bước 17: Lấy hàng ra khỏi xe
  if (step == 17 && package) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 17 && !package) {
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 0;
    s2 = 1;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 18;
  }

  // Bước 18: Xe dừng trong vòng 2 giây và đi thẳng về station 1

  if (step == 18 && (timeCurrent - time < restTime)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
  if (step == 18 && (timeCurrent - time >= restTime)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 0;
    s2 = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = 19;
  }

  // Bước 19: Xe đi thẳng 40cm tới lại station 1
  if (step == 19 && (positionCurrent - newPosition < 35)) {
    Wz = -(z - 180) * 1.6;
    Vx = speed_setpoint;
  }
  if (step == 19 && (positionCurrent - newPosition >= 35)) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
    des_a = 0;
    des_b = 0;
    des_c = 0;
    s1 = 1;
    s2 = 0;
    newPosition = positionCurrent;
    time = timeCurrent;
    step = -1;
  }

  if (step >= 0 && step <= 16 && !package) {
    Vx = 0;
    Vy = 0;
    Wz = 0;
  }
}

void Selection_des() {
  switch (selection_des) {
    // Thời gian đến destination A
    case 1:
      {
        position_des = 35;
        position_completion = 35;
        break;
      }
    // Thời gian đến destination B
    case 2:
      {
        position_des = 180;
        position_completion = 180;
        break;
      }
    // Thời gian đến destination C
    case 3:
      {
        position_des = 310;
        position_completion = 310;
        break;
      }
    default:
      {
        position_des = 30;
        position_completion = 30;
        break;
      }
  }
}