#ifndef __MEM_H__ 
#define  __MEM_H__
#include <stddef.h>

struct mem_Device {
    int (*read)(void *, unsigned long int, char *, size_t);
    int (*write)(void *, unsigned long int, const char *, size_t);
    int (*erase)(void *);
};

#endif /* __MEM_H__ */

