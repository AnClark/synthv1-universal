cmake_minimum_required (VERSION 3.15)

project (synthv1
  VERSION 0.9.32
  DESCRIPTION "an old-school polyphonic synthesizer"
  LANGUAGES C CXX)

set (SYNTHV1_REPO ${PROJECT_SOURCE_DIR}/src)

set (CONFIG_VERSION ${PROJECT_VERSION})
set (CONFIG_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CONFIG_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CONFIG_VERSION_PATCH ${PROJECT_VERSION_PATCH})
execute_process (
  COMMAND cd ${SYNTHV1_REPO} && git describe --tags --dirty --abbrev=6
  OUTPUT_VARIABLE GIT_DESCRIBE_OUTPUT
  RESULT_VARIABLE GIT_DESCRIBE_RESULT
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if (GIT_DESCRIBE_RESULT EQUAL 0)
  set (VERSION "${GIT_DESCRIBE_OUTPUT}")
  string (REGEX REPLACE "^[^0-9]+" "" VERSION "${VERSION}")
  string (REGEX REPLACE "^1_"      "" VERSION "${VERSION}")
  string (REGEX REPLACE "^[_vV]+"  "" VERSION "${VERSION}")
  string (REGEX REPLACE "-g"   "git." VERSION "${VERSION}")
  string (REGEX REPLACE "[_|-]"   "." VERSION "${VERSION}")
  execute_process (
    COMMAND cd ${SYNTHV1_REPO} && git rev-parse --abbrev-ref HEAD
    OUTPUT_VARIABLE GIT_REVPARSE_OUTPUT
    RESULT_VARIABLE GIT_REVPARSE_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  if (GIT_REVPARSE_RESULT EQUAL 0 AND NOT GIT_REVPARSE_OUTPUT STREQUAL "master")
    set (VERSION "${VERSION} [${GIT_REVPARSE_OUTPUT}]")
  endif ()
else ()
  set (VERSION "${PROJECT_VERSION}")
endif ()

set (PACKAGE_NAME "synthv1")
set (PACKAGE_VERSION "${VERSION}")
set (PACKAGE_BUGREPORT "rncbc@rncbc.org")
set (PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")
set (PACKAGE_TARNAME "synthv1")

set (CONFIG_BUILD_VERSION "${PACKAGE_VERSION}")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set (CONFIG_DEBUG 1)
  set (CONFIG_BUILD_TYPE "debug")
else ()
  set (CONFIG_DEBUG 0)
  set (CONFIG_BUILD_TYPE "release")
  set (CMAKE_BUILD_TYPE "Release")
endif ()

set (CONFIG_PREFIX "${CMAKE_INSTALL_PREFIX}")

include (GNUInstallDirs)
set (CONFIG_BINDIR  "${CONFIG_PREFIX}/${CMAKE_INSTALL_BINDIR}")
set (CONFIG_LIBDIR  "${CONFIG_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set (CONFIG_DATADIR "${CONFIG_PREFIX}/${CMAKE_INSTALL_DATADIR}")
set (CONFIG_MANDIR  "${CONFIG_PREFIX}/${CMAKE_INSTALL_MANDIR}")

# Enable DPF (DISTRHO Plugin Framework) plugin build.

option (CONFIG_DPF "Enable DPF (DISTRHO Plugin Framework) plug-in build (default=yes)" 1)

option (CONFIG_DPF_VST2 "Enable VST plug-in support (via DPF) (default=yes)" 1)

option (CONFIG_DPF_VST3 "Enable VST3 plug-in support (via DPF) (default=yes)" 1)

option (CONFIG_DPF_LV2 "Enable LV2 plug-in support (via DPF) (default=yes)" 1)

option (CONFIG_DPF_CLAP "Enable CLAP plug-in support (via DPF) (default=yes)" 1)

if (WIN32)
  # Windows install target pathes.
  set (CONFIG_WINDOWS_VST_PATH "$ENV{SYSTEMDRIVE}/Program Files/Steinberg/VstPlugins" CACHE STRING "Specify Windows VST install path")

  set (CONFIG_WINDOWS_VST3_PATH "$ENV{SYSTEMDRIVE}/Program Files/Common Files/VST3" CACHE STRING "Specify Windows VST3 install path")

  set (CONFIG_WINDOWS_CLAP_PATH "$ENV{SYSTEMDRIVE}/Program Files/Common Files/CLAP" CACHE STRING "Specify Windows CLAP install path")
endif ()


# Enable Wayland support option.
option (CONFIG_WAYLAND "Enable Wayland support (EXPERIMENTAL) (default=no)" 0)

# Enable Qt6 build preference.
option (CONFIG_QT6 "Enable Qt6 build (default=yes)" 1)


# Fix for new CMAKE_REQUIRED_LIBRARIES policy.
if (POLICY CMP0075)
  cmake_policy (SET CMP0075 NEW)
endif ()

# Fix for CXX_VISIBILITY_PRESET policy.
if (POLICY CMP0063)
  cmake_policy (SET CMP0063 NEW)
  set (CMAKE_CXX_VISIBILITY_PRESET hidden)
  set (CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif ()

# Check for Qt...
if (CONFIG_QT6)
  find_package (Qt6 QUIET)
  if (NOT Qt6_FOUND)
    set (CONFIG_QT6 0)
  endif ()
endif ()

if (CONFIG_QT6)
  find_package (QT QUIET NAMES Qt6)
else ()
  find_package (QT QUIET NAMES Qt5)
endif ()

find_package (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Xml Svg)

#find_package (Qt${QT_VERSION_MAJOR}LinguistTools)

include (CheckIncludeFile)
include (CheckIncludeFiles)
include (CheckIncludeFileCXX)
include (CheckFunctionExists)
include (CheckLibraryExists)

# Make sure we get some subtle optimizations out there...
add_compile_options (-ffast-math)

# Checks for header files.
if (UNIX AND NOT APPLE)
  check_include_files ("fcntl.h;unistd.h;signal.h" HAVE_SIGNAL_H)
endif ()


# Find package modules
include (FindPkgConfig)


# Build DPF plugins
if (CONFIG_DPF)
  add_subdirectory ("${PROJECT_SOURCE_DIR}/dpf")
  add_subdirectory (plugin)
endif ()


# Configuration status
macro (SHOW_OPTION text value)
  if (${value})
    message ("${text}: yes")
  else ()
    message ("${text}: no")
  endif ()
endmacro ()


message   ("\n  ${PACKAGE_NAME} ${PACKAGE_VERSION} (Qt ${QT_VERSION})")
message   ("\n  Build target . . . . . . . . . . . . . . . . . . .: ${CONFIG_BUILD_TYPE}\n")
show_option ("  DPF (DISTRHO Plugin Framework) support . . . . . ." CONFIG_DPF)
show_option ("  VST plug-in build (via DPF)  . . . . . . . . . . ." CONFIG_DPF_VST2)
show_option ("  VST3 plug-in build (via DPF) . . . . . . . . . . ." CONFIG_DPF_VST3)
show_option ("  LV2 plug-in build (via DPF)  . . . . . . . . . . ." CONFIG_DPF_LV2)
show_option ("  CLAP plug-in build (via DPF) . . . . . . . . . . ." CONFIG_DPF_CLAP)
message   ("\n  Install prefix . . . . . . . . . . . . . . . . . .: ${CONFIG_PREFIX}\n")
