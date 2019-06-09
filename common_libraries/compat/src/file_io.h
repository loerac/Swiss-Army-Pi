#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include <stdlib.h>
#include <unistd.h>

#define INVALID_FD -1
#define INVALID_IO -1

/**
 * INPUT:
 *    fd - Valid file descriptor used to read the file
 *    output_buffer - Contents of the file to write
 *    output_buffer_size - Size of the file that is being written
 * RETURN:
 *    On success, the total bytes written will be returned.
 *    On failure, -1 is returned.
 * DESCRIPTION:
 *    Writes into a file.
 **/
ssize_t writefd(const int fd, const char *const buffer, const size_t buffer_size);

/**
 * INPUT:
 *    fd - Valid file descriptor used to read the file
 *    output_buffer - Contents of the file
 *    output_buffer_size - Size of the file
 * OUTPUT:
 *    output_buffer - On success, it'll allocate memory to hold the
 *                      content of the file
 *    output_buffer_size - On success, it'll be the size of the file
 * RETURN:
 *    On success, the total bytes read from the file, output_buffer will
 *       hold the content of the file, and output_buffer_size will be the
 *       size of the file.
 *    On failure, -1 is returned, output_buffer will be NULL, and
 *       output_buffer_size will be -1
 * DESCRIPTION:
 *    Reads a file into a buffer.
 *
 *    **User is responsible to free memory.**
 **/
ssize_t readfd(const int fd, char **const buffer, size_t *buffer_size);

#endif /* _FILE_IO_H_ */

