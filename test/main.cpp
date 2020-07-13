#include "unity.h"
#include "pixled_esp.hpp"

#include "test_esp_output.hpp"

extern "C" void app_main() {
	UNITY_BEGIN();

	printf("\n> Running pixled-esp test suite\n\n");

	RUN_TEST(test_esp_output);

	UNITY_END();
}
