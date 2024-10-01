#include <SCServo.h>

SCSCL sc;

// Определение пинов для концевиков
const int button1Pin = 2;  // Концевик 1 (запрещает движение по часовой)
const int button2Pin = 3;  // Концевик 2 (запрещает движение против часовой)

unsigned long debounceDelay = 50;  // Задержка для устранения дребезга
unsigned long lastDebounceTime1 = 0;  // Время последнего изменения состояния для концевика 1
unsigned long lastDebounceTime2 = 0;  // Время последнего изменения состояния для концевика 2

int button1State = HIGH;
int button2State = HIGH;
bool isMovingForward = false;  // Флаг для отслеживания движения по часовой стрелке
bool isMovingBackward = false;  // Флаг для отслеживания движения против часовой стрелки

// Функция для проверки состояния концевиков
bool isMovementAllowed(int pin, int &lastState, unsigned long &lastDebounceTime) {
  int currentState = digitalRead(pin);  // Читаем текущее состояние концевика
  unsigned long currentTime = millis();

  // Проверяем, изменилось ли состояние концевика
  if (currentState != lastState) {
    lastDebounceTime = currentTime;  // Обновляем время изменения состояния
  }

  // Проверяем, прошло ли достаточно времени для исключения дребезга
  if ((currentTime - lastDebounceTime) > debounceDelay) {
    if (currentState == LOW) {
      return false;  // Движение запрещено, концевик нажат
    }
  }

  lastState = currentState;  // Обновляем состояние концевика
  return true;  // Движение разрешено
}

void stopMovement() {
  sc.WritePWM(1, 0);  // Останавливаем движение
  Serial.println("Остановка движения");
  isMovingForward = false;
  isMovingBackward = false;
}

void setup() {
  Serial.begin(115200);  // Для связи с компьютером
  Serial1.begin(1000000);  // Для связи с сервоприводом через Serial1
  sc.pSerial = &Serial1;

  // Настройка пинов концевиков как входов
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  // Переключение в PWM режим
  sc.PWMMode(1);
}

void loop() {
  // Проверяем состояние концевиков и останавливаем движение в соответствующую сторону
  if (isMovingForward && !isMovementAllowed(button1Pin, button1State, lastDebounceTime1)) {
    Serial.println("Концевик 1 нажат, остановка движения по часовой");
    stopMovement();  // Останавливаем только движение вперед
  }
  if (isMovingBackward && !isMovementAllowed(button2Pin, button2State, lastDebounceTime2)) {
    Serial.println("Концевик 2 нажат, остановка движения против часовой");
    stopMovement();  // Останавливаем только движение назад
  }

  // Чтение команд от компьютера
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
    char command = receivedData.charAt(0);  // Извлекаем первый символ как команду
    int speed = receivedData.substring(1).toInt();  // Преобразуем оставшуюся часть в число (скорость)

    // Команда вращения по часовой стрелке (движение вперед)
    if (command == 'F') {
      if (isMovementAllowed(button1Pin, button1State, lastDebounceTime1)) {
        sc.WritePWM(1, speed);  // Вращение по часовой стрелке
        Serial.println("Вращение по часовой");
        isMovingForward = true;
        isMovingBackward = false;  // Останавливаем обратное движение
      } else {
        Serial.println("Вращение по часовой запрещено из-за концевика 1");
        stopMovement();  // Останавливаем движение по часовой, если концевик 1 нажат
      }
    } 
    // Команда вращения против часовой стрелки (движение назад)
    else if (command == 'B') {
      if (isMovementAllowed(button2Pin, button2State, lastDebounceTime2)) {
        sc.WritePWM(1, -speed);  // Вращение против часовой стрелки
        Serial.println("Вращение против часовой");
        isMovingBackward = true;
        isMovingForward = false;  // Останавливаем движение вперед
      } else {
        Serial.println("Вращение против часовой запрещено из-за концевика 2");
        stopMovement();  // Останавливаем движение против часовой, если концевик 2 нажат
      }
    } 
    // Команда остановки
    else if (command == 'S') {
      stopMovement();  // Остановка
    }
  }
}
