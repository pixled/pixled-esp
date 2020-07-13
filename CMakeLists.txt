set(PIXLED_ESP_TEST OFF CACHE BOOL "Set to TRUE to compile this
component as a standalone esp-idf project and set up tests.")

if(PIXLED_ESP_TEST)
	cmake_minimum_required(VERSION 3.5)
	set(CXX_STANDARD 11)

	message("Building pixled-esp standalone tests.")

	set(EXTRA_COMPONENT_DIRS test /home/pbreugno/Development/esp/components)

	include($ENV{IDF_PATH}/tools/cmake/project.cmake)

	project(pixled-esp-test CPP)

	set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
else()
	idf_component_register(
		SRCS "pixled_esp.cpp"
		INCLUDE_DIRS "."
		REQUIRES pixled-lib esp-pixled-driver)
endif()
