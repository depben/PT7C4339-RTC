#include <unity.h>
#include "PT7C4339-RTC.h"

#define SDA_PIN 47 //Change this to the correct pin number for your board or disregard it if you are using the default SDA pin
#define SCL_PIN 48 //Change this to the correct pin number for your board or disregard it if you are using the default SCL pin

PT7C4339 rtc( &Wire, SDA_PIN, SCL_PIN );
PT7C4339 rtc2( &Wire1 );

void test_config_trickleCharger()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t total = 2 * 2 * 4;

    PT7C4339_trickleChargerEnabled wantEnable = PT7C4339_TRICKLE_DISABLE;
    PT7C4339_trickleChargerDiode wantDiode = PT7C4339_DIODE_DISABLE;
    PT7C4339_trickleChargerResistor wantResistor = PT7C4339_RESISTOR_DISABLE;
    bool setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_DISABLE;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_DISABLE;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_DISABLE;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_200R;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_200R;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_200R;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_200R;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_2K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_2K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_2K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_2K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_4K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_DISABLE;
    wantResistor = PT7C4339_RESISTOR_4K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_DISABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_4K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    wantEnable = PT7C4339_TRICKLE_ENABLE;
    wantDiode = PT7C4339_DIODE_ENABLE;
    wantResistor = PT7C4339_RESISTOR_4K;
    setSuccess = rtc.setTrickleChargerConfig( wantEnable, wantDiode, wantResistor );

    if( setSuccess && wantEnable == rtc.getTrickleChargerEnabled() && wantDiode == rtc.getTrickleChargerDiode() && wantResistor == rtc.getTrickleChargerResistor() ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Trickle charger test failed!" );
    TEST_ASSERT_EQUAL_INT32( total, success + fail );

}

/*
    bool isOscillatorEnabled();
    bool enableOscillator( bool enable );

    bool getRtcStopFlag();
    bool clearRtcStopFlag();
*/
void test_config_oscillator()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t total = 6;
    
    rtc.reset();
    if( rtc.getRtcStopFlag() ) success++;
    else fail++;

    rtc.clearRtcStopFlag();
    if( rtc.getRtcStopFlag() ) fail++;
    else success++;

    rtc.enableOscillator( false );
    if( rtc.isOscillatorEnabled() ) fail++;
    else success++;

    if ( rtc.getRtcStopFlag() ) success++;
    else fail++;

    rtc.enableOscillator( true );
    if ( rtc.getRtcStopFlag() ) success++;
    else fail++;

    rtc.clearRtcStopFlag();
    if( rtc.getRtcStopFlag() ) fail++;
    else success++;

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Oscillator test failed!" );
    TEST_ASSERT_EQUAL_INT32( total, success + fail );

}

void test_config_squarewave()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t total = 8;
    
    rtc.setIntOrSqwFlag( false );
    if( rtc.getIntOrSqwFlag() ) fail++;
    else success++;
    rtc.setIntOrSqwFlag( true );
    if( rtc.getIntOrSqwFlag() ) success++;
    else fail++;

    rtc.enableIntFromBattery( false );
    if( rtc.isIntFromBatteryEnabled() ) fail++;
    else success++;
    rtc.enableIntFromBattery( true );
    if( rtc.isIntFromBatteryEnabled() ) success++;
    else fail++;

    rtc.setSqwFrequency( PT7C4339_SQW_1HZ );
    if( rtc.getSqwFrequency() == PT7C4339_SQW_1HZ ) success++;
    else fail++;
    rtc.setSqwFrequency( PT7C4339_SQW_4_96KHZ );
    if( rtc.getSqwFrequency() == PT7C4339_SQW_4_96KHZ ) success++;
    else fail++;
    rtc.setSqwFrequency( PT7C4339_SQW_8_192KHZ );
    if( rtc.getSqwFrequency() == PT7C4339_SQW_8_192KHZ ) success++;
    else fail++;
    rtc.setSqwFrequency( PT7C4339_SQW_32_768KHZ );
    if( rtc.getSqwFrequency() == PT7C4339_SQW_32_768KHZ ) success++;
    else fail++;

    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Square wave test failed!" );
    TEST_ASSERT_EQUAL_INT32( total, success + fail );

}

