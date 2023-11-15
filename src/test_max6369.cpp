#include "test_max6369.h"
uint32_t time_on, time_off, time2;
uint8_t LEDState, cases = 0;

/**
 * @brief Configures the IN port watchdog IC
 * 
 */
void TM_Config(void)
{
    pinMode(LED1  , OUTPUT);
}

/**
 * @brief Running change state of IN port watchdog IC
 * switch case for testing what time IC auto reset
 */
void TM_Run(void)
{
     
     switch (cases){
      case 0:
           time_on = 100;
           time_off = 2000 ;                   
           break;
      case 1:
           time_on = 35000;
           time_off = 35000;    
           break;
      case 2:
           time_on = 40000;
           time_off = 40000;  
           break;
      case 3: 
           time_on = 45000;
           time_off = 45000;  
           break;
      case 4: 
           time_on = 55000;
           time_off = 55000;  
           break;
     case 5:
           time_on = 60000;
           time_off = 60000;  
           break;
      case 6: 
           time_on = 70000;
           time_off = 70000;  
           break;
      case 7: 
           time_on = 75000;
           time_off = 75000; 
      default:
      break;
    }

   if(LEDState == HIGH)
   {
       if ( (unsigned long) (millis() - time2) >= time_on )
       {
          LEDState = LOW ;
          time2 = millis();
       }
   }
   else
   {
        if ( (unsigned long) (millis() - time2) >= time_off )
       {
          LEDState = HIGH ;
          time2 = millis();
       }
   }
  digitalWrite(LED1,LEDState);
}
