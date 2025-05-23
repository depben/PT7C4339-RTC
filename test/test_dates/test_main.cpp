#include <unity.h>
#include <Arduino.h>
#include "PT7C4339-RTC.h"

#define SDA_PIN 47 //Change this to the correct pin number for your board or disregard it if you are using the default SDA pin
#define SCL_PIN 48 //Change this to the correct pin number for your board or disregard it if you are using the default SCL pin

PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );

void test_dates_valid()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalDates = 73049;

    for( uint16_t year = 1900; year <= 2099; ++year )
    {

        for( uint8_t month = 1; month <= 12; ++month )
        {

            uint8_t daysInMonth;
            if( month == 2 )
            {
            
                bool isLeap = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
                daysInMonth = isLeap ? 29 : 28;

            }
            else if( month == 4 || month == 6 || month == 9 || month == 11 ) daysInMonth = 30;    
            else daysInMonth = 31;
                
            for( uint8_t day = 1; day <= daysInMonth; ++day )
            {
            
                PT7C4339_Date setDate = { year, month, day, PT7C4339_WEEKDAY_UNKNOWN };
                bool setSuccess = rtc.setDate( setDate );
                PT7C4339_daysOfWeek currentWeekday = rtc.getWeekDay();
                static PT7C4339_daysOfWeek lastWeekday = PT7C4339_WEEKDAY_UNKNOWN;

                bool firstDate = setDate.year == 1900 && setDate.month == 1 && setDate.day == 1;
                bool correctWeekday = false;
                
                if( firstDate ) correctWeekday = true;
                else correctWeekday = ( ( lastWeekday + 1 == currentWeekday ) || ( currentWeekday == PT7C4339_MONDAY && lastWeekday == PT7C4339_SUNDAY ) ) && currentWeekday != PT7C4339_WEEKDAY_UNKNOWN;
                
                lastWeekday = currentWeekday;
                PT7C4339_Date getDate = rtc.getDate();

                bool dateOk = setSuccess && getDate.year == setDate.year && getDate.month == setDate.month && getDate.day == setDate.day && correctWeekday;
                if( dateOk ) success++;
                else fail++;

            }

        }

    }

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Some date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void test_dates_all()
{
    
    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalValidDates = 73049;
    int32_t totalDates = 200 * 12 * 31;

    for( uint16_t year = 1900; year <= 2099; ++year )
    {
    
        for( uint8_t month = 1; month <= 12; ++month )
        {

            for( uint8_t day = 1; day <= 31; ++day )
            {
            
                PT7C4339_Date setDate = { year, month, day, PT7C4339_WEEKDAY_UNKNOWN };
                bool setSuccess = rtc.setDate( setDate );
                PT7C4339_Date getDate = rtc.getDate();

                bool dateOk = setSuccess && getDate.year == setDate.year && getDate.month == setDate.month && getDate.day == setDate.day;
                if( dateOk ) success++;
                else fail++;

            }

        }

    }

    int32_t acceptedInvalidDates = totalDates - totalValidDates - fail;
    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, acceptedInvalidDates, "Incorrect number of all date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void test_dates_valid_backwards()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalDates = 73049;

    for( uint16_t year = 2099; year >= 1900; year-- )
    {

        for( uint8_t month = 12; month >= 1; month-- )
        {
            
            uint8_t daysInMonth;

            if( month == 2 )
            {

                bool isLeap = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
                daysInMonth = isLeap ? 29 : 28;
            
            }
            else if( month == 4 || month == 6 || month == 9 || month == 11 ) daysInMonth = 30;    
            else daysInMonth = 31;
                
            for( uint8_t day = daysInMonth; day >= 1; day-- )
            {
            
                PT7C4339_Date setDate = { year, month, day, PT7C4339_WEEKDAY_UNKNOWN };
                bool setSuccess = rtc.setDate( setDate );
                
                PT7C4339_daysOfWeek currentWeekday = rtc.getWeekDay();
                static PT7C4339_daysOfWeek lastWeekday = PT7C4339_WEEKDAY_UNKNOWN;
                bool firstDate = setDate.year == 2099 && setDate.month == 12 && setDate.day == 31;
                
                bool correctWeekday = false;
                if( firstDate ) correctWeekday = true;
                else correctWeekday = ( ( lastWeekday - 1 == currentWeekday ) || ( currentWeekday == PT7C4339_SUNDAY && lastWeekday == PT7C4339_MONDAY ) ) && currentWeekday != PT7C4339_WEEKDAY_UNKNOWN;

                lastWeekday = currentWeekday;
                PT7C4339_Date getDate = rtc.getDate();

                bool dateOk = setSuccess && getDate.year == setDate.year && getDate.month == setDate.month && getDate.day == setDate.day && correctWeekday;
                if( dateOk ) success++;
                else fail++;

            }

        }

    }

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Some backwards date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void test_dates_all_backwards()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalValidDates = 73049;
    int32_t totalDates = 200 * 12 * 31;

    for( uint16_t year = 2099; year >= 1900; year-- )
    {
    
        for( uint8_t month = 12; month >= 1; month-- )
        {
        
            for( uint8_t day = 31; day >= 1; day-- )
            {

                uint8_t daysInMonth;
                if( month == 2 )
                {
                
                    bool isLeap = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
                    daysInMonth = isLeap ? 29 : 28;

                }
                else if( month == 4 || month == 6 || month == 9 || month == 11 ) daysInMonth = 30;
                else daysInMonth = 31;
            
                PT7C4339_Date setDate = { year, month, day, PT7C4339_WEEKDAY_UNKNOWN };
                bool setSuccess = rtc.setDate( setDate );
                PT7C4339_Date getDate = rtc.getDate();

                bool dateOk = setSuccess && getDate.year == setDate.year && getDate.month == setDate.month && getDate.day == setDate.day;
                if( dateOk ) success++;
                else fail++;

            }

        }

    }

    int32_t acceptedInvalidDates = totalDates - totalValidDates - fail;
    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, acceptedInvalidDates, "Incorrect number of backwards all date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void test_dates_valid_random()
{
    
    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalDates = 20000;

    for( uint16_t i = 1; i <= totalDates; i++ )
    {
    
        uint16_t year = random( 1900, 2100 );
        uint8_t month = random( 1, 13 );
        
        uint8_t daysInMonth;
            if( month == 2 )
            {
            
                bool isLeap = ( year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 ) );
                daysInMonth = isLeap ? 29 : 28;

            }
            else if( month == 4 || month == 6 || month == 9 || month == 11 ) daysInMonth = 30;
            else daysInMonth = 31;

        uint8_t day = random( 1, daysInMonth + 1 );
        
        PT7C4339_Date setDate = { year, month, day, PT7C4339_WEEKDAY_UNKNOWN };
        bool setSuccess = rtc.setDate( setDate );
        PT7C4339_Date getDate = rtc.getDate();
        
        bool dateOk = setSuccess && getDate.year == setDate.year && getDate.month == setDate.month && getDate.day == setDate.day;
        if( dateOk ) success++;
        else fail++;

    }

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Incorrect number of random date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void test_dates_reset()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t totalDates = 5000;

    for( uint16_t i = 1; i <= totalDates; i++ )
    {
    
        PT7C4339_Date validSetDate = { 2000, 1, 1, PT7C4339_WEEKDAY_UNKNOWN };
        rtc.setDate( validSetDate );

        uint8_t months[] = { 2, 4, 6, 9, 11 };
        uint8_t month = months[random( 0, 5 )];
        uint16_t year = random( 1900, 2100 );

        PT7C4339_Date setDate = { year, month, 31, PT7C4339_WEEKDAY_UNKNOWN };

        PT7C4339_Date getDate = rtc.getDate();

        if( validSetDate.year == getDate.year && validSetDate.month == getDate.month && validSetDate.day == getDate.day && validSetDate.weekDay == getDate.weekDay )
            success++;
        else fail++;

    }

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Some reset date tests failed" );
    TEST_ASSERT_EQUAL_INT32( totalDates, success + fail );

}

void setUp( void )
{
    
    rtc.reset();
    rtc.begin();

}

void tearDown( void )
{

    rtc.reset();

}

void setup()
{

    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_dates_valid );
    RUN_TEST( test_dates_all );
    RUN_TEST( test_dates_valid_backwards );
    RUN_TEST( test_dates_all_backwards );
    RUN_TEST( test_dates_valid_random );
    RUN_TEST( test_dates_reset );
    
    UNITY_END();

}

void loop()
{

    

}