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

// Max length of a string representing a full path.
// Exceeding this limit will produce errors. Exceeding 
// this limit in while the NDEBUG macro is undefined 
// will print error logs to standard out. 
#define MAX_FILE_PATH_LEN 4096

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

// Allocators
/**!
 * Allocate memory for a path
 * 
 * @param pp_path ret
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_create ( path **pp_path );

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

// Accessors
/** !
 * Get the type of a path. The name of this function is bad.
 * 
 * @param p_path the path
 * 
 * @return < PATH_TYPE_FILE | PATH_TYPE_DIRECTORY >
*/
DLLEXPORT path_type path_type_path ( const path *const p_path );

/** !
 *  Return the name of the path as text
 *
 * @param p_path the path
 *
 * @return the name of the path as text on success, (void *) 0 on error
 */
DLLEXPORT const char *path_name_text ( const path *const p_path );

/** !
 *  Return the full path as text
 * 
 * @param p_path the path
 * 
 * @return the full path as text on success, (void *) 0 on error
 */
DLLEXPORT const char *path_full_path_text ( const path *const p_path );

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
DLLEXPORT int path_file_size ( const path *const p_path, size_t *p_size_in_bytes );

/** !
 *  Get the names of the directory's contents as path names, or the number of items in the directory
 *
 * @param p_path
 * @param names   return -OR- null pointer
 *
 * @return 1 on success, 0 on error, if keys != null, else number of properties in directory
 */
DLLEXPORT size_t path_directory_content_names ( const path *const p_path, const char **const names );

/** !
 *  Get the types of the directory's contents, or the number of items in the directory
 *
 * @param p_path
 * @param types   return -OR- null pointer
 *
 * @return 1 on success, 0 on error, if keys != null, else number of properties in directory
 */
DLLEXPORT size_t path_directory_content_types ( const path *const p_path, const path_type *types );

// Mutators
/** !
 * Navigate the filesystem from a source path
 * 
 * @param pp_path   pointer to source path
 * @param path_text text to append to path
 * 
 * @sa path_open
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_navigate ( path **pp_path, const char *path_text );

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
DLLEXPORT int path_create_file( path *p_path, const char *path );

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
DLLEXPORT int path_create_directory( path *p_path, const char *path );

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

// Iterators
/** !
 * Calls a parameterized function on each path in the directory
 * 
 * @param p_path the specified path
 * @param pfn_path_iter the iterator function, of type void (*)(const char *full_path, path_type type, size_t i)
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_directory_foreach_i ( const path *const p_path, void (*pfn_path_iter)(const char *full_path, path_type type, size_t i));

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