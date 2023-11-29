CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(blur_background)
ENABLE_LANGUAGE(CXX)

FIND_PACKAGE(OpenCV	REQUIRED )
LINK_LIBRARIES(${OpenCV_LIBS})
include_directories ("${OpenCV_INCLUDE_DIRS}")

add_executable(blur_background blur_background.cpp 
    common_code.cpp common_code.hpp)

add_executable(blur_background_test_common_code test_common_code.cpp 
    common_code.cpp common_code.hpp)
set_target_properties(blur_background_test_common_code PROPERTIES OUTPUT_NAME "test_common_code")




 
