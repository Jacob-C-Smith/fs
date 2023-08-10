// Header file 
#include <path/path.h>

// Structure definitions
struct path_s
{

    // Path type
    path_type type; 

    // Path as text
    struct 
    {
        bool    dirty;
        char   *text,
               *text_name;
        size_t  text_max_len,
                text_len,
                i_text_name;
    } full_path;
    
    // Path history
    struct
    {
        void *data;
        enum {
            PATH_HISTORY_STACK,
            PATH_HISTORY_QUEUE
        } type;
    } history;

    // Path 
    union
    {
        bool    dirty;
        size_t  file;      // Size of the file in bytes
        dict   *directory; // Directory contents in a dict as < path_name_text : path_type >
    } data;
};

int path_update_full_path ( path *p_path )
{

    // Argument check
    if ( p_path == (void *) 0 ) goto no_path;

    // Initialized data
    char *slash_name = 1+strrchr(p_path->full_path.text, '/');

    // Get the name of the path
    p_path->full_path.text_name = slash_name;

    // Get the index of the last / character
    p_path->full_path.i_text_name = ( slash_name - p_path->full_path.text_name );

    // Clear dirty flag
    p_path->full_path.dirty = false;

    // Success
    return 1;

    no_path:

    // Error
    return 0;
}

int path_update_data ( path *p_path ) 
{

    // Argument check
    #ifndef NDEBUG
        if ( p_path == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t path_name_len = strlen(p_path->full_path.text_name);
    size_t full_path_len = strlen(p_path->full_path.text);
    struct stat st = { 0 };

    // File status
    stat(p_path->full_path.text, &st);

    // Directory
    if ( (st.st_mode & S_IFMT) == S_IFDIR )
    {

        // Initialized data
        dict *p_dict      = 0;
        DIR  *p_directory = { 0 };
        struct dirent *p_file_directory_entry;

        // State check
        if ( p_path->type != PATH_TYPE_DIRECTORY )
        {

            // Set the type            
            p_path->type = PATH_TYPE_DIRECTORY;
            
            // Allocate a dictionary
            dict_construct(&p_dict, 32);

        }

        // Free the contents of the old dictionary
        if ( dict_clear(p_dict) == 0 ) goto failed_to_clear_dict;

        // Open the directory
        p_directory = opendir(p_path->full_path.text);

        // Error check
        if ( p_directory == NULL ) goto path_not_found;

        // Skip "."
        p_file_directory_entry = readdir(p_directory);

        // Skip ".."
        p_file_directory_entry = readdir(p_directory);

        // Iterate over each path
        while ( ( p_file_directory_entry = readdir(p_directory) ) )
        {


            // Build the path 
            size_t path_len = strlen(p_file_directory_entry->d_name);
            char *p_i_path_text = PATH_REALLOC(0, path_len+1);

            strncpy(p_i_path_text, p_file_directory_entry->d_name, path_len);

            // File status
            stat(p_path->full_path.text, &st);

            // Parse the path as a directory
            if ( (st.st_mode & S_IFMT) == S_IFDIR ) dict_add(p_dict, p_i_path_text, (void *)PATH_TYPE_DIRECTORY);

            // Parse the path as a file
            else dict_add(p_dict, p_i_path_text, (void *)PATH_TYPE_FILE);

        }

        p_path->type           = PATH_TYPE_DIRECTORY;
        p_path->data.directory = p_dict;
    }

    // File
    else if ( (st.st_mode & S_IFMT) == S_IFREG )
    {
        p_path->type = PATH_TYPE_FILE;

        p_path->data.file = st.st_size;
    }

    // Clear the dirty bit
    p_path->data.dirty = false;

    //  Success
    return 1;

    no_path:
    failed_to_clear_dict:
    path_not_found:

    return 0;
}

void pfn_path_free( void *vp_memory )
{

    return;
}

int path_create ( path **pp_path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_path == (void *) 0 ) goto no_path;    
    #endif

    // Initialized data
    path *p_path = PATH_REALLOC(0, sizeof(path));
    
    // Error check
    if ( p_path == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_path, 0, sizeof(path));

    // Return 
    *pp_path = p_path;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int path_open ( path **pp_path, const char *path_string )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_path == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    path *p_path = 0;

    // Navigate 
    path_navigate(&p_path, path_string);

    // Return a pointer to the caller
    *pp_path = p_path;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    
        // path
        {
            failed_to_navigate:
                #ifndef NDEBUG
                    printf("[path] path_navigate returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
    // Platform dependent initialized data
    #ifdef _WIN64

    // Windows initialized data
    WIN32_FIND_DATA  find_data      = { 0 };
    char             _path[2048]    = "";
    HANDLE           find_handle    = 0;
    DWORD            path_attribute = 0;
    DWORD            low_file_size  = 0;
    DWORD            high_file_size = 0;
    size_t           file_size      = 0;
    char            *full_path      = 0;
    char            *name           = 0;
    #else

    // Unix like initialized data
    struct dirent *p_file_directory_entry;
    DIR           *p_directory;
    char           _path[2048]    = "";
    size_t         file_size      = 0;
    struct stat    st             = { 0 };
    #endif
    
    // Platform dependent path_open implementation
    #ifdef _WIN64

    ////////////////////////////
    // Windows implementation //
    ////////////////////////////

    // Allocate memory for the path
    p_path = PATH_REALLOC(0, sizeof(path));

    // Error check
    if ( p_path == (void *) 0 )
        goto no_mem;
    
    // Zero set
    memset(p_path, 0, sizeof(path));

    // Copy the user specified path into an internal buffer
    sprintf(_path, "%s", path_string);

    // Get information about the file
    path_attribute = GetFileAttributesA(_path);

    // Error check
    if ( path_attribute == INVALID_FILE_ATTRIBUTES )
        goto failed_to_get_file_attributes;

    // Parse the file as a directory
    if ( path_attribute & FILE_ATTRIBUTE_DIRECTORY )
    {
        // Initialized data
        
        size_t  directory_item_count = 0;
        path   *p_directory_contents = 0;
        path   *p_contents           = 0;
               
        // Copy the name
        {

            // Initialized data
            // TODO: This could be better optimized
            char   *after_backsl = strrchr(_path, '\\'),
                   *after_fwdsl  = strrchr(_path, '/'),
                   *after_sl     = 1 + ( after_backsl > after_fwdsl ) ? after_fwdsl : after_backsl;
            size_t  len          = strlen(after_sl)-1;
            
            trim:
            if ( len == 0 )
            {
                *after_sl = '\0';
                after_backsl = strrchr(_path, '\\'),
                after_fwdsl  = strrchr(_path, '/'),
                after_sl     = ( after_backsl < after_fwdsl ) ? after_fwdsl : after_backsl;
                len          = strlen(after_sl) - 1; 
                goto trim;
            }

            // Allocate memory for name
            name = PATH_REALLOC(0, sizeof(char) * ( len + 1 )); // Don't need to allocate a null terminator

            // Error check
            if ( name == (void *) 0 )
                goto no_mem;
            
            // Copy the name
            strncpy(name, after_sl+1, len);

            // Write a null terminator
            name[len] = '\0';
        }

        // Copy the full path
        {

            // Initialized data
            size_t len = strlen(_path);
            
            // Allocate memory for the full path string
            full_path = PATH_REALLOC(0, sizeof(char) * ( len + 1024 ));

            // Error check
            if ( full_path == (void *) 0 )
                goto no_mem;
            
            // Copy the full path
            strncpy(full_path, _path, len);

            // Write a null terminator
            full_path[len] = '\0';

            // Write a null terminator
            strncat(full_path, "\\*.*", 4);
        }   

        // Recursively construct paths from the directory
        {

            

            // Get the first file in a directory
            find_handle = FindFirstFileA(full_path, &find_data);

            // Error check
            if ( find_handle == INVALID_HANDLE_VALUE )
                goto path_not_found;//printf("Path not found: [%s]\n", path);

            // Parse the path as a directory
            if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                
                // Skip past "." and ".."
                if ( FindNextFileA(find_handle, &find_data) == 0 )
                    goto find_next_file_failed;

                // Iterate over each item in the directory
                while ( FindNextFileA(find_handle, &find_data) )
                {

                    // Initialized data
                    path *i_path = PATH_REALLOC(0, sizeof(path));

                    // Error check
                    if ( i_path == (void *) 0 )
                        goto no_mem;

                    // Increment the directory counter
                    directory_item_count++;

                    // Zero set
                    memset(i_path, 0, sizeof(path));

                    // Build the file path 
                    sprintf(full_path, "%s\\%s", path_string, find_data.cFileName);

                    // Parse the item as a directory
                    if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                    {
                        //printf("Directory: %s\n", full_path);
                        //fflush(stdout);
                        path_open(&p_contents, p_path, full_path);
                        path_append_file(&p_directory_contents, p_contents);
                    }

                    // Parse the item as a file
                    else
                    {
                        //printf("File: %s\n", full_path);
                        //fflush(stdout);
                        path_open(&p_contents, p_path, full_path);
                        path_append_file(&p_directory_contents, p_contents);
                    }
                }

                // Clean up the scope
                FindClose(find_handle);
                
            }
        }

        // Populate the path struct
        *p_path = (path)
        {
            .full_path               = full_path,
            .name                    = name,
            .type                    = PATH_TYPE_DIRECTORY,
            .p_parent_directory_path = p_parent_path,
            .directory = {
                .p_directory_contents   = p_directory_contents,
                .directory_content_size = directory_item_count
            }
        };

        // Clean up
        //CloseHandle(file);
    }

    // Parse the path as a file
    else
    {

        // Initialized data
        HANDLE file = CreateFile(
            _path,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        // Error check
        if ( file == INVALID_HANDLE_VALUE )
            goto failed_to_open_file;

        // Copy the name
        {

            // Initialized data
            // TODO: This could be better optimized
            char   *after_backsl = strrchr(_path, '\\'),
                   *after_fwdsl  = strrchr(_path, '/'),
                   *after_sl     = 1 + ( after_backsl > after_fwdsl ) ? after_fwdsl : after_backsl;
            size_t  len          = strlen(after_sl)-1;

            // Allocate memory for name
            name = PATH_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( name == (void *) 0 )
                goto no_mem;
            
            // Copy the name
            strncpy(name, after_sl+1, len);

            // Write a null terminator
            name[len] = '\0';
        }

        // Copy the full path
        {

            // Initialized data
            size_t len = strlen(_path);

            // Allocate memory for the full path string
            full_path = PATH_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( full_path == (void *) 0 )
                goto no_mem;
            
            // Copy the full path
            strncpy(full_path, _path, len);

            // Write a null terminator
            full_path[len] = '\0';
        }   

        // Get the file size
        {
            low_file_size = GetFileSize(file, &high_file_size);

            file_size = (high_file_size << 32) | low_file_size;
        }

        // Populate the path struct
        *p_path = (path)
        {
            .full_path               = full_path,
            .name                    = name,
            .type                    = PATH_TYPE_FILE,
            .p_next_content          = 0,
            .p_parent_directory_path = p_parent_path,
            .file = {
                .file_size = file_size
            }
        };

        // Clean up
        CloseHandle(file);
    }

    #else

    /////////////////////////
    // UNIX implementation //
    /////////////////////////

    // Initialized data
    char *full_path = 0;

    // Allocate a path
    if ( path_create(&p_path) == (void *) 0 ) goto no_mem;
    
    // Copy the path parameter into an internal buffer
    sprintf(_path, "%s", path_string);

    // Get the path properties
    if ( stat(path_string, &st) != 0 ) goto failed_to_get_file_properties;

    // Copy the full path
    {

        // Initialized data
        size_t len = strlen(_path);
        
        // Allocate memory for the full path string
        full_path = PATH_REALLOC(0, sizeof(char) * ( len + 1024 ));

        // Error check
        if ( full_path == (void *) 0 ) goto no_mem;
        
        // Copy the full path
        strncpy(full_path, _path, len);

        // Write a null terminator
        full_path[len] = '\0';

        // Write a null terminator
        strncat(full_path, "\\*.*", 4);
    }  

    // Parse the path as a directory
    if ( (st.st_mode & S_IFMT) == S_IFDIR )
    {

        // Initialized data
        size_t  directory_item_count = 0,
                _path_len = strlen(_path);
        char   *directory_name       = 0;
        dict   *directory_dict       = 0;

        // Copy the name
        {

            // Initialized data
            // TODO: This could be better optimized
            char   *after_backsl = strrchr(_path, '\\'),
                   *after_fwdsl  = strrchr(_path, '/'),
                   *after_sl     = 1 + ( after_backsl > after_fwdsl ) ? after_fwdsl : after_backsl;
            size_t  len          = strlen(after_sl)-1;
            
            trim:
            if ( len == 0 )
            {
                *after_sl = '\0';
                after_backsl = strrchr(_path, '\\'),
                after_fwdsl  = strrchr(_path, '/'),
                after_sl     = ( after_backsl < after_fwdsl ) ? after_fwdsl : after_backsl;
                len          = strlen(after_sl) - 1; 
                goto trim;
            }

            // Allocate memory for name
            directory_name = PATH_REALLOC(0, sizeof(char) * ( len + 1 )); // Don't need to allocate a null terminator

            // Error check
            if ( directory_name == (void *) 0 ) goto no_mem;
            
            // Copy the name
            strncpy(directory_name, after_sl+1, len);

            // Write a null terminator
            directory_name[len] = '\0';
        }

        // Copy the full path
        {
            
            // Allocate memory for the full path string
            full_path = PATH_REALLOC(0, sizeof(char) * ( _path_len + 1024 ));

            // Error check
            if ( full_path == (void *) 0 ) goto no_mem;
            
            // Copy the full path
            strncpy(full_path, _path, _path_len);

            // Write a null terminator
            full_path[_path_len] = '\0';
        }   

        // Construct paths from the directory contents
        {

            // Open the directory
            p_directory = opendir(full_path);

            // Error check
            if ( p_directory == NULL ) goto path_not_found;
            
            // Skip "."
            p_file_directory_entry = readdir(p_directory);

            // Skip ".."
            p_file_directory_entry = readdir(p_directory);

            dict_construct(&directory_dict, 32);

            // Iterate over each path
            while ( ( p_file_directory_entry = readdir(p_directory) ) )
            {

                // Initialized data
                path *i_path = PATH_REALLOC(0, sizeof(path));

                // Error check
                if ( i_path == (void *) 0 ) goto no_mem;

                // Zero set
                memset(i_path, 0, sizeof(path));

                // Build the path 
                sprintf(full_path, "%s/%s", path_string, p_file_directory_entry->d_name);

                // Parse the path as a directory
                if ( (st.st_mode & S_IFMT) == S_IFDIR ) dict_add(directory_dict, full_path, (void *)PATH_TYPE_DIRECTORY);

                // Parse the path as a file
                else dict_add(directory_dict, full_path, (void *)PATH_TYPE_FILE);
                
            }

            // Write a null terminator
            full_path[_path_len] = '\0';
        }

        // Populate the path struct
        *p_path = (path)
        {
            .full_path = full_path,
            .name      = directory_name,
            .type      = PATH_TYPE_DIRECTORY,
            .directory = directory_dict
        };
    }

    // Parse the path as a file
    else if ( (st.st_mode & S_IFMT) == S_IFREG )
    {

        // Initialized data
        char *full_path = 0;
        char *file_name = 0;

        // Copy the name
        {

            // Initialized data
            // TODO: This could be better optimized
            char   *after_backsl = strrchr(_path, '\\'),
                   *after_fwdsl  = strrchr(_path, '/'),
                   *after_sl     = 1 + ( after_backsl > after_fwdsl ) ? after_fwdsl : after_backsl;
            size_t  len          = strlen(after_sl)-1;
            
            // Allocate memory for the file name
            file_name = PATH_REALLOC(0, ( len + 1 ) * sizeof(char));

            // Error check
            if ( file_name == (void *) 0 ) goto no_mem;
            
            // Copy the name
            strncpy(file_name, after_sl+1, len);

            // Write a null terminator
            file_name[len] = '\0';
        }

        // Copy the full path
        {

            // Initialized data
            size_t len = strlen(_path);

            // Allocate memory for the full path string
            full_path = PATH_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( full_path == (void *) 0 ) goto no_mem;
            
            // Copy the full path
            strncpy(full_path, _path, len);

            // Write a null terminator
            full_path[len] = '\0';
        }   

        // Populate the path struct
        *p_path = (path)
        {
            .full_path = full_path,
            .name      = file_name,
            .type      = PATH_TYPE_FILE,
            .file      = st.st_size
        };
    }
    #endif

*/
const char *path_name_text ( const path *const p_path )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_path == (void *) 0 ) goto no_path;
    #endif

    // 
    if ( p_path->full_path.dirty ) path_update_full_path( p_path );

    // Success
    return p_path->full_path.text_name;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

path_type path_type_path ( const path *const p_path )
{
    // Argument check
    #ifndef NDEBUG
        if ( p_path == (void *) 0 ) goto no_path;
    #endif

    // Success
    return p_path->type;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

const char *path_full_path_text ( const path *const p_path )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_path == (void *) 0 ) goto no_path;
    #endif

    // State check
    if ( p_path->full_path.dirty == false ) 
        if ( path_update_full_path(p_path) == 0 )
            goto failed_to_update_full_path; 

    // Success
    return p_path->full_path.text;

    failed_to_update_full_path:
    // Error
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

bool path_is_file ( const path * const p_path )
{

    // Argument check
    if ( p_path == (void *) 0 ) goto no_path;

    // Success
    return ( p_path->type == PATH_TYPE_FILE) ? true : false;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int path_file_size ( const path *const p_path, size_t *p_size_in_bytes )
{
    
    // Argument check
    #ifndef NDEBUG
        if ( p_path          == (void *) 0 ) goto no_path;
        if ( p_size_in_bytes == (void *) 0 ) goto no_size_in_bytes;
    #endif

    // Error checking
    if ( p_path->type != PATH_TYPE_FILE ) goto wrong_path_type;

    // State checking
    if ( p_path->data.dirty == true ) path_update_full_path(p_path);

    // Return
    *p_size_in_bytes = p_path->data.file;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_size_in_bytes:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_size_in_bytes\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    
        // Path errors
        {
            wrong_path_type:
                #ifndef NDEBUG
                    printf("[path] Parameter \"p_path\" was of wrong type in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t path_directory_content_names ( const path *const p_path, const char **const names )
{

    // Return
    return dict_keys(p_path->data.directory, names );
}

size_t path_directory_content_types ( const path *const p_path, const path_type *const types )
{

    // Return
    return dict_values(p_path->data.directory, types );
}

int path_navigate ( path **pp_path, const char *path_text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_path   == (void *) 0 ) goto no_path;
        if ( path_text == (void *) 0 ) goto no_path_text;
        if ( *pp_path  == (void *) 0 ) goto construct_path;
    #endif

    // Navigate branch
    {

        // Initialized data
        path *p_path = (void *) *pp_path;
        
        // Special cases

        // Maybe "." or ".." or invalid
        if ( path_text[0] == '.' ) 
        {

            // ".."
            if ( path_text[1] == '.' ) 
            {

                if ( path_text[2] == '/')
                {

                }
                else if ( path_text[2] == '\0')
                {

                }
                else
                    return 0;
            }
            else if ( path_text[1] == '\0' )
            {

            }
            else
                return 0;
        }

        // Error check
        if ( p_path->type != PATH_TYPE_DIRECTORY ) goto wrong_path_type;

        // Build the path
        {

            // Write a null terminator
            p_path->full_path.text[p_path->full_path.i_text_name] = '\0';
        }

        path_built:;
        
        // Return
        //return path_navigate( pp_path, i_path_text );
    }

    // Constructor branch
    construct_path:
    {

        // Initialized data
        path   *p_path        = 0;
        size_t  path_text_len = strlen(path_text);

        // Allocate a path
        if ( path_create(&p_path) == 0 ) goto failed_to_allocate_path;

        // Allocate memory for path
        p_path->full_path.text_max_len = 1024+1+path_text_len;
        p_path->full_path.i_text_name = path_text_len;
        p_path->full_path.text = PATH_REALLOC(0, sizeof(char)*(p_path->full_path.text_max_len));

        // Copy the string
        strncpy(p_path->full_path.text, path_text, p_path->full_path.text_max_len);

        path_update_full_path(p_path);
        path_update_data(p_path);

        // Return a pointer to the path
        *pp_path = p_path;

        // Success
        return 1;
    }
    
    failed_to_clear_dict:
    path_not_found:
    no_mem:
    failed_to_allocate_path:
    wrong_path_type:

    // Error
    return 0;
    
    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path_text:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"path_text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // path errors
        {
            failed_to_store_path:
                #ifndef NDEBUG
                    printf("[path] Failed to store path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_navigate:
                #ifndef NDEBUG
                    printf("[path] Failed to navigate in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int path_create_file ( path *p_path, const char *file_name )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_path    == (void *) 0 ) goto no_path;
        if ( file_name == (void *) 0 ) goto no_file_name;
    #endif

    // Initialized data
    char _full_file_path[MAX_FILE_PATH_LEN] = { 0 };
    FILE *f = 0;
    int snprintf_r = 0;

    // Construct the file path
    snprintf_r = snprintf( &_full_file_path, MAX_FILE_PATH_LEN, "%s/%s", p_path->full_path, file_name );
                        
    // Debug mode error checking
    #ifndef NDEBUG
        if ( snprintf_r > MAX_FILE_PATH_LEN ) goto buffer_not_big_enough;
    #endif
    
    // Open the file
    f = fopen(_full_file_path, "w+");

    // Error check
    if ( f == 0 ) goto failed_to_open_file;

    // Close the file
    (void) fclose(f);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_file_name:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"file_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Path errors
        {
            buffer_not_big_enough:
                #ifndef NDEBUG
                    printf("[path] Program has exceeded specified buffer size of \"%d\" in call to function \"%s\"\n       Considier increasing MAX_FILE_PATH_LEN, or refactoring", MAX_FILE_PATH_LEN, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    
        // Standard library errors
        {
            failed_to_open_file:
                #ifndef NDEBUG
                    printf("[path] Failed to create file \"%s\" in call to function \"%s\"\n", _full_file_path, __FUNCTION__);
                #endif

                // Error
                return 0;          
        }
    }
}

int path_create_directory( path *p_path, const char *directory_name )
{
    
    // Argument check
    #ifndef NDEBUG
        if ( p_path    == (void *) 0 ) goto no_path;
        if ( directory_name == (void *) 0 ) goto no_file_name;
    #endif

    // Initialized data
    char _full_file_path[MAX_FILE_PATH_LEN] = { 0 };
    FILE *f = 0;
    int snprintf_r = 0;

    // Construct the file path
    snprintf_r = snprintf( &_full_file_path, MAX_FILE_PATH_LEN, "%s/%s", p_path->full_path, directory_name );
                        
    // Debug mode error checking
    #ifndef NDEBUG
        if ( snprintf_r > MAX_FILE_PATH_LEN ) goto buffer_not_big_enough;
    #endif
    
    // Platform specific implementation
    #ifdef _WIN64
        ////////////////////////////
        // Windows implementation //
        ////////////////////////////
    #else

        /////////////////////////
        // UNIX implementation //
        /////////////////////////

        // Make a directory
        if ( mkdir(&_full_file_path, 777) != 0 ) goto failed_to_create_directory;

    #endif

    // Success
    return 1;

    failed_to_create_directory:

    // Error
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_file_name:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"file_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Path errors
        {
            buffer_not_big_enough:
                #ifndef NDEBUG
                    printf("[path] Program has exceeded specified buffer size of \"%d\" in call to function \"%s\"\n       Considier increasing MAX_FILE_PATH_LEN, or refactoring", MAX_FILE_PATH_LEN, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    
        // Standard library errors
        {
            failed_to_open_file:
                #ifndef NDEBUG
                    printf("[path] Failed to create file \"%s\" in call to function \"%s\"\n", _full_file_path, __FUNCTION__);
                #endif

                // Error
                return 0;          
        }
    }
}

int path_remove ( path *p_path, const char *path_name )
{



    // Success
    return 1;
}

int path_directory_foreach_i ( const path *const p_path, void (*pfn_path_iter)(const char *full_path, path_type type, size_t i))
{

    // Argument check
    #ifndef NDEBUG
        if ( p_path        == (void *) 0 ) goto no_path;
        if ( pfn_path_iter == (void *) 0 ) goto no_path_iter;
    #endif

    // Initialized data
    size_t directory_content_count = 0;
    const char ** pp_path_names = 0;
    const path_type *pp_path_types = 0;

    // Error checking
    if ( p_path->type != PATH_TYPE_DIRECTORY ) goto path_is_not_a_directory;

    // Get the quantity of contents in the directory
    directory_content_count = path_directory_content_names(p_path, 0);

    // State check
    if ( directory_content_count == 0 ) return 1;

    // Allocate memory for path names 
    if ( PATH_REALLOC(pp_path_names, directory_content_count * sizeof(const char *)) == 0 ) goto no_mem;

    // Allocate memory for path types
    if ( PATH_REALLOC(pp_path_types, directory_content_count * sizeof(path_type)) == 0 ) goto no_mem;

    // Get the name of each path
    if ( path_directory_content_names(p_path, pp_path_names) == 0 ) goto failed_to_get_directory_content_names;

    // Get the types of each path
    if ( path_directory_content_types(p_path, pp_path_types) == 0 ) goto failed_to_get_directory_content_types;

    // Iterate over each path in the directory
    for (size_t i = 0; i < directory_content_count; i++)

        // Call the function
        pfn_path_iter(pp_path_names[i], pp_path_types[i], i);
    
    // Clean up
    (void) PATH_REALLOC(pp_path_names, 0);
    (void) PATH_REALLOC(pp_path_types, 0);

    // Success
    return 1;

    no_mem:
        return 0;

    // Error handling
    {
        
        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                    
            no_path_iter:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pfn_path_iter\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }

        // path errors
        {
            path_is_not_a_directory:
                #ifndef NDEBUG
                    printf("[path] Parameter \"p_path\" is not of type directory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            failed_to_get_directory_content_names:
                #ifndef NDEBUG
                    printf("[path] Failed to get names of directory contents in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_directory_content_types:
                #ifndef NDEBUG
                    printf("[path] Failed to get types of directory contents in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int path_close ( path **pp_path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_path == (void *) 0 ) goto no_path;
    #endif
    
    // Initialized data
    path *p_path = *pp_path;

    // Error check
    if ( p_path == (void *) 0 ) goto pointer_to_null_pointer;

    // No more pointer for caller
    *pp_path = 0;

    // TODO

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"pp_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            pointer_to_null_pointer:
                #ifndef NDEBUG
                    printf("[path] Parameter \"pp_path\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }

        // Standard library errors
        {
            failed_to_free:
                #ifndef NDEBUG
                    printf("[Standard Library] Call to \"realloc\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}