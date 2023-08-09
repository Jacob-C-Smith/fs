// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <path/path.h>

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    path       *p_path_file        = 0;
    path       *p_path_directory   = 0;
    size_t      size_in_bytes      = 0;
    size_t      directory_contents = 0;
    const char *p_path_name        = 0;
    char *names[100]               = { 0 };
    path_type types[100]           = { 0 };

    // Error check
    if ( argc != 2 ) goto usage_message;

    // Open a path
    if ( path_open(&p_path_file, "/mnt/c/Users/j/Desktop/path_experiment") == 0 ) goto failed_to_open_path;

    // Create a file
    if ( path_create_file(p_path_file, "file.txt") == 0 ) goto failed_to_create_file;
    
    // Create a directory
    //if ( path_create_directory(p_path_file, "directory") == 0 ) goto failed_to_create_directory;

    // Navigate into the directory
    if ( path_navigate(&p_path_file, "directory") == 0 ) goto failed_to_navigate;

    // Create a subdirectory
    //if ( path_create_directory(p_path_file, "subdirectory") == 0 ) goto failed_to_create_directory;

    // Navigate into the subdirectory
    if ( path_navigate(&p_path_file, "subdirectory") == 0 ) goto failed_to_navigate;

    // Create a file
    if ( path_create_file(p_path_file, "file.txt") == 0 ) goto failed_to_create_file;

    // Close the path
    (void)path_close(&p_path_file);

    // Success
    return EXIT_SUCCESS;

    failed_to_get_file_size:
    failed_to_open_path:
    failed_to_navigate:
    failed_to_create_file:
    failed_to_create_directory:

    // Error
    return EXIT_FAILURE;

    usage_message:
        printf("Usage: path_example /path/to/directory_for_path_example");

        // Error
        return EXIT_FAILURE;
}