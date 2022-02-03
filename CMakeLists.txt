cmake_minimum_required(VERSION 3.15)
project(
  Stage
  VERSION 0.1
  LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

# Set ccache, ipo, etc.
include(${modules}/ProjectSettings.cmake)

include(${modules}/WarningsAndOptions.cmake)

include(${modules}/Sanitizers.cmake)

add_library(Stage src/Stage/stage.cpp src/Stage/cmakeStage.cpp)

add_warnings(TARGET Stage)
add_options(TARGET Stage)

# Set the sanitizer options on project_options
enable_sanitizers(Stage)

target_include_directories(Stage PRIVATE src)
target_include_directories(
  Stage PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
               $<INSTALL_INTERFACE:include>)

set_target_properties(Stage PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                                OFF)

# Add library to link against
add_library(Tolc::Stage ALIAS Stage)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    option(ENABLE_COVERAGE "Use code coverage tools on the tests" OFF)
    enable_testing()
    # add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    # add_subdirectory(tools/packaging)
  endif()
endif()
