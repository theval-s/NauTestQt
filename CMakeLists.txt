cmake_minimum_required(VERSION 3.31)
project(NauTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(NauTest main.cpp)
target_include_directories(NauTest PRIVATE include)
target_link_libraries(NauTest
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

