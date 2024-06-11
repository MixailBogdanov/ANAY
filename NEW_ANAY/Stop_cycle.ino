void Stop_cycle(){ //функция остановки
  //кратковременно подаем звуковой сигнал указывающий на остановку станка
  digitalWrite(beep, 1);
  delay(100);
  digitalWrite(beep, 0);
  
  analogWrite(pinSpeed, brakingSpeed / coefSpeed); //передача на частотник скорости в режиме остановки
  delay(2500); //задержка перед началом остановки, чтобы платформа успела замедлиться
  
  while(1){ //цикл остановки:  
    //проверяем состояние нижнего датчика 
    if(!digitalRead(sensorLow)){  //Если плита дошла до нижнего датчика...
      analogWrite(pinSpeed, 0); //сбрасываем скорость до нуля           
      digitalWrite(enableR, LOW); //останавливаем двигатель
      digitalWrite(forwardR, LOW); //сбрасываем направление вращения
      delay(1000); //ждем пока платформа полностью остановится 
      digitalWrite(grip, LOW); //выключаем сцепление 
      digitalWrite(ledRed, 0); //выключаем индикацию 
      break; //выходим из цикла остановки
    }
  }

    
}
