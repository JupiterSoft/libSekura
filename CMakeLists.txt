cmake_minimum_required(VERSION 3.14)

project(libSekura LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(libSekura STATIC
  sekura.cpp
  sekura.h
)

target_link_libraries(libSekura PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(libSekura PRIVATE LIBSEKURA_LIBRARY)
