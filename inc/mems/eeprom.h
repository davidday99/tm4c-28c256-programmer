#include <stddef.h>

struct mem_Device {
    int (*read)(void *, unsigned long int, char *, size_t);
    int (*write)(void *, unsigned long int, char *, size_t);
    int (*erase)(void *);
};
