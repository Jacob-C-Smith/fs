// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <path/path.h>

// Entry point
int main(int argc, const char* argv[])
{

    // Initialized data
    path *p_path = 0;

    // Open the current working directory
    path_open(&p_path, 0, ".");

    // Success
    return EXIT_SUCCESS;
}