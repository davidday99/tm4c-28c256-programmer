#include "string.h"

int strcmp(const char *__s1, const char *__s2) {
    for (;*__s1 == *__s2 && *__s1 != '\0'; __s1++, __s2++)
        ;
    return *__s1 - *__s2;
}

