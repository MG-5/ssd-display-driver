cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(oled-driver LANGUAGES CXX C ASM)

add_library(${PROJECT_NAME} STATIC
        src/Image.cxx
        src/Renderer.cxx
        src/SSD1305.cxx
        src/SSD1306.cxx
        fonts/mono.cxx
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        .
        )

target_link_libraries(${PROJECT_NAME})
