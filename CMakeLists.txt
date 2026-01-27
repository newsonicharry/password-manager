cmake_minimum_required(VERSION 3.20)

project(password_manager LANGUAGES CXX)

add_subdirectory(FTXUI)

add_executable(password_manager
    src/main.cpp
)


target_compile_features(password_manager PRIVATE cxx_std_17)

target_link_libraries(password_manager
    PRIVATE
        ftxui::screen
        ftxui::dom
        ftxui::component
)
