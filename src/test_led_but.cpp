#include "test_led_but.h"
uint8_t but1_old = 0, but1_new = 0, but2_old = 0, but2_new = 0, led1_state = LOW, led2_state = LOW, led3_state = LOW;
uint32_t count = 0;
void TLB_Config(void)
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    pinMode(BUT1, INPUT_PULLUP);
    pinMode(BUT2, INPUT_PULLUP);
}

void TLB_Blink(void)
{
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    delay(500);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    delay(500);
}

void TLB_Button(void)
{
    count++;
    if(count == 1000)
    {
        count = 0;
    }
    but1_old = but1_new;
    but2_old = but2_new;
    if(count % 2 == 1)
    {
        but1_new = digitalRead(BUT1);
        if(but1_new == 0 && but1_old == 1)
        {
            led2_state = !led2_state;
            digitalWrite(LED2, led2_state);
            TS_Send_SMS("0989691022", "test send sms");
        }
    }
    else
    {
        but2_new = digitalRead(BUT2);
        if(but2_new == 0 && but2_old == 1)
        {
            led3_state = !led3_state;
            digitalWrite(LED3, led3_state);
        }
    }
}
