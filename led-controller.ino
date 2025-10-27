// Определяем количество реле и пины
const int NUM_RELAYS = 6;
const int relayPins[NUM_RELAYS] = {2, 3, 4, 5, 6, 7}; // Пины для реле

// Пины для потенциометров включения и выключения
const int potOnPins[NUM_RELAYS] = {A0, A1, A2, A3, A4, A5};
const int potOffPins[NUM_RELAYS] = {A6, A7, A8, A9, A10, A11};

// Переменные для хранения времени
unsigned long previousMillis[NUM_RELAYS] = {0};
bool relayState[NUM_RELAYS] = {false}; // Текущее состояние реле

void setup() {
  // Инициализация последовательного порта для отладки
  Serial.begin(9600);
  
  // Настройка пинов реле как выходов
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW); // Изначально выключаем все реле
  }
  
  Serial.println("Система управления реле инициализирована");
  Serial.println("Используется 6 реле и 12 потенциометров");
}

void loop() {
  unsigned long currentMillis = millis();
  
  for (int i = 0; i < NUM_RELAYS; i++) {
    // Читаем значения с потенциометров
    int potOnValue = analogRead(potOnPins[i]);
    int potOffValue = analogRead(potOffPins[i]);
    
    // Преобразуем значения в миллисекунды (0-1023 -> 0-10000 мс)
    unsigned long delayOn = map(potOnValue, 0, 1023, 0, 10000);
    unsigned long delayOff = map(potOffValue, 0, 1023, 0, 10000);
    
    // Управление реле на основе состояния и времени
    if (!relayState[i]) {
      // Реле выключено - ждем время включения
      if (currentMillis - previousMillis[i] >= delayOn) {
        relayState[i] = true;
        digitalWrite(relayPins[i], HIGH);
        previousMillis[i] = currentMillis;
        
        // Вывод отладочной информации
        Serial.print("Реле ");
        Serial.print(i);
        Serial.print(" ВКЛЮЧЕНО. Задержка ВКЛ: ");
        Serial.print(delayOn);
        Serial.print(" мс, Задержка ВЫКЛ: ");
        Serial.print(delayOff);
        Serial.println(" мс");
      }
    } else {
      // Реле включено - ждем время выключения
      if (currentMillis - previousMillis[i] >= delayOff) {
        relayState[i] = false;
        digitalWrite(relayPins[i], LOW);
        previousMillis[i] = currentMillis;
        
        // Вывод отладочной информации
        Serial.print("Реле ");
        Serial.print(i);
        Serial.print(" ВЫКЛЮЧЕНО. Задержка ВКЛ: ");
        Serial.print(delayOn);
        Serial.print(" мс, Задержка ВЫКЛ: ");
        Serial.print(delayOff);
        Serial.println(" мс");
      }
    }
  }
  
  // Небольшая задержка для стабильности
  delay(10);
}
