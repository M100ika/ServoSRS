#include <SCServo.h>

SCSCL sc;

// Определение пинов для кнопок
const int button1Pin = 2;  // Кнопка 1 (запрещает движение по часовой)
const int button2Pin = 3;  // Кнопка 2 (запрещает движение против часовой)

void setup() {
  Serial.begin(115200);  // Для связи с компьютером
  Serial1.begin(1000000);  // Для связи с сервоприводом через Serial1
  sc.pSerial = &Serial1;

  // Настройка пинов кнопок как входов
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  // Перекалибровка нулевой позиции (в случае, если поддерживается)
  // Например, sc.ResetPos(1); или аналогичная команда

  // Движение к текущей позиции 0 градусов как к новой нулевой
  sc.WritePWM(1, 500);
  delay(1000);
  sc.WritePWM(1, 0);
  delay(1000);
  sc.WritePWM(1, -500);
  delay(1000);
  sc.WritePWM(1, 0);
  
  sc.PWMMode(1);  // Переключение в PWM режим
}

void loop() {
  int button1State = digitalRead(button1Pin);  // Чтение состояния кнопки 1
  int button2State = digitalRead(button2Pin);  // Чтение состояния кнопки 2
  Serial.print("B1:");
  Serial.println(button1State);
  

  if (Serial.available() > 0) {
    String receivedData = "";  // Создаем пустую строку для хранения данных
    char incomingChar;

    // Читаем данные из Serial по одному символу, пока они доступны
    while (Serial.available() > 0) {
      incomingChar = Serial.read();
      receivedData += incomingChar;
      delay(10);  // Небольшая задержка для стабилизации (опционально)
    }

    // Теперь данные собраны в receivedData
    Serial.println("Получено сообщение: " + receivedData);  // Добавлено отладочное сообщение

    char command = receivedData.charAt(0);  // Извлекаем первый символ как команду
    int speed = receivedData.substring(1).toInt();  // Преобразуем оставшуюся часть в число (скорость)

    Serial.println(command);  // Вывод команды
    Serial.println(speed);    // Вывод скорости


//2 скорость - 51мм/мин
//5 скорость - 153мм/мин
//10 скорость  - 306мм/мин


    if (command == 'F') {
      if (button1State == HIGH) {  // Если кнопка 1 не нажата
        sc.WritePWM(1, speed);
        Serial.println("Вращение по часовой");  // Отладочное сообщение
      } else {
        Serial.println("Вращение по часовой запрещено");  // Отладочное сообщение
      }
    } 
    else if (command == 'B') {
      if (button2State == HIGH) {  // Если кнопка 2 не нажата
        sc.WritePWM(1, -speed);
        Serial.println("Вращение против часовой");  // Отладочное сообщение
      } else {
        Serial.println("Вращение против часовой запрещено");  // Отладочное сообщение
      }
    } 
    else if (command == 'S') {
      sc.WritePWM(1, 0);
      Serial.println("Остановка");  // Отладочное сообщение
    }
  }
}
