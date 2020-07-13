idf_component_register(
	SRCS "main.cpp" "../pixled_esp.cpp" "test_esp_output.cpp"
	INCLUDE_DIRS ".." "."
	REQUIRES unity pixled-lib esp-pixled-driver
	)
