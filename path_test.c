/** !
 * @file path tester
 * 
 * @author Jacob C Smith
*/

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

// path submodule
#include <path/path.h>

// dict submodule
#include <dict/dict.h>

//////////////////
// Test results //
//////////////////
typedef enum result_e {
    zero  = 0,    
    one   = 1,
    match = 2
} result_t;

///////////////////
// Test counters //
///////////////////
int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;

//////////////////////////
// Forward declarations //
//////////////////////////

// Utility functions
int run_tests ( void );
int print_final_summary ( void );
int print_test ( const char *scenario_name, const char *test_name, bool passed );
bool test_path_open ( char *test_file, int (*expected_value_constructor) (path **), result_t expected );
bool test_path_is_file ( char *path_string, int (*expected_path_constructor) (path **), result_t expected );
result_t value_equals ( path *a, path *b );

int test_path_open_cases ( char *text );
int test_path_is_file_cases ( char *name );

// Scenario constructors
int construct_file ( path **pp_path );
int construct_file_size ( path **pp_path );
int construct_directory ( path **pp_path );
int construct_directory_file ( path **pp_path );
int construct_directory_files ( path **pp_path );
int construct_directory_mixed ( path **pp_path );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Run tests
    run_tests();

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int run_tests ( void )
{

    // Test path_open
    test_path_open_cases("path_open");

    // TODO: Test path_make_directory
    // test_path_make_directory("path_make_directory");

    // TODO: Test path_make_file
    // test_path_make_file("path_make_file");

    // TODO: Test path_remove
    // test_path_remove("path_remove");

    // TODO: Test path_is_file
    test_path_is_file_cases("path_is_file");

    // TODO: Test path_size
    // test_path_size("path_size");

    // TODO: Test path_name
    // test_path_name("path_name");

    // Success
    return 1;
}

int test_path_open_cases ( char *name )
{

    // Formatting
    printf("Scenario: %s\n", name);
    
    // Print tests
    print_test(name, "file.txt"       , test_path_open("test cases/file.txt"                 , construct_file           , match));
    print_test(name, "file size.txt"  , test_path_open("test cases/file size.txt"            , construct_file_size      , match));
    print_test(name, "directory"      , test_path_open("test cases/directory/"               , construct_directory      , match));
    print_test(name, "directory weird", test_path_open("test cases/directory\\//\\///"       , construct_directory      , match));
    print_test(name, "file not found" , test_path_open("test cases/this file isn't real.txt", 0                         , zero));
    //print_test(name, "directory file" , test_path_open("test cases/directory file/"          , construct_directory_file , match));
    //print_test(name, "directory files", test_path_open("test cases/directory files/"         , construct_directory_files, match));
    //print_test(name, "directory mixed", test_path_open("test cases/directory mixed/"         , construct_directory_mixed, match));

    // Print a test summary
    print_final_summary();

    // Success
    return 1;
}

int test_path_is_file_cases ( char *name )
{

    // Formatting
    printf("Scenario: %s\n", name);
    
    // Print tests
    print_test(name, "file.txt"       , test_path_is_file("test cases/file.txt"                 , construct_file           , true));
    print_test(name, "file size.txt"  , test_path_is_file("test cases/file size.txt"            , construct_file_size      , true));
    print_test(name, "directory"      , test_path_is_file("test cases/directory/"               , construct_directory      , false));
    print_test(name, "directory weird", test_path_is_file("test cases/directory\\//\\///"       , construct_directory      , false));
    print_test(name, "file not found" , test_path_is_file("test cases/this file isn't real.txt", 0                         , zero));
    //print_test(name, "directory file" , test_path_open("test cases/directory file/"          , construct_directory_file , match));
    //print_test(name, "directory files", test_path_open("test cases/directory files/"         , construct_directory_files, match));
    //print_test(name, "directory mixed", test_path_open("test cases/directory mixed/"         , construct_directory_mixed, match));

    // Print a test summary
    print_final_summary();

    // Success
    return 1;
}

