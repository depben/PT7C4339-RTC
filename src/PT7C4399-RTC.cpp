/**
 * @file PT7C4339-RTC.cpp
 * @brief Implementation file for the PT7C4339 RTC (Real-Time Clock) driver library.
 *
 * This library provides an interface for communicating with the PT7C4339
 * real-time clock chip over I2C. It enables users to read and set the current time and date,
 * manage the square wave output, control the trickle charger, and handle
 * various device-specific features such as oscillator and interrupt settings.
 *
 * ## Main Features and Functions
 * - **Initialization and Communication**
 *   - `begin()`: Initializes the RTC and I2C bus, ensures the device is in 24-hour mode, and checks for power loss.
 *
 * - **Time and Date Handling**
 *   - `getTime()`, `setTime()`: Retrieve or set the current time (hours, minutes, seconds).
 *   - `getDate()`, `setDate()`: Retrieve or set the current date (year, month, day, weekday).
 *   - Individual getters/setters for each time/date component: `getHour()`, `setHour()`, etc.
 *   - Automatic weekday calculation on every call of a date setter.
 *
 * - **Alarm and Output Control**
 *   - Alarm functionality not yet implemented
 *   - Square wave output configuration: `getSqwFrequency()`, `setSqwFrequency()`.
 *   - Output mode selection: `getIntOrSqwFlag()`, `setIntOrSqwFlag()`.
 *
 * - **Oscillator and Power Management**
 *   - Enable/disable oscillator: `isOscillatorEnabled()`, `enableOscillator()`.
 *   - Battery-backed interrupt: `isIntFromBatteryEnabled()`, `enableIntFromBattery()`.
 *   - RTC stopped flag handling: `getRtcStoppedFlag()`, `clearRtcStoppedFlag()`.
 *
 * - **Trickle Charger Configuration**
 *   - Query and configure trickle charger: `getTrickleChargerEnabled()`, `getTrickleChargerDiode()`, `getTrickleChargerResistor()`, `setTrickleChargerConfig()`.
 *
 * - **Device Reset**
 *   - `reset()`: Restores all RTC registers to their default power-on values.
 *
 *
 * @author      Bence Murin
 * @date        2025-05-21
 * @version     0.1.0
 * @copyright   MIT License
 * 
 * 
**/

#include "PT7C4399-RTC.h"
#include <Arduino.h>

/**
 * @brief Constructs a PT7C4339 RTC object with specified I2C parameters.
 * 
 * @param i2cAddress The I2C address of the PT7C4339 device.
 * @param i2cWire Pointer to the TwoWire instance to use for I2C communication.
 * @param SDA The pin number used for the I2C SDA line.
 * @param SCL The pin number used for the I2C SCL line.
 */
PT7C4339::PT7C4339( TwoWire *i2cWire, uint8_t SDA, uint8_t SCL, uint32_t frequency, uint8_t i2cAddress )
{

  _i2cAddress = i2cAddress;
  _i2cWire = i2cWire;
  _SDA = SDA;
  _SCL = SCL;
  _frequency = frequency;

}

/**
 * @brief Initializes the PT7C4339 RTC over I2C.
 *
 * This function attempts to establish I2C communication with the PT7C4339 device.
 * If the initial transmission fails, it will reinitialize the I2C bus using either
 * default or custom SDA/SCL pins and the specified frequency. It also ensures the
 * RTC is set to 24-hour mode if it was previously in 12-hour mode.
 *
 * @return uint8_t
 *         - 0: Initialization failed (I2C communication error or failed to set 24-hour mode)
 *         - 1: Initialization successful and RTC is in 24-hour mode
 *         - 2: Initialization successful but RTC stopped flag is set - power failure or was reset
 */
