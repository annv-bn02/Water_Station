#include "test_adc.h"
uint32_t count_adc = 0, data = 0;
float sensor_calib = 0;
String sensor;
void TA_Config(void)
{
    LengthSensor_Setup();
}

void TA_Run(void)
{
    // count_adc++;
    // data += analogRead(SENSOR_PIN);
    // if(count_adc == 1000)
    // {
    //     sensor_calib = data / 1000;   
    //     sensor = String(sensor_calib);
    //     data = 0;
    //     count_adc = 0;
    // }
    LengthSensor_KalmanFilter_Read();
    count_adc++;
    if(count_adc == 1000)
    {
        sensor_calib = LengthSensor_Return_Value();
        count_adc = 0;
    }
}

float TA_Sensor_Data(void)
{
    return sensor_calib;
}
