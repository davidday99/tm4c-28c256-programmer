#include <stddef.h>
#include "app/rpc.h"
#include "app/rpc_server.h"
#include "mems/mem.h"

void rpc_ReadCommand(struct mem_Device *mem,
                    void *arg,
                    unsigned long int addr,
                    char *buf,
                    size_t count) {
    mem->read(arg, addr, buf, count);
}

void rpc_WriteCommand(struct mem_Device *mem,
                    void *arg,
                    unsigned long int addr,
                    const char *buf,
                    size_t count) {
    mem->write(arg, addr, buf, count);
}

void rpc_EraseCommand(struct mem_Device *mem, void *arg) {
    mem->erase(arg); 
}

