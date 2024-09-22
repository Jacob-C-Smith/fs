/** !
 * Example fs program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// log
#include <log/log.h>

// sync
#include <sync/sync.h>

// fs
#include <fs/fs.h>
#include <fs/path.h>
#include <fs/stream.h>

// Enumeration definitions
enum fs_examples_e
{
    FS_PATH_EXAMPLE        = 0,
    FS_STREAM_EXAMPLE      = 1,
    FS_EXAMPLES_QUANTITY   = 2
};

// Forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run );

/** !
 * Thread example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int fs_path_example ( int argc, const char *argv[] );

/** !
 * Thread pool example program
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return 1 on success, 0 on error
 */
int fs_stream_example ( int argc, const char *argv[] );

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    bool examples_to_run[FS_EXAMPLES_QUANTITY] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv, examples_to_run);

    // Formatting
    log_info("╭────────────╮\n");
    log_info("│ fs example │\n");
    log_info("╰────────────╯\n");
    log_info("The fs library provides high level abstractions for fs computing.\n");
    log_info("fs provides %d abstractions. The path, and the stream.\n\n", FS_EXAMPLES_QUANTITY);
    log_info("A path is [TODO].\n");
    log_info("A stream [TODO].\n");
    
    //////////////////////
    // Run the examples //
    //////////////////////

    // Run the path example program
    if ( examples_to_run[FS_PATH_EXAMPLE] )

        // Error check
        if ( fs_path_example(argc, argv) == 0 ) goto failed_to_run_path_example;

    // Run the path pool example program
    if ( examples_to_run[FS_STREAM_EXAMPLE] )

        // Error check
        if ( fs_stream_example(argc, argv) == 0 ) goto failed_to_run_stream_example;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {
        failed_to_run_path_example:

            // Print an error message
            log_error("Error: Failed to run path example!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_run_stream_example:

            // Print an error message
            log_error("Error: Failed to run path pool example!\n");

            // Error
            return EXIT_FAILURE;

            // Error
            return EXIT_FAILURE;   
    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [path] [stream]\n", argv0);

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[], bool *examples_to_run )
{

    // If no command line arguments are supplied, run all the examples
    if ( argc == 1 ) goto all_examples;

    // Error check
    if ( argc > FS_EXAMPLES_QUANTITY + 1 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (size_t i = 1; i < argc; i++)
    {
        
        // Path example?
        if ( strcmp(argv[i], "path") == 0 )

            // Set the path example flag
            examples_to_run[FS_PATH_EXAMPLE] = true;

        // Stream example?
        else if ( strcmp(argv[i], "stream") == 0 )
            
            // Set the path pool example flag
            examples_to_run[FS_STREAM_EXAMPLE] = true;

        // Default
        else goto invalid_arguments;
    }
    
    // Success
    return;

    // Set each example flag
    all_examples:
    {

        // For each example ...
        for (size_t i = 0; i < FS_EXAMPLES_QUANTITY; i++)
        
            // ... set the example flag
            examples_to_run[i] = true;
        
        // Success
        return;
    }

    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}

int fs_stream_example ( int argc, const char *argv[] )
{

    // Supress warnings
    (void) argc;
    (void) argv;

    // Formatting
    log_info(
        "╭────────────────╮\n"\
        "│ stream example │\n"\
        "╰────────────────╯\n"\
        "This example [TODO: Describe example]\n\n"
    );

    // Initialized data
    stream *p_stream = (void *) 0;
    timestamp   *_timestamps[7] = { 0 };
        
    // Construct a stream
    if ( stream_constuct(&p_stream, "resources/file.qoi", 4096, 0) == 0 ) goto failed_to_construct_stream;

    // Success
    return 1;

    // Error handling
    {

        // fs errors
        {
            failed_to_construct_stream:
                
                // Write an error message to standard out
                log_error("Failed to construct stream!\n");

                // Error
                return 0;
        }
    }
}

int fs_path_example ( int argc, const char *argv[] )
{
    return 1;
}