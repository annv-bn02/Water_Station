#include "test_adc.h"
uint32_t count_adc = 0, data = 0, data_calib = 0;
void TA_Config(void)
{

}

void TA_Run(void)
{
    count_adc++;
    data += analogRead(SENSOR_PIN);
    if(count_adc == 1000)
    {
        data_calib = data / 1000;   
        data = 0;
        count_adc = 0;
    }
}

int TA_Sensor_Data(void)
{
    return data_calib;
}
