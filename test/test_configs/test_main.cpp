#include <Arduino.h>
#include <unity.h>
#include "PT7C4339-RTC.h"

PT7C4339 rtc( &Wire, 47, 48 );

void test_trickleCharger()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t total = 0;
    

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Test failed" );
    TEST_ASSERT_EQUAL_INT32( total, success + fail );

}

void setUp( void )
{

    rtc.begin();
    rtc.reset();

}

void tearDown(void)
{

    rtc.reset();

}

void setup() 
{

    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_trickleCharger );
    
    UNITY_END();

}

void loop()
{



}