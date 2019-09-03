# *  @Copyright (c) 2019-2029 TJU
# *  Create @ 2019-01-17 by tao.jing
# *  cmake file of main

cmake_minimum_required(VERSION 2.8)
project(can_master)

# compile mex
set(C_FILE can-main)

# set up matlab libraries
set(C_FILE_NAME ${C_FILE}.c)

set(CTYPE Native) # Native or Arm

if(ARCH) # arm OR x86_64
  message(STATUS "Compilation Arch ${ARCH}")
  if(${ARCH} MATCHES x86_64 OR ${ARCH} MATCHES x86)
    set(CTYPE Native)
  elseif(${ARCH} MATCHES arm)
    set(CTYPE Arm)
  else()
    message(FATAL "Not support arch!")
  endif()
endif()

if(DEVICE)
  # User Set Can Device
else()
  set(DEVICE Socket)
endif()
  message(STATUS "Compilation Can Device ${DEVICE}")

if (${CTYPE} MATCHES Arm)
  set(TOOLS /home/russel25/tools/arm-linux-gcc-7.4)
  set(CAN_FESTIVAL_DIR /home/russel25/work/imx6/can-festival)
  set(CMAKE_C_COMPILER ${TOOLS}/bin/arm-linux-gnueabihf-gcc)
  set(CMAKE_CXX_COMPILER ${TOOLS}/bin/arm-linux-gnueabihf-g++)

  add_definitions(-DUSE_ARM)
elseif (${CTYPE} MATCHES Native)
  if (DEVICE MATCHES socket)
    set(CAN_FESTIVAL_DIR /usr/local)
  elseif (DEVICE MATCHES virtual)
    set(CAN_FESTIVAL_DIR /home/russel25/work/vcan)
  endif()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread -lrt -lrt")
endif()

message(STATUS "Use Compiler ${CMAKE_C_COMPILER}")

set(CMAKE_SYSTEM_NAME Linux)

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-variable -Wno-unused-but-set-variable" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lrt  -ldl -lpthread " )
# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -march=native -O3 -pthread" )

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/libs)

# Add module path for <name>.cmake conf files
set(CURRENT_CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
set(CMAKE_MODULE_PATH ${CURRENT_CMAKE_MODULE_PATH})

##################
## Include_Dirs ##
##################
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(master)
include_directories(slave)
include_directories(epos_motor)

include_directories(${CAN_FESTIVAL_DIR}/include/canfestival)
include_directories(canfestival/)
include_directories(canfestival/unix)
include_directories(canfestival/timers_unix)
include_directories(canfestival/can_socket)
#easy_logger
include_directories(easylogger/inc)
include_directories(easylogger/plugins)
INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR})

##########################
## Main Target Sources  ##
##########################
FILE(GLOB SOURCE_FILES *.h *.c
        ${CMAKE_CURRENT_SOURCE_DIR}/epos_motor/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/master/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/slave/*.c
        #easy_logger
        ${CMAKE_CURRENT_SOURCE_DIR}/easylogger/plugins/file/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/easylogger/port/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/easylogger/src/*.c
        )

################
## Link_Dirs  ##
################
# Lib for canfestival
link_directories(${CAN_FESTIVAL_DIR}/lib)
# Extra libs
if (ARCH MATCHES Native)
  if (DEVICE MATCHES socket)
    link_directories(/usr/lib/x86_64)
    link_directories(/usr/lib/x86_64-linux-gnu)
  endif()
endif()

###########################
## Main Target Specific  ##
###########################
# add_library(can_master SHARED ${SOURCE_FILES})
add_library(can_master SHARED ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/Matlabdef.def)

################
## Link Libs  ##
################
target_link_libraries(${PROJECT_NAME} PUBLIC libcanfestival_unix.a)
target_link_libraries(${PROJECT_NAME} PUBLIC libcanfestival.a)
target_link_libraries(${PROJECT_NAME} ${MATLAB_LIBRARIES})
# target_link_libraries(${PROJECT_NAME} PUBLIC libcanfestival_can_socket.so)
# target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/uw_lib/lib/uw_imx6.a)
if (${CTYPE} MATCHES Native)
  target_link_libraries(${PROJECT_NAME} PUBLIC dl)
  target_link_libraries(${PROJECT_NAME} PUBLIC rt)
  target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
endif()

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX .mexa64 PREFIX "")
# 32-bit or 64-bit mex
# if(WIN32)
#   if (CMAKE_CL_64)
#       SET_TARGET_PROPERTIES(${CPP_FILE} PROPERTIES SUFFIX .mexw64)
#   else(CMAKE_CL_64)
#       SET_TARGET_PROPERTIES(${CPP_FILE} PROPERTIES SUFFIX .mexw32)
#   endif(CMAKE_CL_64)
# else(WIN32)
#   if (CMAKE_SIZEOF_VOID_P MATCHES "8")
#       SET_TARGET_PROPERTIES(${CPP_FILE} PROPERTIES SUFFIX .mexa64 PREFIX "")
#   else(CMAKE_SIZEOF_VOID_P MATCHES "8")
#       SET_TARGET_PROPERTIES(${CPP_FILE} PROPERTIES SUFFIX .mexglx PREFIX "")
#   endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
# endif(WIN32)

# install to /bin by default
install(TARGETS ${PROJECT_NAME} DESTINATION ../bin)