void test_config_beginReset()
{

    int32_t success = 0;
    int32_t fail = 0;
    int32_t total = 7;

    uint8_t resetWorks;
    
    bool resetSuccess = rtc.reset();
    if ( resetSuccess ) success++;
    else fail++;

    if( resetSuccess && rtc.getSecond() == 0 && rtc.getMinute() == 0 && rtc.getHour() == 0
    && rtc.getDay() == 1 && rtc.getMonth() == 1 && rtc.getYear() == 2000 && rtc.getWeekDay() == PT7C4339_MONDAY
    && rtc.isOscillatorEnabled() == true && rtc.getRtcStopFlag() == true && rtc.isIntFromBatteryEnabled() == false
    && rtc.getIntOrSqwFlag() == false && rtc.getSqwFrequency() == PT7C4339_SQW_32_768KHZ
    && rtc.getTrickleChargerEnabled() == PT7C4339_TRICKLE_DISABLE && rtc.getTrickleChargerDiode() == PT7C4339_DIODE_DISABLE
    && rtc.getTrickleChargerResistor() == PT7C4339_RESISTOR_DISABLE )
    {
        success++;
        resetWorks = 1;
    }
    else
    {
        fail++;
        resetWorks = 0;
    }
    
    rtc.clearRtcStopFlag();
    
    uint8_t wireAlreadyCalledBeginSuccess = rtc.begin();
    if( wireAlreadyCalledBeginSuccess ) success++;
    else fail++;

    Wire.begin(SDA,SCL);
    uint8_t wireNotCalledBeginSuccess = rtc.begin();
    if( wireNotCalledBeginSuccess ) success++;
    else fail++;


    rtc.reset();

    uint8_t detectStopFlag = rtc.begin();
    Wire.begin(SDA,SCL);
    detectStopFlag += rtc.begin();
    if( detectStopFlag == 4 ) success++;
    else fail++;

    rtc.clearRtcStopFlag();

    Wire.end();
    Wire1.begin(SDA,SCL);
    uint8_t wrongAddressAttempt = rtc2.begin();
    if( wrongAddressAttempt == 0 ) success++;
    else fail++;

    Wire1.end();
    Wire1.begin( SDA_PIN, SCL_PIN );
    rtc2.clearRtcStopFlag();
    uint8_t onlyWireknows = rtc2.begin();
    if( onlyWireknows == 1 ) success++;
    else fail++;

    Wire1.end();
    Wire.begin(SDA_PIN, SCL_PIN);

    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 0, wrongAddressAttempt, "begin() falsely reported successful initialization when no I2C pins were given" );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 1, onlyWireknows, "Failed begin() when no Pin_numbers were given in the constructor, but Wire was already initialized!" );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 4, detectStopFlag, "Stop Flags not detected correctly by begin()" );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 1, wireNotCalledBeginSuccess, "Failed begin() when Wire was NOT already initialized!" );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 1, wireAlreadyCalledBeginSuccess, "Failed begin() when Wire was already initialized!" );
    TEST_ASSERT_EQUAL_UINT8_MESSAGE( 1, resetWorks, "Reset did not properly set default values!" );
    TEST_ASSERT_EQUAL_INT32_MESSAGE( 0, fail, "Begin and reset behaviour test failed!" );
    TEST_ASSERT_EQUAL_INT32( total, success + fail );

}

void setUp( void )
{

    rtc.reset();
    rtc.begin();

}

void tearDown(void)
{

    rtc.reset();

}

void setup() 
{

    delay( 2000 );
    
    UNITY_BEGIN();
    
    RUN_TEST( test_config_beginReset );
    RUN_TEST( test_config_trickleCharger );
    RUN_TEST( test_config_oscillator );
    RUN_TEST( test_config_squarewave );
    
    UNITY_END();

}

void loop()
{



}