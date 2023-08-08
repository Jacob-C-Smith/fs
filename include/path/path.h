/** !
 * @file path/path.h 
 * 
 * @author Jacob Smith
 * 
 * Include header for path library
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// dict submodule
#include <dict/dict.h>

// stack submodule
#include <stack/stack.h>

// Platform dependent includes
#ifdef _WIN64
#include <windows.h>
#include <process.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Set the reallocator for the dict submodule
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the stack submodule
#ifdef STACK_REALLOC
    #undef STACK_REALLOC
    #define STACK_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef PATH_REALLOC
#define PATH_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct path_s;

// Type definitions
typedef struct path_s path;

// Enumeration definitions
typedef enum 
{
    PATH_TYPE_FILE      = 1,
    PATH_TYPE_DIRECTORY = 2
} path_type;

// Constructors
/** !
 * Construct a path from a string if pp_path references null pointer else update an existing path
 * 
 * @param pp_path return
 * @param path the path, as a string
 * 
 * @sa path_close
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_open ( path **pp_path, const char *path );

// File system operations
/** !
 * Make a directory in the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the directory
 * 
 * @sa path_make_file
 * @sa path_remove
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_make_directory( path *p_path, const char *path );

/** !
 * Make a file in the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the file
 * 
 * @sa path_make_directory
 * @sa path_remove
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_make_file( path *p_path, const char *path );

/** !
 * Remove a file/directory from the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the file/directory
 * 
 * @sa path_make_directory
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_remove ( path *p_path, const char *path );

// Accessors
/** !
 *  Is the path a directory or a file?
 *
 * @param pp_directory return
 *
 * @sa directory_file_size
 * @sa directory_name
 *
 * @return true if directory item is file, false if directory item is directory. False on error
 */
DLLEXPORT bool path_is_file ( const path * const p_path );

/** !
 *  Returns the number of items in a directory, if p_path is a directory, else the size of the file
 *
 * @param p_path the directory item
 *
 * @sa directory_is_file
 * @sa directory_name
 *
 * @return the size of the path on success, 0 on error
 */
DLLEXPORT int path_file_size ( path *p_path, size_t *p_size_in_bytes );

/** !
 *  Returns the directory item name
 *
 * @param p_directory_item the directory item
 *
 * @sa directory_is_file
 * @sa directory_file_size
 *
 * @return the name of the file on success, 0 on error
 */
DLLEXPORT const char *path_name ( path *p_path );

// Destructors
/** !
 * Close a path
 * 
 * @param pp_path pointer to path pointer
 * 
 * @sa path_open
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_close ( path **pp_path );

