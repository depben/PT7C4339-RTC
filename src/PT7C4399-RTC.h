/**
 * @file PT7C4339-RTC.h
 * @brief Header file for the PT7C4339 RTC (Real-Time Clock) driver library.
 *
 * This library provides an interface for communicating with the PT7C4339
 * real-time clock chip over I2C. It enables users to read and set the current time and date,
 * manage the square wave output, control the trickle charger, and handle
 * various device-specific features such as oscillator and interrupt settings.
 *
 * @author      Bence Murin
 * @date        2025-05-22
 * @version     0.1.0
 * @copyright   MIT License
 * 
 * @todo Implement alarm functionality.
 * 
**/

#ifndef PT7C4339_H
#define PT7C4339_H

#include <Wire.h>

#define PT7C4339_I2C_ADDR           0x68
#define PT7C4339_I2C_READ_ADDR      0xD1
#define PT7C4339_I2C_WRITE_ADDR     0xD0

#define PT7C4339_REG_SECONDS        0x00
#define PT7C4339_REG_MINUTES        0x01
#define PT7C4339_REG_HOURS          0x02
#define PT7C4339_REG_DAYS_OF_WEEK   0x03
#define PT7C4339_REG_DATES          0x04
#define PT7C4339_REG_MONTHS         0x05
#define PT7C4339_REG_YEARS          0x06
#define PT7C4339_REG_A1_SECONDS     0x07
#define PT7C4339_REG_A1_MINUTES     0x08
#define PT7C4339_REG_A1_HOURS       0x09
#define PT7C4339_REG_A1_DAY_DATE    0x0A
#define PT7C4339_REG_A2_MINUTES     0x0B
#define PT7C4339_REG_A2_HOURS       0x0C
#define PT7C4339_REG_A2_DAY_DATE    0x0D
#define PT7C4339_REG_CONTROL        0x0E
#define PT7C4339_REG_STATUS         0x0F
#define PT7C4339_REG_TRICKLE_CHG    0x10

enum daysOfWeek
{

  UNKNOWN = 0,
  MO,
  TU,
  WE,
  TH,
  FR,
  SA,
  SU

};

enum sqwFrequency
{

  SQW_1HZ = 0x00,
  SQW_4_96KHZ = 0x01,
  SQW_8_192KHZ = 0x02,
  SQW_32_768KHZ = 0x03

};

enum trickleChargerEnabled
{

  TRICKLE_DISABLE = 0x00,
  TRICKLE_ENABLE = 0x0A

};

enum trickleChargerDiode
{

  DIODE_DISABLE = 0x01,
  DIODE_ENABLE = 0x02

};

enum trickleChargerResistor
{

  RESISTOR_DISABLE = 0x00,
  RESISTOR_200R = 0x01,
  RESISTOR_2K = 0x02,
  RESISTOR_4K = 0x03

};

typedef struct
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} PT7C4339_Time;

typedef struct
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  daysOfWeek weekDay;
} PT7C4339_Date;

class PT7C4339
{
  
  public:
    
    PT7C4339( TwoWire *i2cWire = &Wire, uint8_t SDA = 0, uint8_t SCL = 0, uint32_t frequency = 400000, uint8_t i2cAddress = PT7C4339_I2C_ADDR );
    uint8_t begin();
    bool reset();
    PT7C4339_Time getTime();
    bool setTime( PT7C4339_Time time );
    PT7C4339_Date getDate();
    bool setDate( PT7C4339_Date date );
    daysOfWeek calculateWeekDay( uint16_t year, uint8_t month, uint8_t date );
    
    uint8_t getSecond();
    uint8_t getMinute();
    uint8_t getHour();
    daysOfWeek getWeekDay();
    uint8_t getDay();
    uint8_t getMonth();
    uint16_t getYear();
    bool setSecond( uint8_t seconds );
    bool setMinute( uint8_t minutes );
    bool setHour( uint8_t hours );
    bool setCorrectWeekDay();
    bool setDay( uint8_t day );
    bool setMonth( uint8_t month );
    bool setYear( uint16_t year );

    bool isOscillatorEnabled();
    bool enableOscillator( bool enable );

    bool isIntFromBatteryEnabled();
    bool enableIntFromBattery( bool enable );

    sqwFrequency getSqwFrequency();
    bool setSqwFrequency( sqwFrequency frequency );

    bool getRtcStoppedFlag();
    bool clearRtcStoppedFlag();

    bool getIntOrSqwFlag();
    bool setIntOrSqwFlag( bool setting );

    trickleChargerEnabled getTrickleChargerEnabled();
    trickleChargerDiode getTrickleChargerDiode();
    trickleChargerResistor getTrickleChargerResistor();
    bool setTrickleChargerConfig( trickleChargerEnabled enable, trickleChargerDiode diode, trickleChargerResistor resistor );

  private:
    uint8_t _i2cAddress;
    uint8_t _SDA;
    uint8_t _SCL;
    TwoWire *_i2cWire;
    uint32_t _frequency;

    uint8_t bcdToDec( uint8_t bcd );
    uint8_t decToBcd( uint8_t dec );
    
    uint8_t readRegister( uint8_t REG );
    bool writeRegister( uint8_t REG, uint8_t DATA );

    bool readBit( uint8_t REG, uint8_t BIT );
    bool writeBit( uint8_t REG, uint8_t BIT, bool value );


};

#endif