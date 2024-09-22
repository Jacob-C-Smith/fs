#include <fs/stream.h>

struct stream_s
{
    size_t size;
    unsigned char *p_buffer;
    int fd;
    struct 
    {
        int(*pfn_stream_read)();
        int(*pfn_stream_write)();
        int(*pfn_stream_close)();
    } functions;
};

int stream_constuct ( stream **pp_stream, const char *p_path, size_t buffer_size, size_t offset )
{

    // Argument check
    if ( pp_stream == (void *) 0 ) goto no_stream;

    // Initialized data
    stream *p_stream = PATH_REALLOC(0, sizeof(stream));

    // Open the file
    p_stream->fd = open(p_path, O_RDONLY);

    p_stream->size = buffer_size;

    p_stream->p_buffer = mmap(0, buffer_size, PROT_READ, MAP_SHARED, p_stream->fd, offset);

    if ( p_stream->p_buffer == MAP_FAILED ) return 0;

    *pp_stream = p_stream;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_stream:
                #ifndef NDEBUG

                #endif

                // Error
                return 0;
        }
    }
}