uint8_t PT7C4339::begin()
{

  _i2cWire->beginTransmission( PT7C4339_I2C_ADDR );
  uint8_t error = _i2cWire->endTransmission();

  if( error != 0 )
  {

    if( _SDA == 0 && _SCL == 0 )
    {

      _i2cWire->begin();
      _i2cWire->setClock( _frequency );

    }
    else
    {

      _i2cWire->begin( _SDA, _SCL );
      _i2cWire->setClock( _frequency );

    }

    _i2cWire->beginTransmission( PT7C4339_I2C_ADDR );
    error = _i2cWire->endTransmission();

    if( error != 0 ) return 0;

  }

  uint8_t hours = readRegister( PT7C4339_REG_HOURS );

  bool is12H = hours & 0x40;
  if( is12H )
  {

    hours &= 0xBF;
    if( !writeRegister( PT7C4339_REG_HOURS, hours ) ) return 0;

  }

  if( getRtcStoppedFlag() ) return 2;
  else return 1;

}

/**
 * @brief Converts a BCD (Binary-Coded Decimal) value to its decimal equivalent.
 *
 * This function takes an 8-bit BCD value and converts it to the corresponding
 * decimal value. BCD encodes each decimal digit in 4 bits.
 *
 * @param bcd The BCD value to convert.
 * @return uint8_t The decimal representation of the BCD value.
 */
uint8_t PT7C4339::bcdToDec( uint8_t bcd )
{

  return ( ( ( bcd >> 4 ) * 10 ) + ( bcd & 0x0F ) );

}

/**
 * @brief Converts a decimal value to its Binary-Coded Decimal (BCD) representation.
 *
 * This function takes an 8-bit unsigned integer representing a decimal value (0-99)
 * and converts it to its equivalent BCD format. In BCD, each nibble (4 bits) of the byte
 * represents a decimal digit.
 *
 * @param dec The decimal value to convert (expected range: 0-99).
 * @return uint8_t The BCD representation of the input decimal value.
 */
uint8_t PT7C4339::decToBcd( uint8_t dec )
{

  return ( ( ( dec / 10 ) << 4) | ( dec % 10 ) );

}

/**
 * @brief Reads a single byte from the specified register of the PT7C4339 device via I2C.
 *
 * This function initiates an I2C transmission to the PT7C4339 device, writes the register address,
 * ends the transmission, and then requests one byte of data from the specified register.
 *
 * @param REG The register address to read from.
 * @return uint8_t The value read from the specified register.
 */
uint8_t PT7C4339::readRegister( uint8_t REG )
{

  uint8_t registerData;

  _i2cWire->beginTransmission( PT7C4339_I2C_ADDR );
  _i2cWire->write( REG );
  _i2cWire->endTransmission();

  _i2cWire->requestFrom( PT7C4339_I2C_ADDR, 1 );
  while( _i2cWire->available() )
  {

    registerData = _i2cWire->read();

  }

  return registerData;

}

/**
 * @brief Writes a byte of data to a specified register of the PT7C4339 device over I2C.
 *
 * This function sends the register address and data byte to the PT7C4339 via the I2C bus,
 * then reads back the register to verify that the write was successful.
 *
 * @param REG The register address to write to.
 * @param DATA The data byte to write to the register.
 * @return bool true if the data was successfully written and verified, false otherwise.
 */
bool PT7C4339::writeRegister( uint8_t REG, uint8_t DATA )
{

  bool writeSuccess;

  _i2cWire->beginTransmission( PT7C4339_I2C_ADDR );
  _i2cWire->write( REG );
  _i2cWire->write( DATA );
  _i2cWire->endTransmission();

  if( DATA == readRegister( REG ) ) writeSuccess = true;
  else writeSuccess = false;

  return writeSuccess;

}

/**
 * @brief Retrieves the current time from the PT7C4339 RTC module.
 *
 * This function reads the current seconds, minutes, and hour from the RTC
 * and returns them encapsulated in a PT7C4339_Time structure.
 *
 * @return PT7C4339_Time Structure containing the current time (hours, minutes, seconds).
 */
PT7C4339_Time PT7C4339::getTime()
{

  PT7C4339_Time time;

  time.hour = getHour();
  time.minute = getMinute();
  time.second = getSecond();
  
  return time;

}

/**
 * @brief Sets the current time of the PT7C4339 RTC.
 *
 * Validates the provided time values (hour, minute, second) and sets them if they are within valid ranges.
 * The time is only set if all values are valid:
 * - hour:   0-23
 * - minute: 0-59
 * - second: 0-59
 *
 * @param time A PT7C4339_Time struct containing the hour, minute, and second to set.
 * @return bool True if the time was successfully set, false otherwise.
 */
