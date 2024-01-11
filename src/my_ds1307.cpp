#include "my_ds1307.h"
#include "RTClib.h"

#define I2C_SDA 12
#define I2C_SCL 14

#define SCAN_I2C  

RTC_DS1307 rtc;
TwoWire I2C_DS1307 = TwoWire(0);
byte error, address;
int nDevices, count_rtc = 0;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
uint8_t md_time[8];
String ds1307;
void MD_Scan_I2C_Address(void);
void MD_Config(void)
{
   
    // Serial.begin(9600);
    if (! I2C_DS1307.begin(I2C_SDA, I2C_SCL, 100000ul)) {
        // Serial.println("Couldn't init I2C Pins");
        // Serial.flush();
        while (1) delay(10);
    }
#ifndef SCAN_I2C
    while (! rtc.begin(&I2C_DS1307)) {
        // Serial.println("Reconnect RTC");
        // Serial.flush();
        delay(1000);
    }

    if (! rtc.isrunning()) {
        // Serial.println("RTC is NOT running, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        rtc.adjust(DateTime(2023, 10, 9, 21, 40, 50));
    }
    // rtc.adjust(DateTime(2023, 11, 15, 16, 47, 0));
#endif
    count_rtc = millis();
}

/**
 * @brief Read RTC value or Scan I2C address dependent SCAN I2C define
 * Data is use for push on Modbus Register and debug data
 */
void MD_Run(void)
{
    // count_rtc++;
    if(millis() > count_rtc + 5000)
    {
#ifdef SCAN_I2C
        MD_Scan_I2C_Address();
        if (! rtc.begin(&I2C_DS1307)) {
        WebSerial.println("Reconnect RTC");
        WebSerial.flush();
        }
#endif
#ifndef SCAN_I2C
        DateTime now = rtc.now();
        // Serial.print(now.year(), DEC);
        // Serial.print('/');
        // Serial.print(now.month(), DEC);
        // Serial.print('/');
        // Serial.print(now.day(), DEC);
        // Serial.print(" (");
        // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        // Serial.print(") ");
        // Serial.print(now.hour(), DEC);
        // Serial.print(':');
        // Serial.print(now.minute(), DEC);
        // Serial.print(':');
        // Serial.print(now.second(), DEC);
        // Serial.println();
        
        uint8_t *temp_data = Convert_From_Uint16_To_Bytes(now.year());
        md_time[0] = temp_data[0];
        md_time[1] = temp_data[1];    
        md_time[2] = now.month();
        md_time[3] = now.day();
        md_time[4] = now.hour();
        md_time[5] = now.minute();
        md_time[6] = now.second();
        md_time[7] = 0;
#if DEBUG_WEB
        ds1307 = "";
        ds1307 += String(now.year()) + "/";
        if(now.month() < 10) ds1307 += "0";
        ds1307 += String(now.month()) + "/";
        if(now.day() < 10) ds1307 += "0";
        ds1307 += String(now.day()) + "; ";
        if(now.hour() < 10) ds1307 += "0";
        ds1307 += String(now.hour()) + ":";
        if(now.minute() < 10) ds1307 += "0";
        ds1307 += String(now.minute()) + ":";
        if(now.second() < 10) ds1307 += "0";
        ds1307 += String(now.second());
        WebSerial.println(ds1307);
#endif
#endif
        count_rtc = millis();
    }
}

/**
 * @brief Function scan I2C address
 * 
 */
void MD_Scan_I2C_Address(void)
{
    WebSerial.println("Scanning...");
    nDevices = 0;
    for(address = 1; address < 127; address++ ) {
        I2C_DS1307.beginTransmission(address);
        error = I2C_DS1307.endTransmission();
        if (error == 0) {
        WebSerial.print("I2C device found at address 0x");
        if (address<16) {
            WebSerial.print("0");
        }
        WebSerial.println(address,HEX);
        nDevices++;
        }
        else if (error==4) {
        WebSerial.print("Unknow error at address 0x");
        if (address<16) {
            WebSerial.print("0");
        }
        WebSerial.println(address,HEX);
        }
        delay(10);    
    }
    if (nDevices == 0) {
        WebSerial.println("No I2C devices found\n");
    }
    else {
        WebSerial.println("done\n");
    }
}

/**
 * @brief Function for setting time DS1307 with WebSerial
 * format setup: setuptime:year/month/day/hour/minute/second
 * @example: setuptime:2023/11/15/13/45/14
 * @param time 
 */
void MD_Setup_Time(String time)
{
    int year, mon, day, hour, min, sec;
    year = (time.substring(10, 14)).toInt();
    mon = (time.substring(15, 17)).toInt();
    day = (time.substring(18, 20)).toInt();
    hour = (time.substring(21, 23)).toInt();
    min = (time.substring(24, 26)).toInt();
    sec = (time.substring(27)).toInt();
    WebSerial.printf("Setup time: %d %d %d %d %d %d\n", (uint16_t)year, (uint8_t)mon, (uint8_t)day, (uint8_t)hour, (uint8_t)min, (uint8_t)sec);
    rtc.adjust(DateTime((uint16_t)year, (uint8_t)mon, (uint8_t)day, (uint8_t)hour, (uint8_t)min, (uint8_t)sec));
}
