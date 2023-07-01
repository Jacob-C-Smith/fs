#include <path/path.h>

// Platform dependent includes
#ifdef _WIN64
#include <windows.h>
#include <process.h>
#else
#include <sys/dirent.h>
#include <sys/stat.h>
#endif

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
    WIN32_FIND_DATA find_data   = { 0 };
    char            _path[2048] = "";
    HANDLE          find_handle = 0;
    #else

    // Unix like initialized data
    #endif
    
    // Platform dependent path_open implementation
    #ifdef _WIN64
    
    // Windows implementation
    
    // Copy the user specified path into an internal buffer
    sprintf(_path, "%s\\*.*", path_string);

    // Get the first file in a directory
    find_handle = FindFirstFile(_path, &find_data);
    
    // Error check
    if ( find_handle == INVALID_HANDLE_VALUE )
        goto path_not_found;//printf("Path not found: [%s]\n", path);

    // Parse the path as a directory
    if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
    {
        
        // Skip past "." and ".."
        if ( FindNextFile(find_handle, &find_data) == 0 )
            goto find_next_file_failed;

        // Iterate over each item in the directory
        while ( FindNextFile(find_handle, &find_data) )
        {

            // Initialized data
            path *i_path = realloc(0, sizeof(path));

            // Error check
            if ( i_path == (void *) 0 )
                goto no_mem;

            // Build the file path 
            sprintf(_path, "%s\\%s", path_string, find_data.cFileName);

            // Parse the item as a directory
            if ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
                printf("Directory: %s\n", _path);

                path_open(pp_path, p_path, _path);
            }

            // Parse the item as a file
            else
            {
                printf("File: %s\n", _path);
                path_open(pp_path, p_path, _path);
            }
        }

        // Clean up the scope
        FindClose(find_handle);
    }

    // Parse the path as a file
    {

    }

    

    
    #else
    #endif

    // Success
    return 1;

    path_not_found:
    find_next_file_failed:
    no_mem:

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
/** !
 * Make a directory in the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the directory
 * 
 * @sa path_make_file
 * @sa path_remove
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_make_directory( path *p_path, const char *path );

/** !
 * Make a file in the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the file
 * 
 * @sa path_make_directory
 * @sa path_remove
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_make_file( path *p_path, const char *path );

/** !
 * Remove a file/directory from the specified path
 * 
 * @param p_path the specified path
 * @param path the name of the file/directory
 * 
 * @sa path_make_directory
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_remove ( path *p_path, const char *path );

// Accessors
/** !
 *  Is the path a directory or a file?
 *
 * @param pp_directory return
 *
 * @sa directory_file_size
 * @sa directory_name
 *
 * @return true if directory item is file, false if directory item is directory. False on error
 */
DLLEXPORT bool path_is_file ( path *p_path );

/** !
 *  Returns the number of items in a directory, if p_path is a directory, else the size of the file
 *
 * @param p_path the directory item
 *
 * @sa directory_is_file
 * @sa directory_name
 *
 * @return the size of the path on success, 0 on error
 */
DLLEXPORT size_t path_size ( path *p_path );

/** !
 *  Returns the directory item name
 *
 * @param p_directory_item the directory item
 *
 * @sa directory_is_file
 * @sa directory_file_size
 *
 * @return the name of the file on success, 0 on error
 */
DLLEXPORT const char *path_name ( path *p_path );

// Destructors
/** !
 * Close a path
 * 
 * @param pp_path pointer to path pointer
 * 
 * @sa path_open
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int path_close ( path **pp_path );



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