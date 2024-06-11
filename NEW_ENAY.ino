#include <Controllino.h> //библиотека для работы с Controllino
#include <EEPROM.h> //библиотека для записи переменных в флешпамять

//Переменные сигнальной башни
#define  ledGreen 35 //Реле R13 включения зеленого света в башне
#define  ledRed 34 //Реле R12 включения красного света в башне
#define  beep 36 //Реле R14 включения звукового сигнала в башне

//Переменные для управления приводом вырубной платформы
#define mainSwitch 28 //Реле R6 включения контакторов для питания приводов
#define pinSpeed 2 //аналоговый выход D0 для управления скоростью вращения привода шатунов
#define enableR 32 //Реле R10 сигнал вкл основного привода шатунов. 
#define reverseR 31 //Реле R9 сигнал обратного вращения привода шатунов
#define forwardR 30//Реле R8 сигнал прямого вращения привода шатунов
#define grip 22 //Реле R0 включения муфты сцепления

//Датчики
#define sensorHigh A7 //вход датчика верхнего положения вырубной секции
#define sensorLow A15 //вход датчика нижнего положения вырубной секции

//Константы
const float coefSpeed = 5.76; //коэффициент для преобразования аналогового сигнала в реальную скорость (об/мин)
const int delaybeforegrip = 3000; //задержка перед включением муфты сцепления двигателя с приводом вырубной платформы
const int brakingSpeed = 30; //скорость вырубной платформы в режиме торможения (об/мин)
/*адреса EEPROM*/ 
const int address_height = 0; // адрес ячейки флешпамяти для хранения длины протяжки в мм
const int address_speed = 2; // адрес ячейки флешпамяти для хранения скорости вращения привода шатунов в об/м
const int address_steps = 4; // адрес ячейки флешпамяти для хранения количества шагов на мм протяжки


//Переменные, в которые записываются данные хранящиеся в EEPROM памяти
int value_height = 0; //инициализация переменной длинны протяжки
int value_speed = 0; //инициализация переменной скорости привода шатунов
int value_steps = 0; //инициализация переменной количества шагов на мм протяжки

void setup() {

  Serial.begin(9600);
  Serial.setTimeout(50); //время задержки опроса пакетов

  //Инициализация переменных сигнальной башни
  pinMode(ledGreen, OUTPUT); // выход отвечающий за включение зеленого сигнала
  pinMode(ledRed, OUTPUT); // выход отвечающий за включение красного сигнала
  pinMode(beep, OUTPUT); // выход отвечающий за включение звукового сигнала
  //Инициализация переменных для управления приводом вырубной платформы
  pinMode(mainSwitch, OUTPUT); // выход отвечающий за включение главного пускателя
  pinMode(pinSpeed, OUTPUT); // аналоговый выход для управления скоростью вращения привода шатунов
  pinMode(grip, OUTPUT); // выход отвечающий за включение электромагнитной муфты
  pinMode(enableR, OUTPUT); // сигнал вкл основного привода шатунов.
  pinMode(reverseR, OUTPUT); //сигнал прямого вращения привода шатунов
  pinMode(forwardR, OUTPUT); //сигнал обратного вращения привода шатунов
  //запись данных EEPROM в переменные при загрузке ПЛК:
  EEPROM.get(address_height, value_height); //обращаемся в флешпамять по адресам и записываем значения в переменные 
  EEPROM.get(address_speed, value_speed); 
  EEPROM.get(address_steps, value_steps); 
  //Отправляем значения в панель для визуализации примененых параметров:
  //Serial.print((String)"Start.n1.val=" + value_speed + char(255) + char(255) + char(255)); //скорость платформы в уд/мин
  //Serial.print((String)"Start.n0.val=" + value_height + char(255) + char(255) + char(255)); //длина протяжки в мм
  //Serial.print((String)"Settings.n2.val=" + value_steps + char(255) + char(255) + char(255)); //количество шагов на миллиметр протяжки

  Serial.println("");
  Serial.println(value_speed);
  Serial.println(value_height);
  Serial.println(value_steps);
}


void loop() {
  if (Serial.available()) {  //обработка команд с HMI панели
    char str[30];
    int amount = Serial.readBytesUntil(';', str, 30);
    str[amount] = NULL;
    // разделяет строку на подстроки
    char* ptrs[10];       // указатели на строки
    int count = 0;        // счётчик подстрок
    char* offset = str;   // указатель для работы
    while (true) {
      ptrs[count++] = offset;         // запоминаем указатель
      offset = strchr(offset, ',');   // ищем новую запятую
      if (offset) {                   // если это не NULL
        *offset = NULL;               // заменяем запятую на нулл
        offset++;                     // продолжаем
      }
      else break;                     // иначе покидаем цикл
    }
    // выполнение команд:
    String command = ptrs[0]; //1й элемент массива записываем в переменную "command"
    //далее идет опрос команд
    if (command == "MainSwON") { //если пришла команда на включение приводов
      digitalWrite(ledGreen, 1); //включаем зеленый сигнал указывающей на включение драйверов
      digitalWrite(mainSwitch, 1); //подача питания на частотник и драйвер
    }
    if (command == "MainSwOFF") { //если пришла команда на включение приводов
      digitalWrite(ledGreen, 0); //отключаем зеленый сигнал
      digitalWrite(mainSwitch, 0); //выключаем подачу питания на частотник и драйвер
    }
    if (command == "START") { //если пришла команда "старт"...
      Start_cycle(); //переходим в функцию запуска станка
    }
    if (command == "STOP") { //если пришла команда "стоп"...
      Stop_cycle(); //запускаем функцию остановки
    }
    if (command == "SAVE"){
      value_height = ((String)ptrs[1]).toInt(); //запись значения в переменную длинны протяжки
      value_speed = ((String)ptrs[2]).toInt();  //запись значения в переменную скорости привода шатунов
      EEPROM.put(address_height, value_height); //записываем полученные значения в флешпамять  
      EEPROM.put(address_speed, value_speed);      
      Serial.print((String)"Start.n0.val=" + value_height + char(255) + char(255) + char(255)); //отправляем значения в панель для визуализации примененых параметров
      Serial.print((String)"Start.n1.val=" + value_speed + char(255) + char(255) + char(255));     
    }
    if (command == "SAVESET"){      
      value_steps = ((String)ptrs[1]).toInt(); //запись значения количества шагов на мм в переменную
      EEPROM.put(address_steps, value_steps); //записываем полученное значение в флешпамять 
      Serial.print((String)"Settings.n2.val=" + value_steps + char(255) + char(255) + char(255)); //отправляем полученное значение в HMI панель 
    }

  }





}
