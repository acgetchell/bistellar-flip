cmake_minimum_required(VERSION 3.22)

# vcpkg settings must be set before project()
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# In Manifest mode CMake invokes vcpkg automatically This makes setup easier,
# however, in CI or Docker we may want to turn this off
option(VCPKG_MANIFEST_MODE "Build vcpkg ports from manifest" ON)

# Ensure we install vcpkg ports in the same place so they can be reused between
# builds
set(_VCPKG_INSTALLED_DIR
    "${CMAKE_CURRENT_LIST_DIR}/vcpkg_installed"
    CACHE STRING "")

project(
  bistellar_flip
  VERSION 0.0.1
  DESCRIPTION "Bistellar flip in CGAL"
  LANGUAGES CXX)

# Project settings
include(cmake/StandardProjectSettings.cmake)

# Prevent in source builds
include(cmake/NoInSourceBuilds.cmake)

# vcpkg dependencies

# https://github.com/doctest/doctest
find_package(doctest CONFIG REQUIRED)

# https://github.com/fmtlib/fmt
find_package(fmt CONFIG REQUIRED)

# https://github.com/oneapi-src/oneTBB
find_package(TBB CONFIG REQUIRED)

# https://www.cgal.org
find_package(CGAL CONFIG REQUIRED)

# Header files
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)

# Source files
add_subdirectory(src)