bool PT7C4339::setTime( PT7C4339_Time time )
{

  bool setSuccess = false;

  if( time.hour < 24 && time.minute < 60 && time.second < 60 )
  {

    setSuccess = setHour( time.hour ) && setMinute( time.minute ) && setSecond( time.second );

  }
  else setSuccess = false;

  return setSuccess;

}

/**
 * @brief Retrieves the current date from the PT7C4339 RTC.
 *
 * This function reads the year, month, day, and weekday from the RTC
 * and returns them as a PT7C4339_Date structure.
 *
 * @return PT7C4339_Date Structure containing the current date and weekday.
 */
PT7C4339_Date PT7C4339::getDate()
{

  PT7C4339_Date date;

  date.year = getYear();
  date.month = getMonth();
  date.day = getDay();
  date.weekDay = getWeekDay();

  return date;

}

/**
 * @brief Sets the date of the PT7C4339 RTC.
 *
 * This function sets the year, month, and day of the PT7C4339 real-time clock
 * using the values provided in the PT7C4339_Date structure.
 *
 * @param date The PT7C4339_Date structure containing the year, month, and day to set.
 * @return true if all date components (year, month, day) were set successfully; false otherwise.
 *
 * @note The weekDay field of the input parameter is ignored. The correct weekday is automatically 
 * calculated and set based on the provided year, month, and day.
 */
bool PT7C4339::setDate( PT7C4339_Date date )
{

  bool setSuccess = setYear( date.year ) && setMonth( date.month ) && setDay( date.day );

  return setSuccess;

}

/**
 * @brief Calculates the day of the week for a given date.
 *
 * This function determines the day of the week (e.g., Monday, Tuesday, etc.)
 * for the specified year, month, and date using a variant of the Zeller's congruence algorithm.
 *
 * @param year  The full year (e.g., 2024).
 * @param month The month (1 = January, 12 = December).
 * @param date  The day of the month (1-31).
 * @return daysOfWeek The calculated day of the week as an enum value, where 1 represents Monday, 7 Sunday.
 *
 * @note The function assumes the Gregorian calendar and supports years before and after 2000.
 */
daysOfWeek PT7C4339::calculateWeekDay( uint16_t year, uint8_t month, uint8_t date )
{

  uint8_t yearCode;
  uint8_t monthCode;
  uint8_t centuryCode;
  bool leapYearMod = false;

  uint8_t yearLastTwoDigits = year % 100;
  yearCode = ( ( yearLastTwoDigits % 100 ) + ( ( yearLastTwoDigits % 100 ) / 4 ) ) % 7;

  bool isLeapYear = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );

  switch ( month )
  {
    
    case 1:
      monthCode = 0;
      if( isLeapYear ) leapYearMod = true;
      break;

    case 2:
      monthCode = 3;
      if( isLeapYear ) leapYearMod = true;
      break;

    case 3:
      monthCode = 3;
      break;

    case 4:
      monthCode = 6;
      break;

    case 5:
      monthCode = 1;
      break;

    case 6:
      monthCode = 4;
      break;

    case 7:
      monthCode = 6;
      break;

    case 8:
      monthCode = 2;
      break;

    case 9:
      monthCode = 5;
      break;

    case 10:
      monthCode = 0;
      break;

    case 11:
      monthCode = 3;
      break;

    case 12:
      monthCode = 5;
      break;
  
  }

  bool is2000;

  if( year < 2000 ) is2000 = false;
  else is2000 = true;

  if( is2000 ) centuryCode = 6;
  else centuryCode = 0;

  uint8_t offsetWeekday = ( yearCode + monthCode + centuryCode + date - leapYearMod ) % 7;
  if (offsetWeekday == 0) offsetWeekday = 7;

  daysOfWeek calculatedWeekDay = static_cast<daysOfWeek>( offsetWeekday );

  return calculatedWeekDay;

}

