cmake_minimum_required(VERSION 3.21.0)

cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)
add_definitions(-DNAPI_VERSION=8)
set(CMAKE_CXX_STANDARD 20)

project(my_addon CXX)

# Not work at all
# See: https://github.com/cmake-js/cmake-js/issues/42
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include_directories(${CMAKE_JS_INC})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()

message(STATUS "CMAKE_JS_INC: ${CMAKE_JS_INC}")
set(SRCS src/lib.cc)

add_library(${PROJECT_NAME} SHARED ${SRCS} ${CMAKE_JS_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE )
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})