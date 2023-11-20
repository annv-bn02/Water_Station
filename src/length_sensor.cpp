#include "length_sensor.h"

uint32_t adc_read = 0;
uint32_t adc_kalman = 0;
uint32_t adc_sum = 0;
uint8_t  sample_count = 0;
uint8_t  time_count = 0;        // biến đếm để chạy ADC cho average
float voltage = 0.0;
float kalman_length = 0.0;
float average_length = 0.0;
float adc_average = 0;
float a_param = -0.0106, b_param = 35.743, c_param = -0.0121, d_param = 38.872, e_param = 0.72;
uint32_t count_read;
SimpleKalmanFilter      pressureKalmanFilter(10,1,0.01);

void LengthSensor_KalmanFilter_Read(void){
    if(millis() > count_read + 100)
    {
        count_read = millis();
        adc_read = (analogRead(ADC_PIN));
        adc_kalman = (uint32_t)pressureKalmanFilter.updateEstimate((float) adc_read);
        if(adc_kalman < 2086){
            average_length = a_param * adc_kalman + b_param;
        }else{
            average_length = c_param * adc_kalman + d_param;
        }
        // average_length = -0.0113*adc_kalman + 36.565;
        average_length += e_param;
        // Serial.print(average_length + 0.715, 3);
        // Serial.println(" m");
    }
}

void LengthSensor_Average_Read(void){
    if(millis() > count_read + 1000)
    {
        count_read = millis();
        if(sample_count < SAMPLE){
        adc_read = (analogRead(ADC_PIN) );
        adc_sum += adc_read;
        sample_count ++;
        }
        else{
            adc_average = (float)adc_sum / SAMPLE;
            if(adc_average < 2049){
                average_length = -0.0106*adc_average + 35.767;
            }
            else{
                average_length = -0.0121*adc_average + 38.84;
            }
    //       average_length = -0.0113*adc_average + 36.568;
            adc_sum = 0;
            sample_count = 0;
            // Serial.print(average_length, 3);
            // Serial.println(" m");
        }
    }
}


float LengthSensor_Return_Value(void)
{
    return average_length;
}

void LengthSensor_Setup(void){
    count_read = millis();
}