PROJECT(i210-flash)
cmake_minimum_required(VERSION 3.1)

SET(CMAKE_CXX_STANDARD 11)

#SET(Boost_USE_STATIC_LIBS ON)
#SET(Boost_USE_MULTITHREADED OFF)
FIND_PACKAGE(Threads)
FIND_PACKAGE(Boost 1.55 COMPONENTS system filesystem program_options REQUIRED)

INCLUDE_DIRECTORIES("../libi210/include" "../libi210/libregmap/include")

FILE(GLOB i210_FLASH_SOURCES "src/*.cpp")
ADD_EXECUTABLE(i210-flash ${i210_FLASH_SOURCES})
TARGET_LINK_LIBRARIES(i210-flash i210-static ${Boost_LIBRARIES})
