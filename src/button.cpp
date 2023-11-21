#include "button.h"
uint8_t but1_old = 0, but1_new = 0, but2_old = 0, but2_new = 0, led1_state = LOW, led2_state = LOW, led3_state = LOW;
uint32_t count = 0;
button_s button_1, button_2, button_3, button_4, button_5;
uint8_t button_state_change;
/**
 * @brief Config for LED and BUTTON
 * 
 */
void Button_Config(void)
{
    button_state_change = 0;
    pinMode(BUTTON_1, INPUT_PULLDOWN);
    pinMode(BUTTON_2, INPUT_PULLDOWN);
    pinMode(BUTTON_3, INPUT_PULLDOWN);
    pinMode(BUTTON_4, INPUT_PULLDOWN);
    pinMode(BUTTON_5, INPUT_PULLDOWN);
}

/**
 * @brief Function read button 
 * 
 * @param button_struct : struct of button data
 * @param button_read : data read from button current
 * @return uint8_t : return 1 if button is push, 0 otherwise
 */
uint8_t Button_Read(button_s *button_struct, unsigned char button_read)
{
    //di'
	// if(!button_read){
	// 	if(button_read == button_struct->vruc_OldStt){
	// 		if(button_struct->vruc_CountStt <= BUTTON_THERSHOLD){
	// 			button_struct->vruc_CountStt++;
	// 			if(button_struct->vruc_CountStt == BUTTON_THERSHOLD){
	// 				button_struct->vruc_Flag = 1;
	// 			}
	// 		}
	// 	}
	// 	else{
	// 		button_struct->vruc_CountStt = 0;
	// 	}
	// }
	// else{
	// 	button_struct->vruc_CountStt = 0;
	// }
    if(button_read == 1 && button_struct->vruc_OldStt == 0)
    {
        button_struct->vruc_OldStt = button_read;
        return 1;
    }
	button_struct->vruc_OldStt = button_read;
    return 0;
}

/**
 * @brief Function scan button
 * 
 */
void Button_Scan(void)
{
    if(Button_Read(&button_1, digitalRead(BUTTON_1)))
    {
        button_state_change = 1;
#if DEBUG_WEB
        WebSerial.printf("Button 1 push\n");
#endif
    }
    if(Button_Read(&button_2, digitalRead(BUTTON_2)))
    {
        button_state_change = 1;
#if DEBUG_WEB
        WebSerial.printf("Button 2 push\n");
#endif
    }
//     if(Button_Read(&button_3, digitalRead(BUTTON_3)))
//     {
//         button_state_change = 1;
// #if DEBUG_WEB
//         WebSerial.printf("Button 3 push\n");
// #endif
//     }
//     if(Button_Read(&button_4, digitalRead(BUTTON_4)))
//     {
//         button_state_change = 1;
// #if DEBUG_WEB
//         WebSerial.printf("Button 4 push\n");
// #endif
//     }
//     if(Button_Read(&button_5, digitalRead(BUTTON_5)))
//     {
//         button_state_change = 1;
// #if DEBUG_WEB
//         WebSerial.printf("Button 5 push\n");
// #endif
//     }


}
