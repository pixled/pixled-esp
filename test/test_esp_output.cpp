#include "test_esp_output.hpp"
#include "unity.h"
#include "pixled_esp.hpp"
#include "pixled/pixel.h"

void test_esp_output() {
	pixled::RgbStrip strip(GPIO_NUM_0, 2, {RGB, WS2812_T0H, WS2812_T0L, WS2812_T1H, WS2812_T1L});
	pixled::EspOutput output(strip);

	pixled::Color c1;
	c1.setRgb(200, 55, 12);
	pixled::Color c2;
	c2.setRgb(100, 155, 212);

	output.write(c1, 0);
	output.write(c2, 1);

	uint8_t* const buffer = strip.buffer();
	TEST_ASSERT_EQUAL_UINT8(buffer[0], 200);
	TEST_ASSERT_EQUAL_UINT8(buffer[1], 55);
	TEST_ASSERT_EQUAL_UINT8(buffer[2], 12);

	TEST_ASSERT_EQUAL_UINT8(buffer[3], 100);
	TEST_ASSERT_EQUAL_UINT8(buffer[4], 155);
	TEST_ASSERT_EQUAL_UINT8(buffer[5], 212);
}
