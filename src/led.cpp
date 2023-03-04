#include "led.h"

void setColor(int red, int green, int blue)
{
        analogWrite(LED_RED_PIN, red);
        analogWrite(LED_GREEN_PIN, green);
        analogWrite(LED_BLUE_PIN, blue);
}