/**
 * @brief Reads the value of a specific bit from a register.
 *
 * This function reads the value of the specified bit (BIT) from the given register (REG)
 * of the PT7C4339 device. It returns true if the bit is set (1), or false if the bit is clear (0).
 *
 * @param REG The address of the register to read from.
 * @param BIT The bit position within the register to read (0-7).
 * @return bool true if the specified bit is set, false otherwise.
 */
bool PT7C4339::readBit( uint8_t REG, uint8_t BIT )
{

  uint8_t registerData = readRegister( REG );
  bool bitValue = ( registerData >> BIT ) & 0x01;

  return bitValue;

}


/**
 * @brief Sets or clears a specific bit in a register of the PT7C4339 device.
 *
 * This function reads the current value of the specified register, modifies the
 * specified bit according to the provided value, and writes the updated value
 * back to the register.
 *
 * @param REG The address of the register to modify.
 * @param BIT The bit position (0-7) within the register to set or clear.
 * @param value If true, the bit is set; if false, the bit is cleared.
 * @return true if the register write operation was successful, false otherwise.
 */
bool PT7C4339::writeBit( uint8_t REG, uint8_t BIT, bool value )
{

  uint8_t registerData = readRegister( REG );

  if( value == true ) registerData |= ( 1 << BIT );
  else registerData &= ~( 1 << BIT );

  bool writeSuccess = writeRegister( REG, registerData );

  return writeSuccess;
  
}

/**
 * @brief Retrieves the current seconds value from the PT7C4339 RTC.
 *
 * This function reads the seconds register from the PT7C4339 real-time clock (RTC),
 * masks out the unused bit, converts the value from BCD to decimal, and returns it.
 *
 * @return uint8_t The current seconds (0-59).
 */
uint8_t PT7C4339::getSecond()
{

  uint8_t seconds = readRegister( PT7C4339_REG_SECONDS );
  seconds = bcdToDec( seconds & 0x7F );

  return seconds;

}

/**
 * @brief Retrieves the current minutes value from the PT7C4339 RTC.
 *
 * This function reads the minutes register from the PT7C4339 real-time clock (RTC),
 * masks out the unused bit, converts the
 * value from BCD to decimal, and returns the result.
 *
 * @return uint8_t The current minutes (0-59).
 */
uint8_t PT7C4339::getMinute()
{

  uint8_t minutes = readRegister( PT7C4339_REG_MINUTES );
  minutes = bcdToDec( minutes & 0x7F );

  return minutes;

}

/**
 * @brief Retrieves the current hour value from the PT7C4339 RTC.
 *
 * This function reads the hour register from the PT7C4339 real-time clock,
 * masks out the control and unused bits, converts the value from BCD to
 * decimal format, and returns the hour in 24-hour format.
 *
 * @return uint8_t The current hour (0-23).
 */
uint8_t PT7C4339::getHour()
{

  uint8_t hours = readRegister( PT7C4339_REG_HOURS );
  hours = bcdToDec( hours & 0x3F );

  return hours;

}

/**
 * @brief Retrieves the current day of the week from the PT7C4339 RTC.
 *
 * This function reads the day of the week register from the PT7C4339 real-time clock,
 * masks the relevant bits, and returns the value as a daysOfWeek enumeration.
 *
 * @return daysOfWeek The current day of the week as an enum value where 1 = MO .... 7 = SU.
 */
daysOfWeek PT7C4339::getWeekDay()
{

  uint8_t weekDay = readRegister( PT7C4339_REG_DAYS_OF_WEEK );
  weekDay = ( weekDay & 0x07 );

  return static_cast<daysOfWeek>( weekDay );

}

/**
 * @brief Retrieves the current day of the month from the PT7C4339 RTC.
 *
 * This function reads the date register from the PT7C4339 real-time clock,
 * masks the relevant bits, converts it to decimal, and returns it.
 *
 * @return uint8_t The current day of the month (1-31).
 */
uint8_t PT7C4339::getDay()
{

  uint8_t day = readRegister( PT7C4339_REG_DATES );
  day = bcdToDec( day & 0x3F );

  return day;

}

/**
 * @brief Retrieves the current month from the PT7C4339 RTC module.
 *
 * This function reads the month register from the PT7C4339 real-time clock,
 * masks out unused and control bits, converts the value from BCD to decimal, and returns it.
 *
 * @return uint8_t The current month (1-12).
 */
