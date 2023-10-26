#include "test_max6369.h"
uint32_t time_led_xanh_on, time_led_xanh_off, time2;
uint8_t LEDState, cases = 0;
void TM_Config(void)
{
    Serial2.begin(9600);
    pinMode(LED1  , OUTPUT);
    digitalWrite(25, HIGH);
    digitalWrite(26, HIGH); 
    Serial2.println("Test1");
    delay(10);
    Serial2.println("Test2");
    delay(10);
    digitalWrite(25, LOW);
    digitalWrite(26, LOW); 
}

void TM_Run(void)
{
     // while(Serial2.available())
     // {
     //      cases = Serial2.read() - '0';
     // }
     
     switch (cases){
      case 0:
           time_led_xanh_on = 10000;
           time_led_xanh_off = 10000 ;                   
           break;
      case 1:
           time_led_xanh_on = 35000;
           time_led_xanh_off = 35000;    
           break;
      case 2:
           time_led_xanh_on = 40000;
           time_led_xanh_off = 40000;  
           break;
      case 3: 
           time_led_xanh_on = 45000;
           time_led_xanh_off = 45000;  
           break;
      case 4: 
           time_led_xanh_on = 55000;
           time_led_xanh_off = 55000;  
           break;
     case 5:
           time_led_xanh_on = 60000;
           time_led_xanh_off = 60000;  
           break;
      case 6: 
           time_led_xanh_on = 70000;
           time_led_xanh_off = 70000;  
           break;
      case 7: 
           time_led_xanh_on = 75000;
           time_led_xanh_off = 75000; 
      default:
      break;
    }

   if(LEDState == HIGH)
   {
       if ( (unsigned long) (millis() - time2) >= time_led_xanh_on )
       {
          LEDState = LOW ;
          time2 = millis();
       }
   }
   else
   {
        if ( (unsigned long) (millis() - time2) >= time_led_xanh_off )
       {
          LEDState = HIGH ;
          time2 = millis();
       }
   }
  digitalWrite(LED1,LEDState);
}
