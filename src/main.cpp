#include <Arduino.h>

#define LED_PIN1 32
#define LED_PIN2 33
#define BTN_PIN 14 // PULLUP BTN

TaskHandle_t Task1Handle = NULL;
TaskHandle_t Task2Handle = NULL;
TaskHandle_t TaskBtnHandle = NULL;

void Task1(void *parameter) {
  for (;;) {
    digitalWrite(LED_PIN1, HIGH);
    vTaskDelay(1000 / portTICK_RATE_MS);

    digitalWrite(LED_PIN1, LOW);
    Serial.println("Task 1: LED OFF");
    vTaskDelay(1000 / portTICK_RATE_MS);

    Serial.println("Task1 running on core " + static_cast<String>(xPortGetCoreID()));

    Serial.printf("Task1 Stack Free: %u bytes\n", uxTaskGetStackHighWaterMark(NULL));
  }
}

void Task2(void *parameter) {
  for (;;) {
    digitalWrite(LED_PIN2, HIGH);
    vTaskDelay(1000 / portTICK_RATE_MS);

    digitalWrite(LED_PIN2, LOW);
    Serial.println("Task 2: LED OFF");
    vTaskDelay(1000 / portTICK_RATE_MS);

    Serial.println("Task2 running on core " + static_cast<String>(xPortGetCoreID()));

    Serial.printf("Task2 Stack Free: %u bytes\n", uxTaskGetStackHighWaterMark(NULL));
  }
}

void TaskBtn(void *parameter) {
  for (;;) {
    static bool currentBtnState = false;
    if (!digitalRead(BTN_PIN)&&currentBtnState) {
      Serial.println("TaskBtn: Button Press");
    }
    currentBtnState = digitalRead(BTN_PIN);
  }
}

void setup() {
  Serial.begin(115200);

  delay(100);
  Serial.printf("Start FreeRTOS: Memory Usage\nInitial Free Heap: %u bytes\n", xPortGetFreeHeapSize());

  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(BTN_PIN, INPUT);

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
  xTaskCreatePinnedToCore(
    TaskBtn,
    "TaskBtn",
    10000,
    NULL,
    1,
    &TaskBtnHandle,
    1);
}

void loop() {
  static uint32_t currentMillis = 0;
  if (millis() - currentMillis > 5000) {
    Serial.printf("Free Heap: %u bytes\n", xPortGetFreeHeapSize());
    currentMillis = millis();
  }
}