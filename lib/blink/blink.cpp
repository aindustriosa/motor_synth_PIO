#include "blink.h"

void Blink::setup()
{
    pinMode(ledPin, OUTPUT);
    for (int i = 0; i < 10; i++)
    {
        digitalWrite(ledPin, HIGH);
        delay(15);
        digitalWrite(ledPin, LOW);
        delay(15);
    }
}

void Blink::blink()
{
    digitalWrite(ledPin, HIGH);
    delay(150);
    digitalWrite(ledPin, LOW);
    delay(150);
}

void Blink::toggle()
{
    if (ledStatus == LOW)
    {
        ledStatus = HIGH;
    }
    else
    {
        ledStatus = LOW;
    }
    digitalWrite(ledPin, ledStatus);
}