cmake_minimum_required(VERSION 3.20)

project(password_manager LANGUAGES CXX)

# add_subdirectory(dependencies/FTXUI)

#make sure be got pkgconfig
find_package(PkgConfig REQUIRED)
#look for packages named libsodium and perfix them wiht SODIUM
pkg_check_modules(SODIUM REQUIRED libsodium)

add_executable(password_manager
    src/main.cpp
)

target_compile_features(password_manager PRIVATE cxx_std_17)

# use those prefixed SODIUM Varibale
target_include_directories(password_manager
    PRIVATE
        ${SODIUM_INLUDE_DIRS}
)

target_link_libraries(password_manager
    PRIVATE
        # ftxui::screen
        # ftxui::dom
        # ftxui::component
        ${SODIUM_LIBRARIES}
)
