cmake_minimum_required(VERSION 3.22)
project(GCCExplorer) #LANGUAGE CXX)
set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Sources
file(GLOB_RECURSE SOURCES source/*.cpp source/*.c)
file(GLOB_RECURSE HEADERS include/*.hpp include/*.h)
file(GLOB_RECURSE TEST_SOURCES tests/*.cpp tests/*.c)

add_executable(ge ${SOURCES})
target_include_directories(ge PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(ge PUBLIC ${CMAKE_SOURCE_DIR}/include/third_party)
target_link_libraries(ge PUBLIC ${CONAN_LIBS})
set_property(TARGET ge PROPERTY CXX_STANDARD 23)

add_executable(tests ${TEST_SOURCES})
target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(tests PUBLIC ${CMAKE_SOURCE_DIR}/include/third_party)
set_property(TARGET tests PROPERTY CXX_STANDARD 23)
target_link_libraries(tests PUBLIC ${CONAN_LIBS})

