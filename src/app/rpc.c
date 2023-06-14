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

void rpc_HandleCommand(struct rpc_RequestPacket *rq, struct mem_Device *mem) {
    unsigned long int addr = 0;
    size_t count = 0;
    char *buf = 0;
    switch (rq->command) {
        case RPC_CMD_READ:
            rpc_ReadCommand(mem, NULL, addr, rq->buf, count); 
            break;
        case RPC_CMD_WRITE:
            rpc_WriteCommand(mem, NULL, addr, buf, count); 
            break;
        case RPC_CMD_ERASE:
            rpc_EraseCommand(mem, NULL);
            break;

    }
}

