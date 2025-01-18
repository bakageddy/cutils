#ifndef CUTIL_DEBUG_H
#define CUTIL_DEBUG_H

#include <stdio.h>
#define INFO(fmt_msg, ...) fprintf(stdout, "[INFO] %s:%d " fmt_msg, __FILE__, __LINE__, ##__VA_ARGS__)

#endif // CUTIL_DEBUG_H
