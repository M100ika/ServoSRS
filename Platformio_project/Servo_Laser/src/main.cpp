#include <SCServo.h>

SCSCL sc;

// Определение пинов для кнопок
const int button1Pin = 2;  // Кнопка 1 (запрещает движение по часовой)
const int button2Pin = 3;  // Кнопка 2 (запрещает движение против часовой)

void setup() {
  Serial.begin(115200);  // Для связи с компьютером
  Serial1.begin(1000000);  // Для связи с сервоприводом через Serial1
  
  sc.begin(Serial1);  // Инициализация серво с использованием Serial1

  // Настройка пинов кнопок как входов
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);


  // Проверка нажатия кнопок перед выполнением движений
  int button1State = digitalRead(button1Pin);  // Чтение состояния кнопки 1
  int button2State = digitalRead(button2Pin);  // Чтение состояния кнопки 2

  // Движение к текущей позиции 0 градусов как к новой нулевой
  if (button1State == HIGH) {  // Если кнопка 1 не нажата (разрешено движение по часовой стрелке)
    sc.WritePWM(1, 500);  // Движение по часовой стрелке
    // Serial.println("Движение по часовой стрелке");
  } else {
    // Serial.println("Движение по часовой запрещено в setup()");
  }
  delay(1000);

  sc.WritePWM(1, 0);    // Остановка
  // Serial.println("Остановка");
  delay(1000);

  if (button2State == HIGH) {  // Если кнопка 2 не нажата (разрешено движение против часовой стрелки)
    sc.WritePWM(1, -500); // Движение против часовой стрелки
    // Serial.println("Движение против часовой стрелки");
  } else {
    // Serial.println("Движение против часовой запрещено в setup()");
  }
  delay(1000);

  sc.WritePWM(1, 0);    // Остановка
  // Serial.println("Остановка");
  delay(1000);

  sc.PWMMode(1);  // Переключение в PWM режим
}

void loop() {
  int button1State = digitalRead(button1Pin);  // Чтение состояния кнопки 1
  int button2State = digitalRead(button2Pin);  // Чтение состояния кнопки 2

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
    // Serial.println("Получено сообщение: " + receivedData);  // Добавлено отладочное сообщение

    char command = receivedData.charAt(0);  // Извлекаем первый символ как команду
    int speed = receivedData.substring(1).toInt();  // Преобразуем оставшуюся часть в число (скорость)

    // Serial.println(command);  // Вывод команды
    // Serial.println(speed);    // Вывод скорости

    if (command == 'F') {
      if (button1State == HIGH) {  // Если кнопка 1 не нажата
        sc.WritePWM(1, speed);  // Вращение по часовой стрелке
        // Serial.println("Вращение по часовой");  // Отладочное сообщение
      } else {
        // Serial.println("Вращение по часовой запрещено");  // Отладочное сообщение
      }
    } 
    else if (command == 'B') {
      if (button2State == HIGH) {  // Если кнопка 2 не нажата
        sc.WritePWM(1, -speed);  // Вращение против часовой стрелки
        // Serial.println("Вращение против часовой");  // Отладочное сообщение
      } else {
        // Serial.println("Вращение против часовой запрещено");  // Отладочное сообщение
      }
    } 
    else if (command == 'S') {
      sc.WritePWM(1, 0);  // Остановка
      // Serial.println("Остановка");  // Отладочное сообщение
    }
  }
}
