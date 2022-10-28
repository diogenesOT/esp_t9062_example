# This is the project CMakeLists.txt file for the test subproject
cmake_minimum_required(VERSION 3.16)

set(EXTRA_COMPONENT_DIRS "../components")

set(TEST_COMPONENTS "esp_t9062" CACHE STRING "List of components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp_t9062_test)