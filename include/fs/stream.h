/** !
 * Include header for stream abstraction
 * 
 * @file fs/stream.h 
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

// fs library
#include <fs/fs.h>

// Platform dependant includes
#ifdef _WIN64
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

// Preprocessor macros
#define MAX_FILE_PATH_LEN 4096

// Forward declarations
struct stream_s;

// Type definitions
typedef struct stream_s stream;

int stream_constuct ( stream **pp_stream, const char *p_path, size_t buffer_size, size_t offset );