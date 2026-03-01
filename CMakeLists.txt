cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(password_manager LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
)
FetchContent_MakeAvailable(googletest)


FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.12.0
)


FetchContent_MakeAvailable(json)


# add_subdirectory(dependencies/FTXUI)
# add_subdirectory(dependencies/json)

#make sure be got pkgconfig
find_package(PkgConfig REQUIRED)
#look for packages named libsodium and perfix them wiht SODIUM
pkg_check_modules(SODIUM REQUIRED libsodium)


add_executable(password_manager
    src/main.cpp

    src/secure_buffer.h
    src/secure_buffer.cpp

    src/crypto_engine.h
    src/crypto_engine.cpp

    src/file_manager.h
    src/file_manager.cpp

    src/converter.h
    src/converter.cpp

    src/exception.h    
    src/constants.h

    src/vault_serializer.h
    src/vault_serializer.cpp

    src/password_entry.h
    src/password_entry.cpp
)
target_compile_features(password_manager PRIVATE cxx_std_20)

# use those prefixed SODIUM Varibale
target_include_directories(password_manager
    PRIVATE
        ${SODIUM_INLUDE_DIRS}
)


target_link_libraries(password_manager
    PRIVATE
        -fsanitize=address
        # ftxui::screen
        # ftxui::dom
        # ftxui::component
        ${SODIUM_LIBRARIES}
        nlohmann_json::nlohmann_json
        GTest::gtest_main
)


# enable_testing()


# add_executable(unit_tests
#     tests/test_example.cpp
# )

# target_link_libraries(unit_tests
#     PRIVATE
#         gtest
#         gtest_main
# )

# add_test(NAME AllUnitTests COMMAND unit_tests)
