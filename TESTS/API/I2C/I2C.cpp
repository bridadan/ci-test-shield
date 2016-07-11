#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity.h"
#include "utest.h"
#include "rtos.h"
#include "TMP102.h"
#include <I2CEeprom.h>

using namespace utest::v1;

// TODO: take both A0 and 0x90 and put into config file
I2CEeprom memory(I2C_SDA,I2C_SCL, 0xA0, 32, 0);
TMP102 temperature(I2C_SDA, I2C_SCL, 0x90); //A0 pin is connected to ground

const char *convBool(bool value) { return value ? "true" : "false"; }

// a test to see if the temperature can be read. A I2C failure returns a 0
void test_tmp102(){
	TEST_ASSERT(0 != temperature.read());
}

// A test to write the EEprom
void test_eeprom_W(){
	TEST_ASSERT(memory.write(1, 'Z') == 1);
}


// A test to read the EEprom
void test_eeprom_R(){
	char value;
    if (memory.read(1, value) == 0) {
        printf("******\nRead failed\n*******\n");
        TEST_ASSERT(false);
    } else {
        printf("******\nValue read [%c] valid %s\n*******\n", value, convBool(value == 'Z'));
        TEST_ASSERT(true);
    }
}

// A test to Write then read the eeprom
// void test_eeprom_WR(){

// }

utest::v1::status_t test_setup(const size_t number_of_cases) {
    // Setup Greentea using a reasonable timeout in seconds
    GREENTEA_SETUP(40, "default_auto");
    return verbose_test_setup_handler(number_of_cases);
}

// Test cases
Case cases[] = {
    Case("Testing TMP102 Temperature Read",test_tmp102),
	Case("Testing EEProm W",test_eeprom_W),
	Case("Testing EEProm R",test_eeprom_R),
//	Case("Testing EEProm WR",test_eeprom_WR),
};

Specification specification(test_setup, cases);

// Entry point into the tests
int main() {
    return !Harness::run(specification);
}