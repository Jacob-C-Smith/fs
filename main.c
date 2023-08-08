// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <path/path.h>

int print_path ( path *p_path, int d )
{

    // Argument error
    if ( p_path == (void *) 0 ) goto no_path;

    // Initialized data
    bool        is_file   = path_is_file(p_path);
    const char *name      = path_name(p_path);
    size_t      file_size = 0;

    // Get 

    // Path is directory
    if ( is_file == false )
    {

        // Formatting
        for (size_t i = 0; i < d; i++) putchar(' ');

        printf("D : \"%s\"\n", name);        
    }

    // Path is file
    else 
    {

        // Get the file size
        if ( path_file_size(p_path, &file_size) == 0 ) goto failed_to_get_path_file_size;

        // Print file
        printf("F : %010d bytes : \"%s\"\n", file_size, name);

    }

    // Success
    return 1;

    // Error
    failed_to_get_path_file_size:
    return 0;

    // Error handling
    {
        
        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    path   *p_path              = 0;
    size_t  G10_h_size_in_bytes = 0;

    // Open the current working directory
    if ( path_open(&p_path, "/mnt/c/Users/j/Desktop/C/g10/include/G10/G10.h") == 0 ) goto failed_to_open_path;

    // Get the size of the file
    if ( path_file_size(p_path, &G10_h_size_in_bytes) == 0 ) goto failed_to_get_file_size;

    // Navigate around
    if ( path_navigate(&p_path, "../some/other/file/../../thing/else.txt") == 0 ) goto failed_to_navigate;

    // tree like output
    (void)print_path(p_path, 0);

    // Close the path
    (void)path_close(&p_path);

    // Success
    return EXIT_SUCCESS;

    failed_to_get_file_size:
    failed_to_open_path:
    failed_to_navigate:

    // Error
    return EXIT_FAILURE;
}