result_t path_equals ( path *a, path *b )
{

    // Initialized data
    result_t ret = match;

    // Error check
    if ( a == (void *) 0 )
        return zero;
    
    if ( b == (void *) 0 )
        return zero;
    
    // Check for the same name
    if ( strcmp(a->name, b->name) )
        ret = zero;

    // Check for mismatched types
    if ( a->type != b->type )
        ret = 0;

    // Check for file equivalence
    if ( a->type == path_type_file )
    {

        // Check for equivalent size
        if ( a->file.file_size != b->file.file_size )
            ret = 0;
    }
    // Check for directory equivalence
    else
    {

    }

    // Success
    return ret;
}

int construct_file ( path **pp_path )
{
    
    // Argument check
    if ( pp_path == (void *) 0 )
        return 0;  

    // Initialized data
    path *p_path = realloc(0, sizeof(path));

    // Zero set
    memset(p_path, 0, sizeof(path));

    // Error check
    if ( p_path == (void *) 0 )
        return 0;  

    // Populate the path struct
    *p_path = (path) 
    {
        .full_path = 0,
        .name = "file.txt",
        .type = path_type_file,
        .p_next_content = 0,
        .p_parent_directory_path = 0,
        .file =
        {
            .file_size = 0
        }
    };

    // Return a pointer to the caller
    *pp_path = p_path;
    
    // Success
    return 1;
}

int construct_file_size ( path **pp_path )
{
    
    // Argument check
    if ( pp_path == (void *) 0 )
        return 0;  

    // Initialized data
    path *p_path = realloc(0, sizeof(path));

    // Zero set
    memset(p_path, 0, sizeof(path));

    // Error check
    if ( p_path == (void *) 0 )
        return 0;  

    // Populate the path struct
    *p_path = (path) 
    {
        .full_path = 0,
        .name = "file size.txt",
        .type = path_type_file,
        .p_next_content = 0,
        .p_parent_directory_path = 0,
        .file =
        {
            .file_size = 34
        }
    };

    // Return a pointer to the caller
    *pp_path = p_path;
    
    // Success
    return 1;
}

int construct_directory  ( path **pp_path )
{

    // Argument check
    if ( pp_path == (void *) 0 )
        return 0;  

    // Initialized data
    path *p_path = realloc(0, sizeof(path));

    // Zero set
    memset(p_path, 0, sizeof(path));

    // Error check
    if ( p_path == (void *) 0 )
        return 0;  

    // Populate the path struct
    *p_path = (path) 
    {
        .full_path = 0,
        .name = "directory",
        .type = path_type_directory,
        .p_next_content = 0,
        .p_parent_directory_path = 0,
        .directory =
        {
            .directory_content_size = 0,
            .p_directory_contents   = 0
        }
    };

    // Return a pointer to the caller
    *pp_path = p_path;
    
    // Success
    return 1;
}

int construct_directory_file  ( path **pp_path )
{
    
    // Success
    return 1;
}

int construct_directory_files ( path **pp_path )
{
    
    // Success
    return 1;
}

int construct_directory_mixed ( path **pp_path )
{
    
    // Success
    return 1;
}

bool test_path_open ( char *path_string, int (*expected_path_constructor) (path **), result_t expected )
{

    // Initialized data
    path *p_expected_path = 0,
         *p_result_path   = 0;
    int   result_ret      = 0,
          ret             = 0;

    // Construct the path
    if ( expected_path_constructor )

        // Call the path constructor
        expected_path_constructor(&p_expected_path);

    // Open the path
    result_ret = path_open(&p_result_path, 0, path_string);

    // Check for equivalence
    ret = path_equals(p_expected_path, p_result_path);

    // Success
    return (ret == expected);
}

bool test_path_is_file ( char *path_string, int (*expected_path_constructor) (path **), result_t expected )
{

    // Initialized data
    path *p_expected_path = 0,
         *p_result_path   = 0;
    int   result_ret      = 0,
          expected_ret    = 0;

    // Construct the path
    if ( expected_path_constructor )

        // Call the path constructor
        expected_path_constructor(&p_expected_path);

    // Open the path
    path_open(&p_result_path, 0, path_string);

    // Check the result
    result_ret = path_is_file(p_result_path);

    // Check the expected
    expected_ret = path_is_file(p_expected_path);

    // Success
    return (result_ret == expected_ret);
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    printf("%s %-75s %s\n",scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    if ( passed ) 
        ephemeral_passes++;
    else
        ephemeral_fails++;

    ephemeral_tests++;

    // Success
    return 1;
}

int print_final_summary ()
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    printf("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    printf("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
    
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}
