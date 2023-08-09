/** !
 * Tests path module
 * 
 * @file path_test.c
 * 
 * @author Jacob C Smith
*/

/** !
 * Commentary
 *
 *   Testing paths is a lot like testing JSON. Paths are an abstraction of file 
 *   systems, and file systems exist on hard drives. Hard drives are linearly
 *   indexed. That is to say, the file system can be serialized. JSON can also 
 *   be serialized. Thus, the file system can be encoded as JSON. The representation
 *   I've chosen looks like this:
 *      
 *    - FILE SYSTEM -                          | - JSON -
 *      +--- A                                 |   {
 *      |    +                                 |       "A" : { },
 *      |                                      |       "B" : {
 *      +--- B                                 |          "email address.txt" : 30,
 *      |    +--- email address.txt (30 bytes) |          "birthday.txt" : 6,
 *      |    +--- birthday.txt (6 bytes)       |       }, 
 *      |                                      |       "C" : {
 *      +--- C                                 |          "file.c" : 8,
 *          +--- file C.txt (8 byte)           |          "code" : { "path.h" : 600 } 
 *           +--- code                         |       }
 *                + path.h (600 bytes)         |   }  
 *      
 *   TODO: The value_equals function has been included from the json_test.h 
 *         header. The path_as_json function shall serialize a file system from a path
 *         in accordance with the above encoding. The value_equals and path_as_json 
 *         functions make it trivial to compare a path object to an expected result 
 *         serialized as JSON text. 
 */

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <float.h>

// dict submodule
#include <dict/dict.h>

// array submodule
#include <array/array.h>

// json submodule
#include <json/json.h>

// json tester submodule
#include <json/json_test.h>

// path submodule
#include <path/path.h>

