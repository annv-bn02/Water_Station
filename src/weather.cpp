#include "weather.h"

#define WIND_SPEED_PIN 25
#define WIND_DIR_PIN 36
#define RAIN_PIN 35

uint32_t wind_count = 0, time_counter_wind = 0, time_counter_rain = 0, round_trip = 0, dir_counter = 0, rain_counter = 0;
float v = 0, s = 0, r = 0.09, rain_param = 0.3; //R wind speed 0.09cm, Rain param 0.3mm/pulse
uint8_t old_stt = 0, new_stt = 0;
uint8_t old_stt_rain = 0, new_stt_rain = 0;
float wind_v = 0, wind_d = 1, rain_perhour = 0;
void Weather_Config(void)
{
    pinMode(WIND_SPEED_PIN, INPUT_PULLUP);
    pinMode(RAIN_PIN, INPUT_PULLUP);
    s = 2 * r * 3.14;
    dir_counter = millis();
    time_counter_rain = millis();
}

void Weather_Update(void)
{
    Weather_Wind_Speed();
    Weather_Rain();
    if(dir_counter + 5000 < millis())
    {
        dir_counter = millis();
        Weather_Wind_Convert_Direction(analogRead(WIND_DIR_PIN));
    }
}

void Weather_Wind_Speed(void )
{
    old_stt = new_stt;
    new_stt = digitalRead(WIND_SPEED_PIN);
    if(new_stt != old_stt)
    {
        if(wind_count == 0) time_counter_wind = millis();
        wind_count++;
        if(wind_count == 4)
        {
            time_counter_wind = millis() - time_counter_wind;
            wind_count = 0;
            v = s*1000/time_counter_wind; //v(m/s), s(m), t(ms)
            wind_v = v;
            // WebSerial.printf("1 round trip\nV = %.2f m/s\n", v);
        }
    }
}

void Weather_Wind_Convert_Direction(uint32_t dir_adc)
{
    // WebSerial.println(dir_adc);
    if(dir_adc > 0 && dir_adc < 450)
    {
        wind_d = 1;
        WebSerial.println("Direction: West - Tay");
    }
    else if(dir_adc < 650)
    {
        wind_d = 2;
        WebSerial.println("Direction: West North - Tay Bac");
    }        
    else if(dir_adc < 875)
    {
        wind_d = 3;
        WebSerial.println("Direction: North - Bac");
    }        
    else if(dir_adc < 1050)
    {
        wind_d = 4;
        WebSerial.println("Direction: West South - Tay Nam");
    }        
    else if(dir_adc < 1300)
    {
        wind_d = 5;
        WebSerial.println("Direction: East North - Dong Bac");
    }       
    else if(dir_adc < 1750)
    {
        wind_d = 6;
        WebSerial.println("Direction: South - Nam");
    }        
    else if(dir_adc < 2500)
    {
        wind_d = 7;
        WebSerial.println("Direction: East South - Dong Nam");
    }
       
    else if(dir_adc < 3500)
    {
        wind_d = 8;
        WebSerial.println("Direction: East - Dong");
    }
        
}

void Weather_Rain(void)
{
    old_stt_rain = new_stt_rain;
    new_stt_rain = digitalRead(RAIN_PIN);
    if(new_stt_rain == 1 && old_stt_rain == 0)
    { 
        rain_counter++;
    }
    if(time_counter_rain + 36000000 < millis())
    {
        rain_perhour = rain_param * rain_counter;
        rain_counter = 0; //luong mua mm/h 
        time_counter_rain = millis();
    }
}
