cmake_minimum_required (VERSION 3.26)

project ("path")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if ( NOT "${HAS_DICT}")
    message("[path] Adding dict project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)
    set(HAS_DICT true PARENT_SCOPE)
endif()

if ( NOT "${HAS_GPATH}")
    set(HAS_GPATH true PARENT_SCOPE)
endif()


# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (path_example "main.c" "path.c")
add_dependencies(path_example dict)
target_include_directories(path_example PUBLIC include include/directory ${CMAKE_SOURCE_DIR}/extern/dict/include/)
target_link_libraries(path_example dict)

# Add source to the tester
add_executable (path_test "path_test.c" "path.c")
add_dependencies(path_test dict)
target_include_directories(path_test PUBLIC include include/path ${CMAKE_SOURCE_DIR}/extern/dict/include/)
target_link_libraries(path_test dict)

# Add source to this project's library
add_library (path SHARED "path.c")
add_dependencies(path dict)
target_include_directories(path PUBLIC include include/path)
target_link_libraries(path PRIVATE dict)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

# Add source to the address sanatized version.
# Since the address sanitizer doesn't like link libraries, just add the dependency source code directly to the executable
add_executable (path_test_asan "path_test.c" "path.c" "${CMAKE_SOURCE_DIR}/extern/dict/dict.c")
target_include_directories(path_test_asan PUBLIC include include/path ${CMAKE_SOURCE_DIR}/extern/dict/include/)