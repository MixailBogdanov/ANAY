
int petlya_cm() //метод возвращающий расстояние до ультразвукового датчика
{
  long duration, cm;
  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  
  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(PIN_ECHO, HIGH);
  // Преобразуем время в расстояние
  cm = (duration / 2) / 29.1;
  // Возвращаем расстояние
  return cm;
   
}
//Метод выполнения основного цикла программы 
void Main_cycle()
{
 //Автоматическое управление двигателем размотки 
 digitalWrite(PIN_MOTOR, HIGH); //запускаем двигатель размотки 
 regulator.input = petlya_cm(); // сообщаем регулятору текущее расстояние
 Serial.print("показания датчика:");
 Serial.println(petlya_cm());
 Serial.print("ПИД:");
 Serial.println(regulator.getResultTimer());
 
 analogWrite(PIN_SPEED, regulator.getResultTimer());  // управляем скоростью на частотнике по ПИД-регулятору 
//...............

//Автоматический подъем рулона
if(millis()-timer_millis>=10000) //раз в 10 секунд опрашиваем датчик рулона
{
  timer_millis=millis();
  //Если рулон уменьшился и ушел за пределы видимости датчика - приподнимаем его 
  if(digitalRead(PIN_ROLL_SENSOR))
  {
    digitalWrite(PIN_ROLL_UP, HIGH); //поднимаем рулон
  }
}
if(!digitalRead(PIN_ROLL_SENSOR))//если рулон достиг оптического датчика - останавливаем подъем рулона
{
  digitalWrite(PIN_ROLL_UP, LOW); //прекращаем подъем рулона
}
//Конец программы работы в автоматическом режиме........................
 
}