uint8_t PT7C4339::getMonth()
{

  uint8_t month = readRegister( PT7C4339_REG_MONTHS );
  month &= 0x1F;
  month = bcdToDec( month );

  return month;

}

/**
 * @brief Retrieves the current year from the PT7C4339 RTC device.
 *
 * This function reads the year and month registers from the PT7C4339 real-time clock (RTC),
 * converts the year from BCD to decimal, and determines the century based on the highest bit
 * of the month register, which is the century bit.
 *
 * @return uint16_t The full year (e.g., 2024).
 */
uint16_t PT7C4339::getYear()
{

  uint16_t year = readRegister( PT7C4339_REG_YEARS );
  uint8_t month = readRegister( PT7C4339_REG_MONTHS );
  year = bcdToDec( year );

  if( month & 0x80 ) year += 2000;
  else year += 1900;

  return year;

}

bool PT7C4339::setSecond( uint8_t seconds )
{

  bool setSuccess;

  if( seconds < 60 )
  {

    seconds = decToBcd( seconds );

    if( writeRegister( PT7C4339_REG_SECONDS, seconds ) ) setSuccess = true;
    else setSuccess = false;

  }
  else
  {

    setSuccess = false;

  }

  return setSuccess;

}

bool PT7C4339::setMinute( uint8_t minutes )
{

  bool setSuccess;

  if( minutes < 60 )
  {

    minutes = decToBcd( minutes );

    if( writeRegister( PT7C4339_REG_MINUTES, minutes ) ) setSuccess = true;
    else setSuccess = false;

  }
  else
  {

    setSuccess = false;

  }

  return setSuccess;

}

bool PT7C4339::setHour( uint8_t hours )
{

  bool setSuccess;

  if( hours < 24 )
  {

    hours = decToBcd( hours );

    if( writeRegister( PT7C4339_REG_HOURS, hours ) ) setSuccess = true;
    else setSuccess = false;

  }
  else
  {

    setSuccess = false;

  }

  return setSuccess;

}

bool PT7C4339::setCorrectWeekDay()
{

  bool setSuccess;

  daysOfWeek calculatedWeekDay = calculateWeekDay( getYear(), getMonth(), getDay() );

  if( writeRegister( PT7C4339_REG_DAYS_OF_WEEK, calculatedWeekDay ) ) setSuccess = true;
  else setSuccess = false;

  return setSuccess;

}

bool PT7C4339::setDay( uint8_t day )
{

  bool setSuccess;

  uint8_t monthLength;

  PT7C4339_Date oldDate = getDate();

  bool isLeapYear = ( oldDate.year % 4 == 0 && ( oldDate.year % 100 != 0 || oldDate.year % 400 == 0 ) );

  switch( oldDate.month )
  {

    case 1:
      monthLength = 31;
      break;

    case 2:
      if( !isLeapYear ) monthLength = 28;
      else monthLength = 29;
      break;

    case 3:
      monthLength = 31;
      break;

    case 4:
      monthLength = 30;
      break;

    case 5:
      monthLength = 31;
      break;

    case 6:
      monthLength = 30;
      break;

    case 7:
      monthLength = 31;
      break;

    case 8:
      monthLength = 31;
      break;

    case 9:
      monthLength = 30;
      break;

    case 10:
      monthLength = 31;
      break;

    case 11:
      monthLength = 30;
      break;

    case 12:
      monthLength = 31;
      break;
  
  }

  if( day <= monthLength && day > 0 )
  {

    day = decToBcd( day );
    uint8_t oldDay = decToBcd( oldDate.day );

    if( writeRegister( PT7C4339_REG_DATES, day ) && setCorrectWeekDay() ) setSuccess = true;
    else
    {
    
      setSuccess = false;
      writeRegister( PT7C4339_REG_DATES, oldDay );
      setCorrectWeekDay();

    }

  }
  else setSuccess = false;

  return setSuccess;

}

