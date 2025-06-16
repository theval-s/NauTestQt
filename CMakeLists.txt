cmake_minimum_required(VERSION 3.24)
project(NauTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Widgets
        REQUIRED)

add_executable(NauTest main.cpp
        src/viewport_widget.cpp
        include/viewport_widget.hpp
        include/main_window.hpp
        src/main_window.cpp
        src/hierarchy_widget.cpp
        include/hierarchy_widget.hpp
        src/inspector_widget.cpp
        include/inspector_widget.hpp
        src/saving_manager.cpp
        src/image.cpp)

target_include_directories(NauTest PUBLIC include)
target_link_libraries(NauTest PUBLIC
        Qt::Core
        Qt::Widgets
)
qt_finalize_executable(NauTest)

