cmake_minimum_required(VERSION 3.10)

project(infoexchangerproject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)

if(UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries/lib)
elseif(WIN32)
    set(TEST_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY   ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/binaries)
endif()

set(UNITTEST ON)

if(UNITTEST)
    add_subdirectory(${PROJECT_SOURCE_DIR}/googletests)
    enable_testing()
endif()

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/common/mylist)
target_link_libraries(${PROJECT_NAME} PUBLIC mylist)

add_subdirectory(${PROJECT_SOURCE_DIR}/common/msg)
target_link_libraries(${PROJECT_NAME} PUBLIC msg)

add_subdirectory(${PROJECT_SOURCE_DIR}/common/processing)
target_link_libraries(${PROJECT_NAME} PUBLIC processing)

add_subdirectory(${PROJECT_SOURCE_DIR}/datasource)
target_link_libraries(${PROJECT_NAME} PUBLIC datasource)

add_subdirectory(${PROJECT_SOURCE_DIR}/memorysupplier/imemorysupplier)
target_link_libraries(${PROJECT_NAME} PUBLIC imemorysupplier)











