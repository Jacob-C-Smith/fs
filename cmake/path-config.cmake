# PATH CMake configuration file:
# This file is meant to be placed in a cmake subfolder of PATH-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(GPATH PROPERTIES
    URL "https://www.g10.app/status"
    DESCRIPTION "path"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(GPATH_FOUND TRUE)

# For compatibility with autotools GPATH-config.cmake, provide GPATH_* variables.

set_and_check(GPATH_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(GPATH_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(GPATH_INCLUDE_DIR  "${GPATH_PREFIX}/include")
set(GPATH_INCLUDE_DIRS           "${GPATH_INCLUDE_DIR}")
set_and_check(GPATH_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(GPATH_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(GPATH_LIBRARIES GPATH::GPATH)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated GPATH-target.cmake files.

set(_GPATH_library     "${GPATH_LIBDIR}/path.lib")
set(_GPATH_dll_library "${GPATH_BINDIR}/path.dll")
if(EXISTS "${_GPATH_library}" AND EXISTS "${_GPATH_dll_library}")
    if(NOT TARGET GPATH::GPATH)
        add_library(GPATH::GPATH SHARED IMPORTED)
        set_target_properties(GPATH::GPATH
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${GPATH_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_GPATH_library}"
                IMPORTED_LOCATION "${_GPATH_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "GPATH_SHARED"
                INTERFACE_GPATH_SHARED "ON"
        )
    endif()
    set(GPATH_GPATH_FOUND TRUE)
else()
    set(GPATH_GPATH_FOUND FALSE)
endif()
unset(_GPATH_library)
unset(_GPATH_dll_library)

check_required_components(GPATH)
