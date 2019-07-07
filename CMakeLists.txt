cmake_minimum_required(VERSION 3.5.1)
project(br)

add_library(${PROJECT_NAME} STATIC br.c)
target_include_directories(${PROJECT_NAME} PRIVATE .)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "${PROJECT_NAME} is the project name")


#...

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message(STATUS "XXXXXx we got Linux xXXXXXX")
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  message(STATUS "XXXXXx we got Darwin xXXXXXX")
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message(STATUS "XXXXXx we got Windows xXXXXXX")
endif()

message(STATUS "${CMAKE_SYSTEM_NAME} is the system name")

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
