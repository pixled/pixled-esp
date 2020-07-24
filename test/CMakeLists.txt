idf_component_register(
	SRC_DIRS ".." "."
	#"main.cpp" "../pixled_esp.cpp" "test_esp_output.cpp"
	INCLUDE_DIRS ".." "."
	REQUIRES unity pixled-lib esp-pixled-driver
	#REQUIRES unity esp-pixled-driver
	)
