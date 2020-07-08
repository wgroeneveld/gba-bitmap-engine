cmake_minimum_required(VERSION 3.12)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing -Wno-int-to-pointer-cast")
set(CMAKE_VERBOSE_MAKEFILE on)
#set_property(SOURCE ../engine/src/gba/sin_lut.s PROPERTY LANGUAGE ASM)
SET(ASM_OPTIONS "-x assembler-with-cpp")
SET(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

project(Unittest)
enable_language(C CXX ASM)
enable_testing()

SET(GTEST_LIBRARY "$ENV{GTEST_DIR}")

# reset linker flags; ARM + GTest doesn't work
SET(CMAKE_EXE_LINKER_FLAGS "${BASE_CMAKE_LINK_FLAGS}")
SET(CMAKE_ASM_COMPILER gcc)
SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

add_definitions(-DCODE_COMPILED_AS_PART_OF_TEST)

include_directories(${GTEST_LIBRARY}/include)
include_directories(../engine/include)

# compile including library source because it's cross-compiled
add_executable(unittest
        maintest.cpp
        gbatest.cpp
        palettetest.cpp
        real_data.h
        ../engine/src/palette/palette_manager.cpp
        ../engine/src/palette/combined_palette.cpp
        ../engine/src/timer.cpp
        ../engine/src/math.cpp
        ../engine/src/gbavector.cpp
        timertest.cpp gbavectortest.cpp gbamatrixtest.cpp fixedpoinmathtest.cpp)

target_link_libraries(unittest ${GTEST_LIBRARY}/build/libgtest.a ${GTEST_LIBRARY}/build/libgtest_main.a)
