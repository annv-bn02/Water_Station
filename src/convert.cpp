#include "convert.h"

/**
 * @brief Converts a floating-point value to four 8-bit unsigned integers.
 *
 * @param data The floating-point value to convert.
 * @return A pointer to an array of four 8-bit unsigned integers.
 */
uint8_t *Convert_From_Float_To_Bytes(float data)
{
    static data_format_float_bytes input_convert;
    input_convert.data_float = data;
    return input_convert.bytes;
}

/**
 * @brief Converts four 8-bit unsigned integers to a floating-point value.
 *
 * @param data0 The first 8-bit unsigned integer to convert.
 * @param data1 The second 8-bit unsigned integer to convert.
 * @param data2 The third 8-bit unsigned integer to convert.
 * @param data3 The fourth 8-bit unsigned integer to convert.
 * @return The converted floating-point value.
 */
float Convert_From_Bytes_To_Float(uint8_t data0, uint8_t data1, uint8_t data2, uint8_t data3)
{
    data_format_float_bytes input_convert;
    input_convert.bytes[3] = data0;
    input_convert.bytes[2] = data1;
    input_convert.bytes[1] = data2;
    input_convert.bytes[0] = data3;
     return input_convert.data_float;
}

/**
 * @brief Converts a floating-point value to a 16-bit unsigned integer.
 *
 * @param data The floating-point value to convert.
 * @return The converted 16-bit unsigned integer value.
 */
uint16_t *Convert_From_Float_To_Uint16(float data)
{
    static data_format_float_Uint16 input_convert;
    input_convert.data_float = data;
    return input_convert.uint16;
}

/**
 * @brief Converts two 16-bit unsigned integers to a floating-point value.
 *
 * @param data0 The first 16-bit unsigned integer to convert.
 * @param data1 The second 16-bit unsigned integer to convert.
 * @return The converted floating-point value.
 */
float Convert_From_Uint16_To_Float(uint16_t data0, uint16_t data1)
{
    data_format_float_Uint16 input_convert;
    input_convert.uint16[0] = data0;
    input_convert.uint16[1] = data1;
    return input_convert.data_float;
}

/**
 * @brief Converts an integer value to four 8-bit unsigned integers.
 *
 * @param data The integer value to convert.
 * @return A pointer to an array of four 8-bit unsigned integers.
 */
uint8_t *Convert_From_Int_To_Bytes(int data)
{
    static data_format_int_bytes input_convert;
    input_convert.data_int = data;
    return input_convert.bytes;
}

/**
 * @brief Converts four 8-bit unsigned integers to a 32-bit unsigned integer.
 *
 * @param data1 The first 8-bit unsigned integer to convert.
 * @param data2 The second 8-bit unsigned integer to convert.
 * @param data3 The third 8-bit unsigned integer to convert.
 * @param data4 The fourth 8-bit unsigned integer to convert.
 * @return The converted 32-bit unsigned integer value.
 */
uint32_t Convert_From_Bytes_To_Int(uint8_t data1, uint8_t data2, uint8_t data3, uint8_t data4)
{
    data_format_int_bytes input_convert;
    input_convert.bytes[3] = data1;
    input_convert.bytes[2] = data2;
    input_convert.bytes[1] = data3;
    input_convert.bytes[0] = data4;
    return input_convert.data_int;
}

/**
 * @brief Converts a 16-bit unsigned integer to two 8-bit unsigned integers.
 *
 * @param data The 16-bit unsigned integer to convert.
 * @return A pointer to an array of two 8-bit unsigned integers.
 */
uint8_t *Convert_From_Uint16_To_Bytes(uint16_t data)
{
    static data_format_uint8_16_t input_convert;
    input_convert.data_uint16 = data;
    return input_convert.bytes;
}

/**
 * @brief Converts two 8-bit unsigned integers to a 16-bit unsigned integer.
 *
 * @param data1 The first 8-bit unsigned integer to convert.
 * @param data2 The second 8-bit unsigned integer to convert.
 * @return The converted 16-bit unsigned integer value.
 */
uint16_t Convert_From_Bytes_To_Uint16(uint8_t data1, uint8_t data2)
{
    data_format_uint8_16_t input_convert;
    input_convert.bytes[0] = data1;
    input_convert.bytes[1] = data2;

    return input_convert.data_uint16;
}
