cmake_minimum_required(VERSION 3.5.1)
project(br)


if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
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

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
