#include <Arduino.h>

#define LED_PIN1 32
#define LED_PIN2 33

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;

void Task1(void *parameter) {
  for (;;) {
    digitalWrite(LED_PIN1, HIGH);
    vTaskDelay(1000 / portTICK_RATE_MS);

    digitalWrite(LED_PIN1, LOW);
    Serial.println("Task 1: LED OFF");
    vTaskDelay(1000 / portTICK_RATE_MS);

    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());

    Serial.printf("Task1 Stack Free: %u byte", uxTaskGetStackHighWaterMark(NULL));
  }
}

void Task2(void *parameter) {
  for (;;) {
    digitalWrite(LED_PIN2, HIGH);
    vTaskDelay(1000 / portTICK_RATE_MS);

    digitalWrite(LED_PIN2, LOW);
    Serial.println("Task 2: LED OFF");
    vTaskDelay(1000 / portTICK_RATE_MS);

    Serial.print("Task2 running on core ");
    Serial.println(xPortGetCoreID());

    Serial.printf("Task2 Stack Free: %u byte", uxTaskGetStackHighWaterMark(NULL));
  }
}

void setup() {
  Serial.begin(115200);

  delay(100);
  Serial.printf("Start FreeRTOS: Memory Usage\nInitial Free Heap: %u byte", xPortGetFreeHeapSize());

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);

  xTaskCreatePinnedToCore(
    Task1,
    "Task1",
    10000,
    NULL,
    1,
    &Task1Handle,
    1
  );
  xTaskCreatePinnedToCore(
    Task2,
    "Task2",
    10000,
    NULL,
    1,
    &Task2Handle,
    1
  );
}

void loop() {
  static uint32_t currentMillis = 0;
  if (millis() - currentMillis > 5000) {
    Serial.printf("Free Heap: %u bytes\n", xPortGetFreeHeapSize());
    currentMillis = millis();
  }
}