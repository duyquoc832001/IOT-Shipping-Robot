void CBSA() {

  digitalWrite(CBSA_TRIG, 0);  //Tắt chân trig
  delayMicroseconds(2);
  digitalWrite(CBSA_TRIG, 1);  //bật chân trig để phát xung
  delayMicroseconds(10);  //Xung có độ rộng là 10 microsecond
  digitalWrite(CBSA_TRIG, 0);

  //Chân echo sẽ nhận xung phản xạ lại
  //Và đo độ rộng xung cao ở chân echo
  time_SA = pulseIn(CBSA_ECHO, HIGH);

  //Tính khoảng cách đến vật thể (Đơn vị đo là cm)
  //Tốc độ của âm thanh trong không khí là 340 m/s, tương đương 29,412 microSeconds/cm,
  //Do thời gian được tính từ lúc phát tín hiệu tới khi sóng âm phản xạ lại,
  //vì vậy phải chia cho 2
  distance_SA = int(time_SA / 2 / 29.412);

  if (distance_SA <= 30)  //Nếu khoảng cách tới vật nhỏ hơn 20cm
  {
    // Serial.println("Has obstacle");
    obstacle = 1;
  } else {
    // Serial.println("Not obstacle");
    obstacle = 0;
  }
}