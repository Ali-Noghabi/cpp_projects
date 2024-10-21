cmake_minimum_required(VERSION 3.10)

project(FileSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/BlockGroup.cpp
    src/Inode.cpp
    src/Journal.cpp
    src/FileSystem.cpp
    src/main.cpp
)

# Add executable
add_executable(FileSystem ${SOURCES})

# Source files for journal test executable
set(JOURNAL_TEST_SOURCES
    src/BlockGroup.cpp
    src/Inode.cpp
    src/Journal.cpp
    src/FileSystem.cpp
    tests/JournalTest.cpp
)

# Add executable for journal test
add_executable(JournalTestExec ${JOURNAL_TEST_SOURCES})

# Google Test
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable
set(TEST_SOURCES tests/UnitTest.cpp src/BlockGroup.cpp src/Inode.cpp src/Journal.cpp src/FileSystem.cpp)
add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests gtest_main)

include(GoogleTest)
gtest_discover_tests(runTests)
