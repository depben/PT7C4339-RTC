/**
 * @file PT7C4339-RTC.h
 * @brief Header file for the PT7C4339-RTC library.
 *
 * This library provides methods for communicating with the PT7C4339 RTC IC over I2C in the Arduino framework.
 * It enables users to read and set the current time and date, manage the square wave output,
 * control the trickle charger, and use the control mechanisms of the device.
 * More info on the GitHub page: https://github.com/depben/PT7C4339-RTC
 * 
 * @note This library uses 24-hour format for time representation and works from 1900-1-1 to 2099-12-31.
 *
 * @author      Bence Murin
 * @date        2025-05-23
 * @version     0.1.3
 * @copyright   MIT License
 * 
 * @todo Implement alarm functionality.
 * 
**/

#ifndef _PT7C4339_RTC_H_
#define _PT7C4339_RTC_H_

#include <Wire.h>

#define PT7C4339_I2C_ADDRESS          0x68 ///< 7bit I2C address of the PT7C4339 RTC

#define PT7C4339_REG_SECONDS          0x00 ///< Register address for seconds
#define PT7C4339_REG_MINUTES          0x01 ///< Register address for minutes
#define PT7C4339_REG_HOURS            0x02 ///< Register address for hours
#define PT7C4339_REG_DAYS_OF_WEEK     0x03 ///< Register address for days of the week
#define PT7C4339_REG_DATES            0x04 ///< Register address for days
#define PT7C4339_REG_MONTHS           0x05 ///< Register address for months
#define PT7C4339_REG_YEARS            0x06 ///< Register address for years
#define PT7C4339_REG_A1_SECONDS       0x07 ///< Register address for alarm 1 seconds
#define PT7C4339_REG_A1_MINUTES       0x08 ///< Register address for alarm 1 minutes
#define PT7C4339_REG_A1_HOURS         0x09 ///< Register address for alarm 1 hours
#define PT7C4339_REG_A1_DAY_DATE      0x0A ///< Register address for alarm 1 day/date
#define PT7C4339_REG_A2_MINUTES       0x0B ///< Register address for alarm 2 minutes
#define PT7C4339_REG_A2_HOURS         0x0C ///< Register address for alarm 2 hours
#define PT7C4339_REG_A2_DAY_DATE      0x0D ///< Register address for alarm 2 day/date
#define PT7C4339_REG_CONTROL          0x0E ///< Register address for control bits
#define PT7C4339_REG_STATUS           0x0F ///< Register address for status bits
#define PT7C4339_REG_TRICKLE_CHARGER  0x10 ///< Register address for the trickle charger

enum PT7C4339_daysOfWeek ///< Enum for the days of the week of the PT7C4339 RTC
{

  PT7C4339_WEEKDAY_UNKNOWN = 0, ///< Used for calling setDate()
  PT7C4339_MONDAY = 1, ///< Monday
  PT7C4339_TUESDAY = 2, ///< Tuesday
  PT7C4339_WEDNESDAY = 3, ///< Wednesday
  PT7C4339_THURSDAY = 4, ///< Thursday
  PT7C4339_FRIDAY = 5, ///< Friday
  PT7C4339_SATURDAY = 6, ///< Saturday
  PT7C4339_SUNDAY = 7 ///< Sunday

};

enum PT7C4339_sqwFrequency ///< Enum for the frequency of the square wave output of the PT7C4339 RTC
{

  PT7C4339_SQW_1HZ = 0x00, ///< 1Hz square wave output
  PT7C4339_SQW_4_96KHZ = 0x01, ///< 4.96kHz square wave output
  PT7C4339_SQW_8_192KHZ = 0x02, ///< 8.192kHz square wave output
  PT7C4339_SQW_32_768KHZ = 0x03 ///< 32.768kHz square wave output

};

enum PT7C4339_trickleChargerEnabled ///< Enum for the trickle charger enable setting of the PT7C43339 RTC
{

  PT7C4339_TRICKLE_DISABLE = 0x00, ///< The trickle charger is disabled
  PT7C4339_TRICKLE_ENABLE = 0x0A ///< The trickle charger is enabled

};

