#ifndef EXACT_RW_H
#define EXACT_RW_H
#include <unistd.h> 
ssize_t exact_write(int fd, void *buf, size_t nbytes);
ssize_t exact_read(int fd, void *buf, size_t nbytes);



#endif
