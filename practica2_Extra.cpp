#include <Arduino.h>

#define LED_PIN 2
#define BUTTON_UP 18
#define BUTTON_DOWN 19

volatile int blinkFrequency = 2;  // Frecuencia inicial en Hz
volatile bool ledState = false;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
    static unsigned long lastPressUp = 0;
    static unsigned long lastPressDown = 0;
   
    // Cambia el estado del LED
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);

    // Leer los botones con debounce
    if (digitalRead(BUTTON_UP) == LOW && millis() - lastPressUp > 200) {
        if (blinkFrequency < 10) blinkFrequency++;  // Límite superior
        lastPressUp = millis();
    }

    if (digitalRead(BUTTON_DOWN) == LOW && millis() - lastPressDown > 200) {
        if (blinkFrequency > 1) blinkFrequency--;  // Límite inferior
        lastPressDown = millis();
    }

    // Actualizar el periodo del timer
    timerAlarmWrite(timer, 500000 / blinkFrequency, true);
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);

    // Configurar el timer (0) con una interrupción cada 500ms (ajustable)
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 500000 / blinkFrequency, true);
    timerAlarmEnable(timer);
}

void loop() {
    // No es necesario código en loop, todo lo maneja la interrupción
}