#include "file_io.h"
#include "type_compat.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

/* See file_io.h for description */
ssize_t writefd(const int fd, const char *const buffer, const size_t buffer_size) {
   ssize_t bytes_written = 0;
   ssize_t bytes_offset = 0;

   if (INVALID_FD == fd) {
      printf("NOTICE: Invalid file descriptor given\n");
   } else if (NULL != buffer) {
      do {
         bytes_written = write(fd, &buffer[bytes_offset], (buffer_size - bytes_offset));
         if (INVALID_IO == bytes_written) {
            bytes_offset = INVALID_IO;
            break;
         } else if (bytes_written >= 0) {
            bytes_offset += bytes_written;
         }
      } while ( (bytes_written >= 0) && (buffer_size != bytes_offset) );
   }

   return bytes_offset;
}

/* See file_io.h for description */
ssize_t readfd(const int fd, char **const output_buffer, size_t *buffer_size) {
   ssize_t bytes_offset  = 0;
   struct stat file_stat;

   *buffer_size = INVALID_IO;
   if (INVALID_FD == fd) {
      printf("NOTICE: Invalid file descriptor given\n");
   } else if (INVALID_IO == fstat(fd, &file_stat)) {
      printf("NOTICE: Getting file status failed - %m\n");
   } else {
      ssize_t bytes_read = 0;
      const long file_size = (long)file_stat.st_size;
      char file_buff[file_size];
      imemset(file_buff, 0, sizeof(file_buff));

      do {
         bytes_read = read(fd, &file_buff[bytes_offset], (sizeof(file_buff) - bytes_offset) - 1);
         if(INVALID_IO == bytes_read) {
            // Error handle
            bytes_offset = INVALID_IO;
            break;
         } else if ( bytes_read > 0 ) {
            bytes_offset += bytes_read;
         }
      } while ( (bytes_read > 0) && (file_size > bytes_offset) );

      if (INVALID_IO != bytes_read) {
         *output_buffer = calloc(0, bytes_offset);
         if (NULL != *output_buffer) {
            istrncpy(*output_buffer, file_buff, bytes_offset);
            *buffer_size = bytes_offset;
         } else {
            printf("EMERG: Allocating memory for the output buffer failed - %m\n");
            bytes_offset = INVALID_IO;
         }
      }
   }

   return bytes_offset;
}
