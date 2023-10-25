# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# This is the name of the repository
project ("path")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set for debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Find the stack module
if ( NOT "${HAS_STACK}")
    
    # Log
    message("[path] Adding stack project")
    
    # Add the stack directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/stack ${CMAKE_SOURCE_DIR}/build/stack)

    # Let any parent project know that this project has stack
    set(HAS_STACK true PARENT_SCOPE)
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Log
    message("[path] Adding array project")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    set(HAS_ARRAY true PARENT_SCOPE)
endif()

# Find the json module
if ( NOT "${HAS_JSON}")
    
    # Log
    message("[path] Adding json project")
    
    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    set(HAS_JSON true PARENT_SCOPE)
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Log
    message("[path] Adding dict project")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[path] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Find the PATH module
if ( NOT "${HAS_PATH}")

    # Log
    message("[path] Adding path project")

    # Let any parent project know that it has the path project
    set(HAS_PATH true PARENT_SCOPE)
endif()

# Add source to this project's executable.
add_executable (path_example "main.c")
add_dependencies(path_example stack dict sync)
target_include_directories(path_example PUBLIC include include/path ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(path_example path dict sync crypto)

# Add source to the tester
add_executable (path_test "path_test.c" "path.c" )
add_dependencies(path_test stack dict sync)
target_include_directories(path_test PUBLIC include include/path ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(path_test path json_test_lib json array dict sync crypto)

# Add source to this project's library
add_library (path SHARED "path.c")
add_dependencies(path stack dict sync)
target_include_directories(path PUBLIC include include/path ${CMAKE_SOURCE_DIR}/extern/stack/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/sync/include/)
target_link_libraries(path PRIVATE stack dict sync crypto)