enum PT7C4339_trickleChargerDiode ///< Enum for the trickle charger diode enable setting of the PT7C43339 RTC
{

  PT7C4339_DIODE_DISABLE = 0x01, ///< No series diode from Vcc to Vbackup
  PT7C4339_DIODE_ENABLE = 0x02 ///< Series diode from Vcc to Vbackup

};

enum PT7C4339_trickleChargerResistor ///< Enum for the trickle charger resistor settings of the PT7C43339 RTC
{

  PT7C4339_RESISTOR_DISABLE = 0x00, ///< No resistor from Vcc to Vbackup - trickle charger disabled
  PT7C4339_RESISTOR_200R = 0x01, ///< 200 Ohm resistor from Vcc to Vbackup
  PT7C4339_RESISTOR_2K = 0x02, ///< 2K Ohm resistor from Vcc to Vbackup
  PT7C4339_RESISTOR_4K = 0x03 ///< 4K Ohm resistor from Vcc to Vbackup

};

/**
 * @struct PT7C4339_Time
 * Time structure for the PT7C4339 RTC
 */
typedef struct
{

  uint8_t hour; ///< Hours (0-23)
  uint8_t minute; ///< Minutes (0-59)
  uint8_t second; ///< Seconds (0-59)

} PT7C4339_Time; ///< Time structure for the PT7C4339 RTC

/**
 * @struct PT7C4339_Date
 * Date structure for the PT7C4339 RTC
 */
typedef struct
{

  uint16_t year; ///< Year (1900-2099)
  uint8_t month; ///< Month (1-12)
  uint8_t day; ///< Day (1-31)
  PT7C4339_daysOfWeek weekDay; ///< Day of the week (1-7, where 1 = Monday and 7 = Sunday)

} PT7C4339_Date; ///< Date structure for the PT7C4339 RTC

class PT7C4339 ///< Class for the PT7C4339 RTC
{
  
  public:
    
    PT7C4339( TwoWire *i2cWire = &Wire, uint8_t SDA = 0, uint8_t SCL = 0, uint32_t frequency = 400000 );
    
    uint8_t begin();
    bool reset();
    
    PT7C4339_Time getTime();
    bool setTime( PT7C4339_Time time );
    
    PT7C4339_Date getDate();
    bool setDate( PT7C4339_Date date );
    
    uint8_t getSecond();
    bool setSecond( uint8_t seconds );
    
    uint8_t getMinute();
    bool setMinute( uint8_t minutes );

    uint8_t getHour();
    bool setHour( uint8_t hours );

    uint16_t getYear();
    bool setYear( uint16_t year );

    uint8_t getMonth();
    bool setMonth( uint8_t month );

    uint8_t getDay();
    bool setDay( uint8_t day );

    PT7C4339_daysOfWeek getWeekDay();
    bool setCorrectWeekDay(); // Should not be needed as it gets called by every date setter, but leaving it public just in case
    PT7C4339_daysOfWeek calculateWeekDay( uint16_t year, uint8_t month, uint8_t day );

    bool isOscillatorEnabled();
    bool enableOscillator( bool enable );

    bool getRtcStopFlag();
    bool clearRtcStopFlag();

    bool isIntFromBatteryEnabled();
    bool enableIntFromBattery( bool enable );

    bool getIntOrSqwFlag();
    bool setIntOrSqwFlag( bool setting );

    PT7C4339_sqwFrequency getSqwFrequency();
    bool setSqwFrequency( PT7C4339_sqwFrequency frequency );

    PT7C4339_trickleChargerEnabled getTrickleChargerEnabled();
    PT7C4339_trickleChargerDiode getTrickleChargerDiode();
    PT7C4339_trickleChargerResistor getTrickleChargerResistor();
    bool setTrickleChargerConfig( PT7C4339_trickleChargerEnabled enable, PT7C4339_trickleChargerDiode diode, PT7C4339_trickleChargerResistor resistor );

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