#include <stddef.h>

typedef struct eeprom {
    int (*read)(void *, unsigned long int, char *, size_t);
    int (*write)(void *, unsigned long int, char *, size_t);
    int (*erase)(void *);
} eeprom_t;
