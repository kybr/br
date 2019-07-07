cmake_minimum_required(VERSION 3.5.1)
project(br)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#add_library(${PROJECT_NAME} SHARED br.c)
add_library(${PROJECT_NAME} STATIC br.c)
#add_library(${PROJECT_NAME} STATIC br.h br.c)
target_include_directories(${PROJECT_NAME} PRIVATE .)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER br.h)

#set(CMAKE_INSTALL_PREFIX .) # no
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install")
#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
#set(CMAKE_INSTALL_PREFIX ${CMAKE_HOME_DIRECTORY})
install(TARGETS ${PROJECT_NAME})







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

#aux_source_directory(source/macos library_source)
#set(library_library ${library_library} ${core_services})
#add_library(${PROJECT_NAME} SHARED ${library_source})
#target_link_libraries(${PROJECT_NAME} ${library_library})
#set(libraries ${PROJECT_NAME} ${libraries})
#aux_source_directory(test source_list)
#aux_source_directory(example source_list)
#aux_source_directory(playground source_list)
#foreach(source_file ${source_list})
#  string(REGEX REPLACE "/" "_" target ${source_file})
#  string(REGEX REPLACE ".cpp" "" target ${target})
#  add_executable(${target} ${source_file})
#  target_link_libraries(${target} ${libraries})
#endforeach(source_file)

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