bool PT7C4339::setMonth( uint8_t month )
{

  bool setSuccess;

  uint8_t oldMonth = getMonth();

  if( month <= 12 && month > 0 )
  {
    
    bool is2000 = getYear() > 1999;

    month = ( is2000 << 7 ) | decToBcd( month );
    oldMonth = ( is2000 << 7 ) | decToBcd( oldMonth );

    if( writeRegister( PT7C4339_REG_MONTHS, month ) && setCorrectWeekDay() ) setSuccess = true;
    else
    {
    
      setSuccess = false;
      writeRegister( PT7C4339_REG_MONTHS, oldMonth );
      setCorrectWeekDay();

    }

  }
  else setSuccess = false;

  return setSuccess;

}

bool PT7C4339::setYear( uint16_t year )
{

  bool setSuccess;

  uint8_t oldYear = getYear();

  uint8_t oldMonth = getMonth();
  uint8_t newMonth = decToBcd( oldMonth );
  newMonth &= 0x7F;
  oldMonth = newMonth;

  

  if( year > 1899 && year < 2100 )
  {

    if( year > 1999 )
    {

      newMonth |= 0x80;
      year -= 2000;
    
    }
    else year -= 1900;

    if( oldYear > 1999 )
    {

      oldMonth |= 0x80;
      oldYear -= 2000;
    
    }
    else oldYear -= 1900;

    year = decToBcd( year );
    oldYear = decToBcd( oldYear );
    
    if( writeRegister( PT7C4339_REG_YEARS, year ) )
    {

      if( writeRegister( PT7C4339_REG_MONTHS, newMonth ) && setCorrectWeekDay() ) setSuccess = true;
      else
      {

        setSuccess = false;
        writeRegister( PT7C4339_REG_MONTHS, oldMonth );
        writeRegister( PT7C4339_REG_YEARS, oldYear );

      }

    }
    else
    {

      setSuccess = false;
      writeRegister( PT7C4339_REG_YEARS, oldYear );
      setCorrectWeekDay();

    }

  }
  else setSuccess = false;
  
  return setSuccess;

}

bool PT7C4339::isOscillatorEnabled()
{

  return !readBit( PT7C4339_REG_CONTROL, 7 );

}

bool PT7C4339::enableOscillator( bool enable )
{

  return writeBit( PT7C4339_REG_CONTROL, 7, !enable );

}

bool PT7C4339::isIntFromBatteryEnabled()
{

  return readBit( PT7C4339_REG_CONTROL, 5 );

}

bool PT7C4339::enableIntFromBattery( bool enable )
{

  return writeBit( PT7C4339_REG_CONTROL, 5, enable );

}

sqwFrequency PT7C4339::getSqwFrequency()
{

  sqwFrequency freq = static_cast<sqwFrequency>( ( readRegister( PT7C4339_REG_CONTROL ) >> 3 ) & 0x03 );

  return freq;

}

bool PT7C4339::setSqwFrequency( sqwFrequency frequency )
{

  bool setSuccess;

  uint8_t buf = readRegister( PT7C4339_REG_CONTROL );
  buf &= 0xE7;

  if( writeRegister( PT7C4339_REG_CONTROL, buf | ( frequency << 3 ) ) ) setSuccess = true;
  else setSuccess = false;

  return setSuccess;

}

bool PT7C4339::getRtcStoppedFlag()
{

  return readBit( PT7C4339_REG_STATUS, 7 );

}

bool PT7C4339::clearRtcStoppedFlag()
{

  return writeBit( PT7C4339_REG_STATUS, 7, false );

}

/**
 * @brief Checks the status of the INTCN flag in the control register.
 *
 * This function reads bit 2 of the PT7C4339 control register to determine
 * whether the output is configured for Interrupt/alarm or squarewave mode.
 *
 * @return bool True if the output is set to Interrupt/alarm mode, false if set to Squarewave mode.
 */
bool PT7C4339::getIntOrSqwFlag()
{

  return readBit( PT7C4339_REG_CONTROL, 2 );

}

/**
 * @brief Sets the INTCN flag in the PT7C4339 control register.
 *
 * This function configures the INT/SQW (Interrupt or Squarewave) output mode.
 *
 * @param setting true to enable interrupt/alarm mode, false for squarewave mode.
 * @return bool True if the operation was successful, false otherwise.
 */
