#ifndef __RPC_SERVER_H__
#define __RPC_SERVER_H__

#include <stddef.h>

#define RPC_BUFFER_SIZE 1024

enum rpc_Status {
    RPC_REQUEST_PENDING,
    RPC_SUCCESS,
    RPC_ERROR,
    RPC_IDLE,
    RPC_TIMEOUT,
    RPC_INVALID_ARG,
    RPC_BUFFER_OVERFLOW,
};

struct rpc_Server {
    enum rpc_Status status;
    char buf[RPC_BUFFER_SIZE];
};

void rpc_ServerTick(struct rpc_Server *com);
void rpc_ServerIdle(struct rpc_Server *com);
void rpc_ServerNotify(struct rpc_Server *com);
int rpc_RequestWaiting(struct rpc_Server *com);
enum rpc_Status rpc_GetStatus(struct rpc_Server *com);
void rpc_WriteBuffer(struct rpc_Server *com, const char *buf, size_t count);
void rpc_ReadBuffer(struct rpc_Server *com, char *buf, size_t count);

#endif /* __RPC_SERVER_H__ */

