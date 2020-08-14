project(unit_conv)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

qt5_wrap_ui(ui unit_conv.ui)
add_library(lib STATIC
    src/lib.cpp
    src/MainWindow.cpp
    src/Units.cpp

    ${ui}
)
# set_target_properties(lib PROPERTIES CXX_STANDARD 17)
target_link_libraries(lib Qt5::Widgets)
target_include_directories(lib PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif()

set(UNIT_FILE_SRC ${CMAKE_SOURCE_DIR}/res/units.txt)
set(UNIT_FILE_DST ${CMAKE_BINARY_DIR}/units.txt)
add_custom_command(OUTPUT ${UNIT_FILE_DST}
COMMAND ${CMAKE_COMMAND} -E copy_if_different ${UNIT_FILE_SRC} ${UNIT_FILE_DST}
DEPENDS ${UNIT_FILE_SRC}
)
add_custom_target(copy_unit_file SOURCES ${UNIT_FILE_DST})

add_executable(unit_conv WIN32
    src/main.cpp
)


target_link_libraries(unit_conv lib)
add_dependencies(unit_conv copy_unit_file)