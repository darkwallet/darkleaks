cmake_minimum_required(VERSION 2.8.12)
project(darkleaks)

set(VERSION_MAJOR  0)
set(VERSION_MINOR  0)
set(VERSION_BUGFIX 0)
set(VERSION "0.0.0")


#-----------------------------------------------------------------------------
# Options for building

option(BUILD_VERBOSE       "Verbose build output." ON)
#option(BUILD_DOCUMENTATION "Build the Doxygen documentation." ON)
#option(BUILD_TESTS         "Build the tests." ON)

option(BUILD_SHARED_LIBS   "Build shared libraries." ON)

option(RPM                 "Build a RPM" OFF)
option(DEB                 "Build a DEB" OFF)

if(BUILD_VERBOSE)
  set(CMAKE_VERBOSE_MAKEFILE true)
endif()


#-----------------------------------------------------------------------------
# Print system information and build options

message(STATUS "General -------------------------------------")
message(STATUS "CMake version:          ${CMAKE_VERSION}")
message(STATUS "System:                 ${CMAKE_SYSTEM}")
message(STATUS "Processor:              ${CMAKE_SYSTEM_PROCESSOR}")

message(STATUS "Verbose:                ${BUILD_VERBOSE}")
#message(STATUS "Testing:                ${BUILD_TESTS}")

message(STATUS "Build Options -------------------------------")
message(STATUS "Build Shared libraries: ${BUILD_SHARED_LIBS}")

message(STATUS "Packaging -----------------------------------")
message(STATUS "Build RPM:              ${RPM}")
message(STATUS "Build DEB:              ${DEB}")


#-----------------------------------------------------------------------------
# Collect all binaries into bin subdirectory

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${CMAKE_CURRENT_BINARY_DIR}/lib)


#-----------------------------------------------------------------------------
# Set compiler flags

if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if(HAVE_STD11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(GCC_WARN_FLAGS "-Wnoexcept -Wstrict-null-sentinel -Wlogical-op -Wuseless-cast")
  else()
    message(FATAL_ERROR "No advanced standard C++ support (-std=c++11 not defined).")
  endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  set(CMAKE_COMPILER_IS_CLANGXX 1)
  INCLUDE(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG(-std=c++11 HAVE_STD11)
  if(HAVE_STD11)
    if(APPLE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
      set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} -stdlib=libc++")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif()
  else()
    message(FATAL_ERROR "No C++11 support for Clang version. Please upgrade Clang to a version supporting C++11.")
  endif()
endif()

if(BUILD_SHARED_LIBS)
  # Windows would complain about unknown flag -fPIC
  if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  endif()
endif()

if(WIN32)
  add_definitions(-D_UNICODE)
else()
  set(PEDANTIC_CXX_FLAGS "-Wall -Werror -Wextra -pedantic -Wno-missing-braces -Wcast-align -Wctor-dtor-privacy -Wdisabled-optimization -Winit-self -Wmissing-include-dirs -Wswitch-default -Wold-style-cast -Wredundant-decls -Woverloaded-virtual -Wmissing-declarations -Wunused-macros -Wundef -Wformat=2 -Wno-format-nonliteral ${GCC_WARN_FLAGS}")
endif()


message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Flags:       ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ link flags:  ${CMAKE_CXX_LINK_FLAGS}")


#-----------------------------------------------------------------------------
# Dependencies

if(WIN32)
  #ToDo: find libbitcoin on Windows
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(libbitcoin REQUIRED libbitcoin)
endif()


#-----------------------------------------------------------------------------
# Force out-of-source build

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
message(FATAL_ERROR "CMake generation is not allowed within the source directory!
Remove the CMakeCache.txt file and try again from another folder, e.g.:

   rm CMakeCache.txt
   mkdir build
   cd build
   cmake ..
")
endif()


#-----------------------------------------------------------------------------
# Default to Release build

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()


#-----------------------------------------------------------------------------
# Build source

#Fix RPATH
if(APPLE)
  set(CMAKE_INSTALL_NAME_DIR @rpath)
endif()
if(NOT RPM)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(python/darkleaks)
add_subdirectory(app)


#-----------------------------------------------------------------------------
# Install

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/darkleaks.hpp
        DESTINATION include
        COMPONENT dev)


#-----------------------------------------------------------------------------
# Uninstal

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)


#-----------------------------------------------------------------------------
# Packaging

set(CPACK_GENERATOR "TGZ")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_CONTACT "lclc")
set(CPACK_PACKAGE_VENDOR "unSYSTEM")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Decentralised Information Black Market")
set(CPACK_PACKAGE_DESCRIPTION "Decentralised Information Black Market

This library is experimental, so use at your own risk.

* Distribute an encrypted file to the world.
* Reveal some random section of the contents to prove
  the legitimacy of its contents using an algorithm for provably
  fair random selection to all parties.
* People bid on the remaining parts.
* Leaker redeeming funds automatically releases the decryption key
  allowing people to unlock the remaining parts of the file.
")

if(RPM)
  set(CPACK_GENERATOR "RPM")
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_RPM_COMPONENT_INSTALL ON)
  set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ChangeLog")
endif()

if(DEB)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEB_COMPONENT_INSTALL ON)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "extra")
  set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
  set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/darkwallet/darkleaks")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "lclc")
endif()

include(CPack)
