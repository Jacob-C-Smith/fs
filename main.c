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
    size_t      file_size = path_size(p_path);

    if ( is_file == false )
    {
        path *i_path = p_path->directory.p_directory_contents;

        // Formatting
        for (size_t i = 0; i < d; i++)
            putchar('\t');
        printf("D : \"%s\"\n", name);
        
        if ( i_path == 0 )
            goto done;
        
        while( i_path )
        {
            print_path(i_path, d+1);
            fflush(stdout);
            i_path = i_path->p_next_content;
        }
        
        done:

        // Formatting
        for (size_t i = 0; i < d; i++)
            putchar('\t');
        
        
        fflush(stdout);
    }
    else
    {
        // Formatting
        for (size_t i = 0; i < d; i++)
            putchar('\t');

        printf("F : %010d bytes : \"%s\"\n", file_size, name);
        fflush(stdout);
    }

    // Success
    return 1;

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
int main(int argc, const char* argv[])
{

    // Initialized data
    path *p_path = 0;

    // Open the current working directory
    path_open(&p_path, 0, "C:/Users/j/Desktop/C/g10/include");

    // tree like output
    print_path(p_path, 0);

    // Close the path
    //path_close(&p_path);

    // Success
    return EXIT_SUCCESS;
}