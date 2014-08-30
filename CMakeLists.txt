# CMakeFile

# Do an out-of-source build (let us say debug build, with each file build
# command being echoed) as follows:
#
# $ cd $TOP; mkdir debug && cd debug 
# $ cmake -i -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=TRUE
# $ make

# Use -i if you want to change some settings in interactive mode (e.g. Debug,
# Minsize etc.). 

# Altenatively you can edit build/CMakeCache.txt once it is generated. Makefile
# generation needs to be done only once, unless you change the build mode.

# e.g. 
# CMAKE_BUILD_TYPE:STRING=Debug
# CMAKE_VERBOSE_MAKEFILE:BOOL=TRUE

# To generate makefile that runs unit tests do the following:
# $ cmake -i -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug -DUNIT_TEST=1

# ----- START -----

# Essential directive for CMake
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Provide the project name
PROJECT (TIMER_WHEEL)

# Base path for all includes
SET (TOP "./")

# Always go for 32 bit compilation (this is not expected to work as a 64 bit binary right now)
SET (CMAKE_C_FLAGS "-m32 -Wall")
SET (CMAKE_EXE_LINKER_FLAGS "-m32 -Wall -L/usr/lib")

# Definitions - common ones first
# ADD_DEFINITIONS (-DNDEBUG)
ADD_DEFINITIONS (-D_DEBUG)

# set(UNIT_TEST 1)
if (UNIT_TEST EQUAL 1)
# Enable this to run unit tests 
ADD_DEFINITIONS(-DUNIT_TEST)
endif ()

# Include paths 
INCLUDE_DIRECTORIES( ${TOP}/src
)

# Usually a project consists of multiple directories, each of which is a static
# or shared lib, all linked to one executable. This example makes a single
# executable from all files.

# Name of the executable, followed by all files that are to be built
ADD_EXECUTABLE(timer_wheel 
               ${TOP}/src/LinuxTimer.cpp
               ${TOP}/src/TransactionContext.cpp
               ${TOP}/src/TimerWheel.cpp
               ${TOP}/src/AsyncCall.cpp
               ${TOP}/src/Task.cpp
               ${TOP}/test/main.cpp
)

# link against necessary system libs as well
TARGET_LINK_LIBRARIES (timer_wheel pthread rt)

# eof
