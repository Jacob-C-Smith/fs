#include <path/path.h>

// Platform dependent includes
#ifdef _WIN64
#include <windows.h>
#include <process.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

// Helper functions
int path_append_file ( path **pp_head, path *p_path )
{

    // Argument check
    if ( pp_head == (void *) 0 ) goto no_head;
    if ( p_path  == (void *) 0 ) goto no_path;
    
    // State check
    if ( *pp_head == (void *) 0 )
        goto no_head_p;

    // Initialized data
    path *i_path = *pp_head;

    // Walk to the end of the list
    while ( i_path->p_next_content )
    { 
        i_path = i_path->p_next_content;
    };

    // Append the content
    i_path->p_next_content = p_path;

    // Avoid circles
    i_path->p_next_content->p_next_content = 0;

    // Success
    return 1;

    // Set the head
    no_head_p:
        
        *pp_head = p_path;

        // Success
        return 1;

    // Error handling
    {

        // Arugment errors
        {

            no_head:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_head\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    printf("[path] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

// Constructors
int path_open ( path **pp_path, path *p_parent_path, const char *path_string )
{

    // Argument check
    {
        if ( pp_path == (void *) 0 ) goto no_path;
    }

    // Platform independent initialized data
    path *p_path = 0;

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
    char             _path[2048]    = "";
    size_t           file_size      = 0;
    char            *full_path      = 0;
    char            *name           = 0;
    struct stat      st             = { 0 };

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
            .type                    = path_type_directory,
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
            .type                    = path_type_file,
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

    // Allocate memory for the path
    p_path = PATH_REALLOC(0, sizeof(path));

    // Error check
    if ( p_path == (void *) 0 )
        goto no_mem;
    
    // Zero set
    memset(p_path, 0, sizeof(path));
    
    // Copy the user specified path into an internal buffer
    sprintf(_path, "%s", path_string);

    // Get the file size
    stat(path_string, &st);
    file_size = st.st_size;
    
    // Get information about the file
    //path_attribute = GetFileAttributesA(_path);

    // Error check
    //if ( path_attribute == INVALID_FILE_ATTRIBUTES )
    //    goto failed_to_get_file_attributes;

    // Parse the file as a directory
    if ( (st.st_mode & S_IFMT) == S_IFDIR ) // path_attribute & FILE_ATTRIBUTE_DIRECTORY 
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
        }   

        // Recursively construct paths from the directory
        {

            // Open the directory
            p_directory = opendir(full_path);

            // Error check
            if ( p_directory == NULL )
                ;//goto path_not_found;
            
            p_file_directory_entry = readdir(p_directory);
            p_file_directory_entry = readdir(p_directory);

            while ( (p_file_directory_entry = readdir(p_directory)) )
            {

                // Initialized data
                path        *i_path = PATH_REALLOC(0, sizeof(path));
                struct stat  stN    = { 0 };

                // Error check
                if ( i_path == (void *) 0 )
                    goto no_mem;

                // Increment the directory counter
                directory_item_count++;

                // Zero set
                memset(i_path, 0, sizeof(path));

                // Build the file path 
                sprintf(full_path, "%s/%s", path_string, p_file_directory_entry->d_name);

                // Parse the item as a directory
                if ( (st.st_mode & S_IFMT) == S_IFDIR ) 
                {
                    printf("Directory: %s\n", full_path);
                    fflush(stdout);
                    path_open(&p_contents, p_path, full_path);
                    path_append_file(&p_directory_contents, p_contents);
                }
                // Parse the item as a file
                else
                {
                    printf("File: %s\n", full_path);
                    fflush(stdout);
                    path_open(&p_contents, p_path, full_path);
                    path_append_file(&p_directory_contents, p_contents);
                }
               
                fflush(stdout);
            }
                
            // Skip past "." and ".."
            //    if ( FindNextFileA(find_handle, &find_data) == 0 )
            //        goto find_next_file_failed;

            // Iterate over each item in the directory
            //    while ( FindNextFileA(find_handle, &find_data) )
                //{

                   
            //     

                   
            //     
            //     

                   
            //     

                   
            //     

                   
            //     

                   
            //     
                   
                   
                   
            //     
            //     
                   

                   
            //     
                   
                   
                   
            //     
            //     
                   
                //}

                // Clean up the scope
            //    FindClose(find_handle);
                
            }

        // Populate the path struct
        *p_path = (path)
        {
            .full_path               = full_path,
            .name                    = name,
            .type                    = path_type_directory,
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
    else if ( (st.st_mode & S_IFMT) == S_IFREG )
    {

        // Initialized data
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
        file_size = st.st_size;

        // Populate the path struct
        *p_path = (path)
        {
            .full_path               = full_path,
            .name                    = name,
            .type                    = path_type_file,
            .p_next_content          = 0,
            .p_parent_directory_path = p_parent_path,
            .file = {
                .file_size = file_size
            }
        };
    }
    // Default

    
    #endif

    // Return a pointer to the caller
    *pp_path = p_path;

    // Success
    return 1;

    no_mem:
    path_not_found:
    find_next_file_failed:
    failed_to_open_file:
    failed_to_get_file_attributes:
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
        }
    }
}

// File system operations
int path_make_directory( path *p_path, const char *path );

int path_make_file( path *p_path, const char *path );

int path_remove ( path *p_path, const char *path );

bool path_is_file ( path *p_path )
{

    // Argument check
    if ( p_path == (void *) 0 )
        goto no_path;

    // Success
    return ( p_path->type == path_type_file) ? true : false;

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

size_t path_size ( path *p_path )
{
    
    // Argument check
    if ( p_path == (void *) 0 )
        goto no_path;

    // Success
    return ( p_path->type == path_type_file ) ? p_path->file.file_size : p_path->directory.directory_content_size;

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

const char *path_name ( path *p_path )
{
    // Argument check
    if ( p_path == (void *) 0 )
        goto no_path;

    // Success
    return p_path->name;

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

// Destructors
int path_close ( path **pp_path )
{

    // Argument check
    if ( pp_path == (void *) 0 )
        goto no_path;
    
    // Initialized data
    path *p_path = *pp_path;

    // Error check
    if ( p_path == (void *) 0 )
        goto pointer_to_null_pointer;

    // No more pointer for caller
    *pp_path = 0;

    // Free the full path string
    if ( p_path->full_path )
        if ( PATH_REALLOC(p_path->full_path, 0) )
            goto failed_to_free;

    // Free the name
    if ( PATH_REALLOC(p_path->name, 0) )
        goto failed_to_free;

    // Free the next content
    if ( p_path->p_next_content )
        if ( path_close(&p_path->p_next_content) == 0 )
            goto failed_to_recursively_free_path_contents;

    // Free the path
    if ( PATH_REALLOC(p_path, 0) ) 
        goto failed_to_free;

    // Success
    return 1;

    failed_to_recursively_free_path_contents:
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

/*
int directory_create ( const char *name )
{

    // Create the directory
    CreateDirectory(name, NULL);

    // Success
    return 1;
}

int directory_remove ( directory *p_directory, const char *name )
{

    // Success
    return 1;
}

int directory_open ( directory **pp_directory, const char *path )
{

    // Argument check
    if ( pp_directory == (void *) 0 ) goto no_directory;

    // Initialized data
    dict   *p_dict         = 0;
    size_t  path_len       = strlen(path);
    char    buf     [4096] = { 0 };

    // Platform dependent initialized data
    #ifdef _WIN64
        // DIR           *dir = opendir(path);
        // struct dirent *entry;
        // struct stat    st; 
    #else
        DIR           *dir = opendir(path);
        struct dirent *entry;
        struct stat    st; 
    #endif

    // Allocate a dictionary
    if ( dict_construct(&p_dict, 32) == 0 )
        goto failed_to_allocate_dict;

    // Platform dependent directory populaton
    #ifdef _WIN64
        // Error checking
        //if ( dir == NULL )
        //    goto failed_to_open_file;
    
        // Iterate over each directory
        //while ((entry = readdir(dir)) != NULL)
        //{
        //
        //    // Initialized data
        //    size_t                   file_size     = 0,
        //                             file_name_len = strlen(entry->d_name);
        //    char                    *file_name     = calloc(path_len+file_name_len+1, sizeof(char)),
        //                             file_type     = entry->d_type;
        //    struct directory_item_s *item          = calloc(1,sizeof(struct directory_item_s));
        //    // TODO: Error checking
        //    // Copy the path to the buffer
        //    strncpy(file_name, path, path_len);
        //    file_name[path_len] = '/';
        //    strncpy(&file_name[path_len+1], entry->d_name, file_name_len);
        //    stat(file_name, &st);
        //    file_size = st.st_size;
        //    // Store the item to the allocated memory
        //    *item = (struct directory_item_s){ .path = file_name, .size = file_size, .is_directory = (file_type == DT_DIR) };
        //    // Store the allocated memory in the dictionary
        //    dict_add(p_dict, file_name, item);
        //}
            
        //closedir(dir);
    #else

        // Error checking
        if ( dir == NULL )
            goto failed_to_open_file;
    
        // Iterate over each directory
        while ((entry = readdir(dir)) != NULL)
        {

            // Initialized data
            size_t                   file_size     = 0,
                                     file_name_len = strlen(entry->d_name);
            char                    *file_name     = calloc(path_len+file_name_len+1, sizeof(char)),
                                     file_type     = entry->d_type;
            struct directory_item_s *item          = calloc(1,sizeof(struct directory_item_s));

            // TODO: Error checking
            // Copy the path to the buffer
            strncpy(file_name, path, path_len);
            file_name[path_len] = '/';
            strncpy(&file_name[path_len+1], entry->d_name, file_name_len);
            stat(file_name, &st);

            file_size = st.st_size;

            // Store the item to the allocated memory
            *item = (struct directory_item_s){ .path = file_name, .size = file_size, .is_directory = (file_type == DT_DIR) };

            // Store the allocated memory in the dictionary
            dict_add(p_dict, file_name, item);
        }
            
        closedir(dir);
        
    #endif

    // Return the directory 
    *pp_directory = p_dict;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_directory:
                #ifndef NDEBUG
                    printf("[directory] Null pointer provided for parameter \"pp_directory\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Dict errors
        {
            failed_to_allocate_dict:
                #ifndef NDEBUG
                    printf("[directory] Failed to allocate dict in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        failed_to_open_file:
        // TODO:
        // Error
        return 0;

    }
}

bool directory_is_file ( directory_item *p_directory_item )
{
    
    // Argument check
    if ( p_directory_item == (void *) 0 ) goto no_directory_item;

    // Success
    return !p_directory_item->is_directory;

    // Error handling
    {

        // Argument errors
        {
            no_directory_item:
                #ifndef NDEBUG
                    printf("[directory] Null pointer provided for parameter \"p_directory_item\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return false;
        }
    }
}

size_t directory_file_size ( directory_item *p_directory_item )
{

    // Argument check
    if ( p_directory_item == (void *) 0 ) goto no_directory_item;

    // Success
    return p_directory_item->is_directory ? 0 : p_directory_item->size;

    // Error handling
    {

        // Argument errors
        {
            no_directory_item:
                #ifndef NDEBUG
                    printf("[directory] Null pointer provided for parameter \"p_directory_item\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

const char *directory_name ( directory_item *p_directory_item )
{

    // Argument check
    if ( p_directory_item == (void *) 0 ) goto no_directory_item;

    // Success
    return p_directory_item->path;

    // Error handling
    {

        // Argument errors
        {
            no_directory_item:
                #ifndef NDEBUG
                    printf("[directory] Null pointer provided for parameter \"p_directory_item\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return (char *) 0;
        }
    }
}

*/