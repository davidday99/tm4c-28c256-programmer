#include "app/rpc_server.h"

static void default_handle_request_cb(struct rpc_Server *com);
static void (*handle_request_cb)(struct rpc_Server *com) = &default_handle_request_cb;
static void default_idle_cb(struct rpc_Server *com);
static void (*idle_cb)(struct rpc_Server *com) = &default_idle_cb; 

void rpc_ServerTick(struct rpc_Server *com) {
    if (com->status == RPC_REQUEST_PENDING) {
        handle_request_cb(com);
    } else {
        idle_cb(com);
    } 
}



void rpc_SetServerIdleCallback(void (*cb)(void)) {
    if (cb) {
        idle_cb = cb;
    }
}
void rpc_ServerRequest(struct rpc_Server *com) {
    com->status = RPC_REQUEST_PENDING;
}
int rpc_RequestPending(struct rpc_Server *com) {
    return com->status == RPC_REQUEST_PENDING;
}

enum rpc_Status rpc_GetStatus(struct rpc_Server *com) {
    return com->status;
}

void rpc_WriteBuffer(struct rpc_Server *com, const char *buf, size_t count) {
    if (count > RPC_BUFFER_SIZE) {
        com->status = RPC_BUFFER_OVERFLOW;
    } else {
        for (size_t i = 0; i < count; i++) {
            com->buf[i] = buf[i];
        }
    }
}

void rpc_ReadBuffer(struct rpc_Server *com, char *buf, size_t count) {
    if (count > RPC_BUFFER_SIZE) {
        com->status = RPC_BUFFER_OVERFLOW;
    } else {
        for (size_t i = 0; i < count; i++) {
            buf[i] = com->buf[i];
        }
    }
}

static void  default_handle_request_cb(struct rpc_Server *com) {
    return;
}

static void default_idle_cb(struct rpc_Server *com) {
    /* go to sleep */
    return;
}

