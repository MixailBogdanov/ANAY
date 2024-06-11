void Start_cycle() { //функция запуска
  //включение индикации:
  //кратковременная подача звукового сигнала
  digitalWrite(beep, 1);
  delay(100);
  digitalWrite(beep, 0);
  //включение красной лампы указывающей на включение силовых элементов
  digitalWrite(ledRed, 1);
  delay(500);
  digitalWrite(ledRed, 0);
  delay(500);
  digitalWrite(ledRed, 1);
  //включение привода вырубной платформы:
  analogWrite(pinSpeed, value_speed / coefSpeed); //отправка значения скорости на частотник двигателя вырубной платформы
  digitalWrite(enableR, HIGH);//разрешение на включение двигателя
  digitalWrite(forwardR, HIGH);//включение вращения вперед
  digitalWrite(reverseR, LOW);
  delay(delaybeforegrip);//задержка перед вкл муфты сцепления
  digitalWrite(grip, HIGH);//включение муфты сцепления
}