bool PT7C4339::setIntOrSqwFlag( bool setting )
{

  return writeBit( PT7C4339_REG_CONTROL, 2, setting );

}

trickleChargerEnabled PT7C4339::getTrickleChargerEnabled()
{

  trickleChargerEnabled enabled = static_cast<trickleChargerEnabled>( ( readRegister( PT7C4339_REG_TRICKLE_CHG ) >> 4 ) & 0x0F );

  if( ( enabled != TRICKLE_DISABLE ) && ( enabled != TRICKLE_ENABLE ) ) enabled = TRICKLE_DISABLE;

  return enabled;

}

trickleChargerDiode PT7C4339::getTrickleChargerDiode()
{

  trickleChargerDiode diode = static_cast<trickleChargerDiode>( ( readRegister( PT7C4339_REG_TRICKLE_CHG ) >> 2 ) & 0x03 );

  if( ( diode != DIODE_DISABLE ) && ( diode != DIODE_ENABLE ) ) diode = DIODE_DISABLE;

  return diode;

}

trickleChargerResistor PT7C4339::getTrickleChargerResistor()
{

  trickleChargerResistor resistor = static_cast<trickleChargerResistor>( readRegister( PT7C4339_REG_TRICKLE_CHG ) & 0x03 );

  return resistor;

}

bool PT7C4339::setTrickleChargerConfig( trickleChargerEnabled enable, trickleChargerDiode diode, trickleChargerResistor resistor )
{

  bool setSuccess;

  if( writeRegister( PT7C4339_REG_TRICKLE_CHG, ( enable << 4 ) | ( diode << 2 ) | resistor ) ) setSuccess = true;
  else setSuccess = false;

  return setSuccess;

}

/**
 * @brief Resets all PT7C4339 RTC registers to their first power-on state.
 *
 * This function writes the default values to all registers of the PT7C4339 RTC.
 *
 * @return true if all register writes succeed, false if any write fails.
 */
bool PT7C4339::reset()
{

  bool secondsReset = writeRegister( PT7C4339_REG_SECONDS, 0x00 );
  bool minutesReset = writeRegister( PT7C4339_REG_MINUTES, 0x00 );
  bool hoursReset = writeRegister( PT7C4339_REG_HOURS, 0x00 );

  bool weekDayReset = writeRegister( PT7C4339_REG_DAYS_OF_WEEK, 0x01 );
  bool daysReset = writeRegister( PT7C4339_REG_DATES, 0x01 );
  bool monthsReset = writeRegister( PT7C4339_REG_MONTHS, 0x81 );
  bool yearsReset = writeRegister( PT7C4339_REG_YEARS, 0x00 );

  bool alarm1SecondsReset = writeRegister( PT7C4339_REG_A1_SECONDS, 0x00 );
  bool alarm1MinutesReset = writeRegister( PT7C4339_REG_A1_MINUTES, 0x00 );
  bool alarm1HoursReset = writeRegister( PT7C4339_REG_A1_HOURS, 0x00 );
  bool alarm1DayDateReset = writeRegister( PT7C4339_REG_A1_DAY_DATE, 0x00 );

  bool alarm2MinutesReset = writeRegister( PT7C4339_REG_A2_MINUTES, 0x00 );
  bool alarm2HoursReset = writeRegister( PT7C4339_REG_A2_HOURS, 0x00 );
  bool alarm2DayDateReset = writeRegister( PT7C4339_REG_A2_DAY_DATE, 0x00 );

  bool controlReset = writeRegister( PT7C4339_REG_CONTROL, 0x18 );
  bool statusReset = writeRegister( PT7C4339_REG_STATUS, 0x80 );
  bool trickleChargerReset = writeRegister( PT7C4339_REG_TRICKLE_CHG, 0x00 );

  return ( secondsReset && minutesReset && hoursReset && weekDayReset && daysReset && monthsReset
    && yearsReset && alarm1SecondsReset && alarm1MinutesReset && alarm1HoursReset && alarm1DayDateReset
    && alarm2MinutesReset && alarm2HoursReset && alarm2DayDateReset && controlReset && statusReset && trickleChargerReset );

}