//////////////////
// Test results //
//////////////////
typedef enum result_e {
    zero = 0,    
    one  = 1,
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
/*
int      print_time_pretty   ( double seconds );
int      run_tests           ( void );
int      print_final_summary ( void );
int      print_test          ( const char   *scenario_name, const char   *test_name,                                    bool     passed );

int test_parse_null   (char *text);
int test_parse_bool   (char *text);
int test_parse_int    (char *text);
int test_parse_float  (char *text);
int test_parse_string (char *text);
int test_parse_object (char *text);
int test_parse_array  (char *text);
int test_serial_null  (char *text);
int test_serial_bool  (char *text);
int test_serial_int   (char *text);
int test_serial_float (char *text);
int test_serial_string(char *text);
int test_serial_object(char *text);
int test_serial_array (char *text);
*/

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();

    // Formatting
    printf("|=============|\n| PATH TESTER |\n|=============|\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();
    
    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    printf("\npath tests took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    printf(" to test\n");

    // Flush stdio
    fflush(stdout);

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) 
        printf("%d D, ", days);
    
    // Print hours
    if ( hours )
        printf("%d h, ", hours);

    // Print minutes
    if ( minutes )
        printf("%d m, ", minutes);

    // Print seconds
    if ( __seconds )
        printf("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds )
        printf("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds )
        printf("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( void )
{

    // Test the parser
    // Start
    timestamp parser_t0 = 0,
              parser_t1 = 0,
              serial_t0 = 0,
              serial_t1 = 0;

    // Start
    parser_t0 = timer_high_precision();
    
    // Test open
    {
        /*
        // Test opening a file
        test_open_file_txt("open_file_txt");

        // Test opening a file with a specific size
        test_parse_bool("path_open_");

        // 
        test_parse_int("parse_int");

        // Test parsing valid, invalid, and over/underflowed floating point numbers
        test_parse_float("parse_float");
        
        // Test parsing valid, invalid, and unicode strings
        test_parse_string("parse_string");

        // Test parsing a variety of objects
        test_parse_object("parse_object");

        // Test parsing a variety of arrays
        test_parse_array("parse_array");
        */
    }

    // Stop
    parser_t1 = timer_high_precision();


    // Test the serializer

    // Start
    serial_t0 = timer_high_precision();

/*
    // Test serializing null
    test_serial_null("serial_null");
    
    // Test serializing booleans
    test_serial_bool("serial_bool");

    // Test serializing integers
    test_serial_int("serial_int");

    // Test serializing floating point numbers
    test_serial_float("serial_float");
    
    // Test serializing strings
    test_serial_string("serial_string");

    // Test serializing objects
    test_serial_object("serial_object");

    // Test serializing arrays
    test_serial_array("serial_array");
*/
    // Stop
    serial_t1 = timer_high_precision();

/*
    // Report the time it took to run the parser tests
    printf("parser tests took: ");
    print_time_pretty ( (double)(parser_t1-parser_t0)/(double)timer_seconds_divisor() );
    printf(" to test\n");
*/
/*
    // Report the time it took to run the serializer tests
    printf("serial tests took: ");
    print_time_pretty ( (double)(serial_t1-serial_t0)/(double)timer_seconds_divisor() );
    printf(" to test\n");
*/

    // Success
    return 1;
}

int path_to_json ( const path *const p_path, json_value **pp_value )
{

    // Initialized data
    json_value *p_value1    = calloc(1, sizeof(json_value));
    json_value *p_value2    = calloc(1, sizeof(json_value));
    const char *p_path_name = path_name_text(p_path);
    size_t      len         = strlen(path_name);
    char       *_path_name  = calloc(len+1, sizeof(char));

    p_value1->type = JSON_VALUE_OBJECT;
    
    dict_construct(&p_value1->object, 32);
    
    strncpy(_path_name, p_path_name, len);

    // file path -> JSON
    if ( path_type_path(p_path) == PATH_TYPE_FILE )
    {
        p_value2->type = JSON_VALUE_INTEGER;

        path_file_size(p_path, &p_value2->integer);

        dict_construct(&p_value1->object, 16);
        dict_add(&p_value1->object, _path_name, p_value2);
    }

    // directory path -> JSON
    else if ( path_type_path(p_path) == PATH_TYPE_DIRECTORY )
    {
        
        // TODO:
        // 1. Dump directory names and contents
        // 2. Iterate over each path in the directory
        // 2.1 Call path_to_json on each path in the directory
        // 3 Serialize and return json_value

        char       *names[4096] = { 0 };
        size_t      count       = path_directory_content_names(p_path, 0);

        path_directory_content_names(p_path, names);

        for (size_t i = 0; i < count; i++)
        {
            
            //path_to_json(0, &p_valueN);

            //dict_add(p_value1, names[i], p_valueN);
        }
        
    }

    // Return 
    *pp_value = p_value1;

    // Success
    return 1;

}

/*
int test_parse_null ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "null", test_parse_json("parse test cases/pass/null.json", (void *) 0, one));
    print_test(name, "nul" , test_parse_json("parse test cases/fail/null.json", (void *) 0, zero));

    print_final_summary();

    return 1;

}

int test_parse_bool ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "false", test_parse_json("parse test cases/pass/bool/bool_false.json", construct_bool_false, one));
    print_test(name, "true" , test_parse_json("parse test cases/pass/bool/bool_true.json" , construct_bool_true , one));
    print_test(name, "fals" , test_parse_json("parse test cases/fail/bool/bool_false.json", (void *) 0          , zero));
    print_test(name, "tru"  , test_parse_json("parse test cases/fail/bool/bool_true.json" , (void *) 0          , zero));

    print_final_summary();

    return 1;

}

int test_parse_int ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "-1"    , test_parse_json("parse test cases/pass/int/int_-1.json" , construct_int_minus_one, one));
    print_test(name, "0"     , test_parse_json("parse test cases/pass/int/int_0.json"  , construct_int_zero     , one));
    print_test(name, "1"     , test_parse_json("parse test cases/pass/int/int_1.json"  , construct_int_one      , one));
    print_test(name, "max"   , test_parse_json("parse test cases/pass/int/int_max.json", construct_int_max      , one));
    print_test(name, "min"   , test_parse_json("parse test cases/pass/int/int_min.json", construct_int_min      , one));
    print_test(name, "max +1", test_parse_json("parse test cases/fail/int/int_max.json", (void *)0              , zero));
    print_test(name, "min -1", test_parse_json("parse test cases/fail/int/int_min.json", (void *)0              , zero));

    print_final_summary();

    return 1;
}

int test_parse_float ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "-1.0"    , test_parse_json("parse test cases/pass/float/float_-1.json" , construct_float_minus_one, one));
    print_test(name, "0.0"     , test_parse_json("parse test cases/pass/float/float_0.json"  , construct_float_zero     , one));
    print_test(name, "1.0"     , test_parse_json("parse test cases/pass/float/float_1.json"  , construct_float_one      , one));
    print_test(name, "max"     , test_parse_json("parse test cases/pass/float/float_max.json", construct_float_max      , one));
    print_test(name, "min"     , test_parse_json("parse test cases/pass/float/float_min.json", construct_float_min      , one));
    print_test(name, "max + 1.0", test_parse_json("parse test cases/fail/float/float_max.json", (void *)0                , zero));
    print_test(name, "min - 1.0", test_parse_json("parse test cases/fail/float/float_min.json", (void *)0                , zero));

    print_final_summary();

    return 1;
}

int test_parse_string ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "\"\""       , test_parse_json("parse test cases/pass/string/string_empty.json"          , construct_string_empty          , one));
    print_test(name, "\"a\""      , test_parse_json("parse test cases/pass/string/string_a.json"              , construct_string_a              , one));
    print_test(name, "\"abc\""    , test_parse_json("parse test cases/pass/string/string_abc.json"            , construct_string_abc            , one));
    print_test(name, "\"\"abc\"\"", test_parse_json("parse test cases/pass/string/string_quote_abc_quote.json", construct_string_quote_abc_quote, one));
    print_test(name, "\"\"\"\""   , test_parse_json("parse test cases/pass/string/string_quote_quote.json"    , construct_string_quote_quote    , one));
    print_test(name, "    \"abc\"", test_parse_json("parse test cases/pass/string/string_whitespaces_abc.json", construct_string_whitespaces_abc, one));
    print_test(name, "\"\"\""     , test_parse_json("parse test cases/pass/string/string_quote.json"          , construct_string_quote          , one));
    print_test(name, "\"\\\\\""   , test_parse_json("parse test cases/pass/string/string_reverse_solidus.json", construct_string_reverse_solidus, one));
    print_test(name, "\"\\/\""    , test_parse_json("parse test cases/pass/string/string_solidus.json"        , construct_string_solidus        , one));
    print_test(name, "\"\\b\""    , test_parse_json("parse test cases/pass/string/string_backspace.json"      , construct_string_backspace      , one));
    print_test(name, "\"\\f\""    , test_parse_json("parse test cases/pass/string/string_formfeed.json"       , construct_string_formfeed       , one));
    print_test(name, "\"\\n\""    , test_parse_json("parse test cases/pass/string/string_linefeed.json"       , construct_string_linefeed       , one));
    print_test(name, "\"\\r\""    , test_parse_json("parse test cases/pass/string/string_carriage_return.json", construct_string_carriage_return, one));
    print_test(name, "\"\\t\""    , test_parse_json("parse test cases/pass/string/string_horizontal_tab.json" , construct_string_horizontal_tab , one));
    //print_test(name, "string \"\u1234\"" , test_parse_json("parse test cases/pass/string/string_escape.json"         , construct_string_escape         , one));

    print_final_summary();

    return 1;
}

int test_parse_object ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "{}"                                               , test_parse_json("parse test cases/pass/object/object_empty.json"        , construct_object_empty        , one));
    print_test(name, "{\"abc\":\"def\"}"                                , test_parse_json("parse test cases/pass/object/object_string.json"       , construct_object_string       , one));
    print_test(name, "{\"abc\":123}"                                    , test_parse_json("parse test cases/pass/object/object_int.json"          , construct_object_int          , one));
    print_test(name, "{\"pi\":3.14}"                                    , test_parse_json("parse test cases/pass/object/object_float.json"        , construct_object_float        , one));
    print_test(name, "{\"abc\":false}"                                  , test_parse_json("parse test cases/pass/object/object_false.json"        , construct_object_false        , one));
    print_test(name, "{\"abc\":true}"                                   , test_parse_json("parse test cases/pass/object/object_true.json"         , construct_object_true         , one));
    print_test(name, "{\"abc\":\"def\",\"ghi\":\"jkl\",\"mno\":\"pqr\"}", test_parse_json("parse test cases/pass/object/object_strings.json"      , construct_object_strings      , one));
    print_test(name, "{\"name\":\"jake\",\"age\":20,\"height\":1.779}"  , test_parse_json("parse test cases/pass/object/object_mixed_values.json" , construct_object_mixed_values , one));
    print_test(name, "{\"abc\":{\"def\":123}}"                          , test_parse_json("parse test cases/pass/object/object_object.json"       , construct_object_object       , one));
    print_test(name, "{\"abc\":{\"def\":{\"ghi\":123}}}"                , test_parse_json("parse test cases/pass/object/object_object_object.json", construct_object_object_object, one));
    print_test(name, "{\"abc\":[1,2,3]}"                                , test_parse_json("parse test cases/pass/object/object_array.json"        , construct_object_array        , one));
    print_test(name, "{\"a\":[{\"a\":1},{\"b\":2},{\"c\":3}]}"          , test_parse_json("parse test cases/pass/object/object_array_objects.json", construct_object_array_objects, one));
    print_test(name, "{\"a\":[{\"a\":1}]}"                              , test_parse_json("parse test cases/pass/object/object_array_object.json" , construct_object_array_object , one));
    print_test(name, "{\"a\":{\"b\":{\"c\":{\"d\":{...{\"z\":{ }...}"   , test_parse_json("parse test cases/pass/object/object_recursive.json"    , construct_object_recursive    , one));

    print_final_summary();

    return 1;
}

int test_parse_array ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "[]"                                  , test_parse_json("parse test cases/pass/array/array_empty.json"            , construct_array_empty            , one));
    print_test(name, "[null]"                              , test_parse_json("parse test cases/pass/array/array_null.json"             , construct_array_null             , one));
    print_test(name, "[null, null, null]"                  , test_parse_json("parse test cases/pass/array/array_nulls.json"            , construct_array_nulls            , one));
    print_test(name, "[true]"                              , test_parse_json("parse test cases/pass/array/array_bool.json"             , construct_array_bool             , one));
    print_test(name, "[true, false, true]"                 , test_parse_json("parse test cases/pass/array/array_bools.json"            , construct_array_bools            , one));
    print_test(name, "[1]"                                 , test_parse_json("parse test cases/pass/array/array_int.json"              , construct_array_int              , one));
    print_test(name, "[1, 2, 3]"                           , test_parse_json("parse test cases/pass/array/array_ints.json"             , construct_array_ints             , one));
    print_test(name, "[3.14]"                              , test_parse_json("parse test cases/pass/array/array_float.json"            , construct_array_float            , one));
    print_test(name, "[1.2, 3.4, 5.6]"                     , test_parse_json("parse test cases/pass/array/array_floats.json"           , construct_array_floats           , one));
    print_test(name, "[\"\"]"                              , test_parse_json("parse test cases/pass/array/array_string_empty.json"     , construct_array_string_empty     , one));
    print_test(name, "[\"abc\"]"                           , test_parse_json("parse test cases/pass/array/array_string.json"           , construct_array_string           , one));
    print_test(name, "[\"abc\", \"def\", \"ghi\"]"         , test_parse_json("parse test cases/pass/array/array_strings.json"          , construct_array_strings          , one));
    print_test(name, "[{}]"                                , test_parse_json("parse test cases/pass/array/array_object_empty.json"     , construct_array_object_empty     , one));
    print_test(name, "[{\"a\":1}]"                         , test_parse_json("parse test cases/pass/array/array_object.json"           , construct_array_object           , one));
    print_test(name, "[[{\"a\":1}, {\"b\":2}, {\"c\":3}]]" , test_parse_json("parse test cases/pass/array/array_objects.json"          , construct_array_objects          , one));
    print_test(name, "[[]]"                                , test_parse_json("parse test cases/pass/array/array_array_empty.json"      , construct_array_array_empty      , one));
    print_test(name, "[[[]]]"                              , test_parse_json("parse test cases/pass/array/array_array_array_empty.json", construct_array_array_array_empty, one));
    print_test(name, "[[1, 2, 3],[4, 5, 6],[7, 8, 9]]"     , test_parse_json("parse test cases/pass/array/array_matrix.json"           , construct_array_matrix           , one));
    print_test(name, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]", test_parse_json("parse test cases/pass/array/array_tensor.json"           , construct_array_tensor           , one));

    print_final_summary();

    return 1;
}

int test_serial_null ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "null", test_serial_json("serial test cases/TESTER_null.json", "parse test cases/pass/null.json", construct_null, one));

    print_final_summary();

    return 1;

}

int test_serial_bool ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "false", test_serial_json("serial test cases/bool/TESTER_bool_false.json", "parse test cases/pass/bool/bool_false.json", construct_bool_false, one));
    print_test(name, "true" , test_serial_json("serial test cases/bool/TESTER_bool_true.json" , "parse test cases/pass/bool/bool_true.json" , construct_bool_true , one));

    print_final_summary();

    return 1;

}

int test_serial_int ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    print_test(name, "-1"    , test_serial_json("serial test cases/int/TESTER_int_-1.json" , "parse test cases/pass/int/int_-1.json" , construct_int_minus_one, one));
    print_test(name, "0"     , test_serial_json("serial test cases/int/TESTER_int_0.json"  , "parse test cases/pass/int/int_0.json"  , construct_int_zero     , one));
    print_test(name, "1"     , test_serial_json("serial test cases/int/TESTER_int_1.json"  , "parse test cases/pass/int/int_1.json"  , construct_int_one      , one));
    print_test(name, "max"   , test_serial_json("serial test cases/int/TESTER_int_max.json", "parse test cases/pass/int/int_max.json", construct_int_max      , one));
    print_test(name, "min"   , test_serial_json("serial test cases/int/TESTER_int_min.json", "parse test cases/pass/int/int_min.json", construct_int_min      , one));

    print_final_summary();

    return 1;
}

int test_serial_float ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);

    printf("TODO: While the floating point serializer does work, the formatting is poor.\n");
    printf("These tests will fail if compared against the original file, so I've disabled them\n");

    // TODO: Format perfectly. 
    //print_test(name, "-1.0"    , test_serial_json("serial test cases/float/TESTER_float_-1.json" , "parse test cases/pass/float/float_-1.json" , construct_float_minus_one, one));
    //print_test(name, "0.0"     , test_serial_json("serial test cases/float/TESTER_float_0.json"  , "parse test cases/pass/float/float_0.json"  , construct_float_zero     , one));
    //print_test(name, "1.0"     , test_serial_json("serial test cases/float/TESTER_float_1.json"  , "parse test cases/pass/float/float_1.json"  , construct_float_one      , one));
    //print_test(name, "max"     , test_serial_json("serial test cases/float/TESTER_float_max.json", "parse test cases/pass/float/float_max.json", construct_float_max      , one));
    //print_test(name, "min"     , test_serial_json("serial test cases/float/TESTER_float_min.json", "parse test cases/pass/float/float_min.json", construct_float_min      , one));

    print_final_summary();

    return 1;
}

int test_serial_string ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "\"\""       , test_serial_json("serial test cases/string/TESTER_string_empty.json"          , "parse test cases/pass/string/string_empty.json"          , construct_string_empty          , one));
    print_test(name, "\"a\""      , test_serial_json("serial test cases/string/TESTER_string_a.json"              , "parse test cases/pass/string/string_a.json"              , construct_string_a              , one));
    print_test(name, "\"abc\""    , test_serial_json("serial test cases/string/TESTER_string_abc.json"            , "parse test cases/pass/string/string_abc.json"            , construct_string_abc            , one));
    print_test(name, "\"\"abc\"\"", test_serial_json("serial test cases/string/TESTER_string_quote_abc_quote.json", "parse test cases/pass/string/string_quote_abc_quote.json", construct_string_quote_abc_quote, one));
    print_test(name, "\"\"\"\""   , test_serial_json("serial test cases/string/TESTER_string_quote_quote.json"    , "parse test cases/pass/string/string_quote_quote.json"    , construct_string_quote_quote    , one));
    print_test(name, "\"\"\""     , test_serial_json("serial test cases/string/TESTER_string_quote.json"          , "parse test cases/pass/string/string_quote.json"          , construct_string_quote          , one));
    print_test(name, "\"\\\\\""   , test_serial_json("serial test cases/string/TESTER_string_reverse_solidus.json", "parse test cases/pass/string/string_reverse_solidus.json", construct_string_reverse_solidus, one));
    print_test(name, "\"\\/\""    , test_serial_json("serial test cases/string/TESTER_string_solidus.json"        , "parse test cases/pass/string/string_solidus.json"        , construct_string_solidus        , one));
    print_test(name, "\"\\b\""    , test_serial_json("serial test cases/string/TESTER_string_backspace.json"      , "parse test cases/pass/string/string_backspace.json"      , construct_string_backspace      , one));
    print_test(name, "\"\\f\""    , test_serial_json("serial test cases/string/TESTER_string_formfeed.json"       , "parse test cases/pass/string/string_formfeed.json"       , construct_string_formfeed       , one));
    print_test(name, "\"\\n\""    , test_serial_json("serial test cases/string/TESTER_string_linefeed.json"       , "parse test cases/pass/string/string_linefeed.json"       , construct_string_linefeed       , one));
    print_test(name, "\"\\r\""    , test_serial_json("serial test cases/string/TESTER_string_carriage_return.json", "parse test cases/pass/string/string_carriage_return.json", construct_string_carriage_return, one));
    print_test(name, "\"\\t\""    , test_serial_json("serial test cases/string/TESTER_string_horizontal_tab.json" , "parse test cases/pass/string/string_horizontal_tab.json" , construct_string_horizontal_tab , one));

    print_final_summary();

    return 1;
}

int test_serial_object ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "{}"                                               , test_serial_json("serial test cases/object/TESTER_object_empty.json"        , "parse test cases/pass/object/object_empty.json"        , construct_object_empty        , one));
    print_test(name, "{\"abc\":\"def\"}"                                , test_serial_json("serial test cases/object/TESTER_object_string.json"       , "parse test cases/pass/object/object_string.json"       , construct_object_string       , one));
    print_test(name, "{\"abc\":123}"                                    , test_serial_json("serial test cases/object/TESTER_object_int.json"          , "parse test cases/pass/object/object_int.json"          , construct_object_int          , one));
    print_test(name, "{\"pi\":3.14}"                                    , test_serial_json("serial test cases/object/TESTER_object_float.json"        , "parse test cases/pass/object/object_float.json"        , construct_object_float        , one));
    print_test(name, "{\"abc\":false}"                                  , test_serial_json("serial test cases/object/TESTER_object_false.json"        , "parse test cases/pass/object/object_false.json"        , construct_object_false        , one));
    print_test(name, "{\"abc\":true}"                                   , test_serial_json("serial test cases/object/TESTER_object_true.json"         , "parse test cases/pass/object/object_true.json"         , construct_object_true         , one));
    print_test(name, "{\"abc\":\"def\",\"ghi\":\"jkl\",\"mno\":\"pqr\"}", test_serial_json("serial test cases/object/TESTER_object_strings.json"      , "parse test cases/pass/object/object_strings.json"      , construct_object_strings      , one));
    print_test(name, "{\"name\":\"jake\",\"age\":20,\"height\":1.779}"  , test_serial_json("serial test cases/object/TESTER_object_mixed_values.json" , "parse test cases/pass/object/object_mixed_values.json" , construct_object_mixed_values , one));
    print_test(name, "{\"abc\":{\"def\":123}}"                          , test_serial_json("serial test cases/object/TESTER_object_object.json"       , "parse test cases/pass/object/object_object.json"       , construct_object_object       , one));
    print_test(name, "{\"abc\":{\"def\":{\"ghi\":123}}}"                , test_serial_json("serial test cases/object/TESTER_object_object_object.json", "parse test cases/pass/object/object_object_object.json", construct_object_object_object, one));
    print_test(name, "{\"abc\":[1,2,3]}"                                , test_serial_json("serial test cases/object/TESTER_object_array.json"        , "parse test cases/pass/object/object_array.json"        , construct_object_array        , one));
    print_test(name, "{\"a\":[{\"a\":1},{\"b\":2},{\"c\":3}]}"          , test_serial_json("serial test cases/object/TESTER_object_array_objects.json", "parse test cases/pass/object/object_array_objects.json", construct_object_array_objects, one));
    print_test(name, "{\"a\":[{\"a\":1}]}"                              , test_serial_json("serial test cases/object/TESTER_object_array_object.json" , "parse test cases/pass/object/object_array_object.json" , construct_object_array_object , one));
    print_test(name, "{\"a\":{\"b\":{\"c\":{\"d\":{...{\"z\":{ }...}"   , test_serial_json("serial test cases/object/TESTER_object_recursive.json"    , "parse test cases/pass/object/object_recursive.json"    , construct_object_recursive    , one));

    print_final_summary();

    return 1;
}

int test_serial_array ( char *name )
{

    // Initialized data
    json_value *p_value = 0;

    printf("Scenario: %s\n", name);
    
    print_test(name, "[]"                                  , test_serial_json("serial test cases/array/TESTER_array_empty.json"            , "parse test cases/pass/array/array_empty.json"            , construct_array_empty            , one));
    print_test(name, "[null]"                              , test_serial_json("serial test cases/array/TESTER_array_null.json"             , "parse test cases/pass/array/array_null.json"             , construct_array_null             , one));
    print_test(name, "[null, null, null]"                  , test_serial_json("serial test cases/array/TESTER_array_nulls.json"            , "parse test cases/pass/array/array_nulls.json"            , construct_array_nulls            , one));
    print_test(name, "[true]"                              , test_serial_json("serial test cases/array/TESTER_array_bool.json"             , "parse test cases/pass/array/array_bool.json"             , construct_array_bool             , one));
    print_test(name, "[true, false, true]"                 , test_serial_json("serial test cases/array/TESTER_array_bools.json"            , "parse test cases/pass/array/array_bools.json"            , construct_array_bools            , one));
    print_test(name, "[1]"                                 , test_serial_json("serial test cases/array/TESTER_array_int.json"              , "parse test cases/pass/array/array_int.json"              , construct_array_int              , one));
    print_test(name, "[1, 2, 3]"                           , test_serial_json("serial test cases/array/TESTER_array_ints.json"             , "parse test cases/pass/array/array_ints.json"             , construct_array_ints             , one));
    print_test(name, "[3.14]"                              , test_serial_json("serial test cases/array/TESTER_array_float.json"            , "parse test cases/pass/array/array_float.json"            , construct_array_float            , one));
    print_test(name, "[1.2, 3.4, 5.6]"                     , test_serial_json("serial test cases/array/TESTER_array_floats.json"           , "parse test cases/pass/array/array_floats.json"           , construct_array_floats           , one));
    print_test(name, "[\"\"]"                              , test_serial_json("serial test cases/array/TESTER_array_string_empty.json"     , "parse test cases/pass/array/array_string_empty.json"     , construct_array_string_empty     , one));
    print_test(name, "[\"abc\"]"                           , test_serial_json("serial test cases/array/TESTER_array_string.json"           , "parse test cases/pass/array/array_string.json"           , construct_array_string           , one));
    print_test(name, "[\"abc\", \"def\", \"ghi\"]"         , test_serial_json("serial test cases/array/TESTER_array_strings.json"          , "parse test cases/pass/array/array_strings.json"          , construct_array_strings          , one));
    print_test(name, "[{}]"                                , test_serial_json("serial test cases/array/TESTER_array_object_empty.json"     , "parse test cases/pass/array/array_object_empty.json"     , construct_array_object_empty     , one));
    print_test(name, "[{\"a\":1}]"                         , test_serial_json("serial test cases/array/TESTER_array_object.json"           , "parse test cases/pass/array/array_object.json"           , construct_array_object           , one));
    print_test(name, "[[{\"a\":1}, {\"b\":2}, {\"c\":3}]]" , test_serial_json("serial test cases/array/TESTER_array_objects.json"          , "parse test cases/pass/array/array_objects.json"          , construct_array_objects          , one));
    print_test(name, "[[]]"                                , test_serial_json("serial test cases/array/TESTER_array_array_empty.json"      , "parse test cases/pass/array/array_array_empty.json"      , construct_array_array_empty      , one));
    print_test(name, "[[[]]]"                              , test_serial_json("serial test cases/array/TESTER_array_array_array_empty.json", "parse test cases/pass/array/array_array_array_empty.json", construct_array_array_array_empty, one));
    print_test(name, "[[1, 2, 3],[4, 5, 6],[7, 8, 9]]"     , test_serial_json("serial test cases/array/TESTER_array_matrix.json"           , "parse test cases/pass/array/array_matrix.json"           , construct_array_matrix           , one));
    print_test(name, "[[[1, 2], [3, 4]], [[5, 6], [7, 8]]]", test_serial_json("serial test cases/array/TESTER_array_tensor.json"           , "parse test cases/pass/array/array_tensor.json"           , construct_array_tensor           , one));

    print_final_summary();

    return 1;
}
*/

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    printf("%s %-75s %s\n",scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    {
        if (passed)
        {
            ephemeral_passes++;
        }
        else
        {
            ephemeral_fails++;
        }

        ephemeral_tests++;
    }

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

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    #ifndef NDEBUG
        if ( path == 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL )
        goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer )
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null path provided to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}