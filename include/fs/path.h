/** !
 * Include header for path abstraction
 * 
 * @file fs/path.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// fs library
#include <fs/fs.h>

// Preprocessor macros
#define MAX_FILE_PATH_LEN 4096

// Forward declarations
struct path_s;

// Type definitions
typedef struct path_s path;

// Enumeration definitions
enum path_type_e
{
    PATH_TYPE_FILE      = 1,
    PATH_TYPE_DIRECTORY = 2,
    PATH_TYPE_SOCKET    = 3
};

// // Allocators
// /** !
//  * Allocate memory for a path
//  * 
//  * @param pp_path result
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_create ( path **pp_path );

// // Constructors
// /** !
//  * Construct a path from a string if pp_path references null pointer else update an existing path
//  * 
//  * @param pp_path return
//  * @param path the path, as a string
//  * 
//  * @sa path_close
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_open ( path **pp_path, const char *path );

// // Accessors
// /** !
//  * Get the type of a path. The name of this function is bad.
//  * 
//  * @param p_path the path
//  * 
//  * @return < PATH_TYPE_FILE | PATH_TYPE_DIRECTORY >
// */
// DLLEXPORT path_type path_type_path ( const path *const p_path );

// /** !
//  *  Return the name of the path as text
//  *
//  * @param p_path the path
//  *
//  * @return the name of the path as text on success, (void *) 0 on error
//  */
// DLLEXPORT const char *path_name_text ( const path *const p_path );

// /** !
//  *  Return the full path as text
//  * 
//  * @param p_path the path
//  * 
//  * @return the full path as text on success, (void *) 0 on error
//  */
// DLLEXPORT const char *path_full_path_text ( const path *const p_path );

// /** !
//  *  Returns the number of items in a directory, if p_path is a directory, else the size of the file
//  *
//  * @param p_path the directory item
//  *
//  * @sa directory_is_file
//  * @sa directory_name
//  *
//  * @return the size of the path on success, 0 on error
//  */
// DLLEXPORT int path_file_size ( const path *const p_path, size_t *p_size_in_bytes );

// /** !
//  *  Get the names of the directory's contents as path names, or the number of items in the directory
//  *
//  * @param p_path
//  * @param names   return -OR- null pointer
//  *
//  * @return 1 on success, 0 on error, if keys != null, else number of properties in directory
//  */
// DLLEXPORT size_t path_directory_content_names ( const path *const p_path, const char **const names );

// /** !
//  *  Get the types of the directory's contents, or the number of items in the directory
//  *
//  * @param p_path
//  * @param types   return -OR- null pointer
//  *
//  * @return 1 on success, 0 on error, if keys != null, else number of properties in directory
//  */
// DLLEXPORT size_t path_directory_content_types ( const path *const p_path, const path_type *types );

// // Mutators
// /** !
//  * Navigate the filesystem from a source path
//  * 
//  * @param pp_path   pointer to source path
//  * @param path_text text to append to path
//  * 
//  * @sa path_open
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_navigate ( path **pp_path, const char *path_text );

// /** !
//  * Make a file in the specified path
//  * 
//  * @param p_path the specified path
//  * @param path the name of the file
//  * 
//  * @sa path_make_directory
//  * @sa path_remove
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_create_file ( path *p_path, const char *path );

// /** !
//  * Make a directory in the specified path
//  * 
//  * @param p_path the specified path
//  * @param path the name of the directory
//  * 
//  * @sa path_make_file
//  * @sa path_remove
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_create_directory ( path *p_path, const char *path );

// /** !
//  * Remove a file / directory from the specified path
//  * 
//  * @param p_path the specified path
//  * @param path the name of the file/directory
//  * 
//  * @sa path_make_directory
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_remove ( path *p_path, const char *path );

// // Iterators
// /** !
//  * Call a function for each path in a directory
//  * 
//  * @param p_path the specified path
//  * @param pfn_path_iter the iterator function, of type void (*)(const char *full_path, path_type type, size_t i)
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_directory_foreach_i ( const path *const p_path, void (*pfn_path_iter)(const char *full_path, path_type type, size_t i));

// // Destructors
// /** !
//  * Close a path
//  * 
//  * @param pp_path pointer to path pointer
//  * 
//  * @sa path_open
//  * 
//  * @return 1 on success, 0 on error
// */
// DLLEXPORT int path_close ( path **pp_path );