#ifndef FUNC_H
#define FUNC_H

int read_func(const char *filename, int num_bytes, char *buffer, size_t buffer_size);
int write_func(const char *filename, const char *content, int num_bytes);

#endif // FUNC_H