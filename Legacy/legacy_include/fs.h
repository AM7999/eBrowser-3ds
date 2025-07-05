#ifndef FS_H
#define FS_H

char *readFile(char *filename);

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

#endif