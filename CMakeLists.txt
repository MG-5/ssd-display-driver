cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(ssd-display-driver LANGUAGES CXX C ASM)

add_subdirectory(display-renderer)

add_library(${PROJECT_NAME} STATIC
        src/SSD1305.cxx
        src/SSD1306.cxx
        src/SSD1675a.cxx
        src/SSD1680.cxx
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        .
        )

target_link_libraries(${PROJECT_NAME}
        display-renderer)
