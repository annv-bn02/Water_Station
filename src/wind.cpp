#include "wind.h"

#define WIND_SPEED_PIN 25
#define WIND_DIR_PIN 36

uint32_t wind_count = 0, t = 0, round_trip = 0, dir_counter = 0;
float v = 0, s = 0, r = 0.09;
uint8_t old_stt = 0, new_stt = 0;
void Wind_Config(void)
{
    pinMode(WIND_SPEED_PIN, INPUT_PULLUP);
    s = 2 * r * 3.14;
    dir_counter = millis();
}

void Wind_Run(void)
{
    Wind_Speed();
    if(dir_counter + 5000 < millis())
    {
        dir_counter = millis();
        Wind_Convert_Direction(analogRead(WIND_DIR_PIN));
    }
}

void Wind_Speed(void )
{
    old_stt = new_stt;
    new_stt = digitalRead(WIND_SPEED_PIN);
    if(new_stt != old_stt)
    {
        if(wind_count == 0) t = millis();
        wind_count++;
        if(wind_count == 4)
        {
            t = millis() - t;
            wind_count = 0;
            v = s*1000/t; //v(m/s), s(m), t(ms)
            WebSerial.printf("1 round trip\nV = %.2f m/s\n", v);
        }
    }
}

void Wind_Convert_Direction(uint32_t dir_adc)
{
  WebSerial.println(dir_adc);
  if(dir_adc > 0 && dir_adc < 450)
      WebSerial.println("Direction: West - Tay");
  else if(dir_adc < 650)
      WebSerial.println("Direction: West North - Tay Bac");
  else if(dir_adc < 875)
      WebSerial.println("Direction: North - Bac");
  else if(dir_adc < 1050)
      WebSerial.println("Direction: West South - Tay Nam");
  else if(dir_adc < 1300)
      WebSerial.println("Direction: East North - Dong Bac");
  else if(dir_adc < 1750)
      WebSerial.println("Direction: South - Nam");
  else if(dir_adc < 2500)
      WebSerial.println("Direction: East South - Dong Nam");
  else if(dir_adc < 3500)
      WebSerial.println("Direction: East - Dong");
}
