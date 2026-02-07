cmake_minimum_required(VERSION 3.16)

project(bjobfileswidget_test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Executable
add_executable(${PROJECT_NAME}
    main.cpp
)

# Link Qt6
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Windows-specific settings
if(WIN32)
    # Enable console window for debugging
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
endif()

message(STATUS "TreeView Test Application configured")
message(STATUS "Build with: cmake --build . --target ${PROJECT_NAME}")
