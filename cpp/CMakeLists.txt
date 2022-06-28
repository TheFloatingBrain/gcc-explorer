project(GCCExplorer) #LANGUAGE CXX)
set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(COMPILE_FLAGS "-g -std=c++20")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Sources
file(GLOB_RECURSE SOURCES source/*.cpp source/*.c)
file(GLOB_RECURSE HEADERS include/*.hpp include/*.h)

add_executable(ge ${SOURCES})
target_include_directories(ge PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(ge PUBLIC ${CONAN_LIBS})

# Test Sources
# file(GLOB_RECURSE TEST_SOURCES test/*.cpp)
# 
# add_executable(test ${SOURCES} ${TEST_SOURCES})
# target_include_directories(test PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(test PUBLIC ${CONAN_LIBS})
