#ifndef _CONVERT_H_
#define _CONVERT_H_

#include <Arduino.h>

/**
 * @brief union convert float <-> uint8_t array
 * 
 */
typedef union 
{
	float data_float;
	uint8_t bytes[4];	
} data_format_float_bytes;

/**
 * @brief union convert float <-> uint16_t array
 * 
 */
typedef union 
{
	float data_float;
	uint16_t uint16[2];	
} data_format_float_Uint16;

/**
 * @brief union convert int <-> uint8_t array
 * 
 */
typedef union 
{
    uint32_t data_int;
    uint8_t bytes[4];
} data_format_int_bytes;

/**
 * @brief union convert uint16_t <-> uint8_t array
 * 
 */
typedef union 
{
    uint16_t data_uint16;
    uint8_t bytes[2];
} data_format_uint8_16_t;


uint8_t* Convert_From_Float_To_Bytes(float data);

float Convert_From_Bytes_To_Float(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);

uint16_t* Convert_From_Float_To_Uint16(float data);

float Convert_From_Uint16_To_Float(uint16_t data1, uint16_t data2);

uint8_t* Convert_From_Int_To_Bytes(int data);

uint32_t Convert_From_Bytes_To_Int(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4);

uint8_t* Convert_From_Uint16_To_Bytes(uint16_t data);

uint16_t Convert_From_Bytes_To_Uint16(uint8_t data1, uint8_t data2);


#endif
