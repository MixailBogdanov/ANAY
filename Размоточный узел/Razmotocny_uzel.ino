#include <Controllino.h> //библиотека для работы с контроллино
#include <GyverPID.h> //библиотека для работы с ПИД регулятором
#define AUTO_MODE A3 //запуск авторежима
#define PIN_ROLL_UP A4 //поднять рулон
#define PIN_ROLL_DOWN A5 //опустить рулон
#define PIN_VAL_ON 8 //D4 опустить вал прижима картона
#define PIN_CUT 9 //D5 включение ножей
#define PIN_MOTOR 7 //D3 включить двигатель размотки
#define PIN_SPEED 5 //D1(X1) аналоговый сигнал задающий скорость двигателя
#define PIN_TRIG 4
#define PIN_ECHO 6
#define PIN_ROLL_SENSOR A2

//Объявляем экзэмпляр класса ПИД-регулятора
GyverPID regulator(5.2, 0.55, 0.01, 20);  // коэф. П, коэф. И, коэф. Д, период дискретизации dt (мс)

//Переменные для считывания данных с панели
String str; //переменная, в которую записываются данные, получаемые с панели
String command; //переменная команд получаемых с панели
int data[4]; //массив получаемых данных
int c = 0; //счетчик записи
//Флаги, таймеры для работы основного цикла программы
bool flag_start_main_cycle = 0; // флаг старта основного цикла программы
uint32_t timer_millis; //переменная для работы задержек на основе аппаратного таймера

void setup() {
  // Инициализация переменных входов и выходов
  Serial.begin(9600);
  pinMode(PIN_ROLL_UP, OUTPUT);
  pinMode(PIN_ROLL_DOWN, OUTPUT);
  pinMode(PIN_VAL_ON , OUTPUT);
  pinMode(PIN_CUT, OUTPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_SPEED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(AUTO_MODE, INPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_ROLL_SENSOR, INPUT);
  //настройка ПИД-регулятора
  regulator.setDirection(NORMAL); // направление регулирования (NORMAL/REVERSE). Положительное регулирование ставится когда увеличение управляющего сигнала ведет к увеличению сигнала с датчика
  regulator.setLimits(0, 255);    // пределы (ставим для 8 битного ШИМ). ПО УМОЛЧАНИЮ СТОЯТ 0 И 255
  regulator.setpoint = 60;        // сообщаем регулятору расстояние в см, которое он должен поддерживать

  // в процессе работы можно менять коэффициенты
  //regulator.Kp = 5.2;
  //regulator.Ki += 0.5;
  //regulator.Kd = 0;

}

void loop()
{
  Main_cycle(); //запускаем оновной цикл программы размотки
}
