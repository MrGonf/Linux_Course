#ifndef FUNC_H
#define FUNC_H

#include <sys/types.h>
#include <sys/stat.h>

void print_file_infor(const struct stat *info, const char *filename);

#endif
