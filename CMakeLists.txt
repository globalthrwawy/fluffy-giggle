cmake_minimum_required(VERSION 3.8)
project(snakev1)

#GLOBAL SETTINGS
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -v -Werror -Wextra")


#allows current directory to be source
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin/) #move assets into bin dir

set(SOURCE_FILES main.cpp)
add_executable(snakev1 ${SOURCE_FILES})

#Modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/build/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    message( ${SFML_INCLUDE_DIR} )
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${SFML_LIBRARIES})
endif()