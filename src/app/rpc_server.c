#include "app/rpc_server.h"

static void default_handle_request_cb(struct rpc_Server *server);
static void (*handle_request_cb)(struct rpc_Server *server) = &default_handle_request_cb;
static void default_idle_cb(struct rpc_Server *server);
static void (*idle_cb)(struct rpc_Server *server) = &default_idle_cb; 

void rpc_ServerInit(struct rpc_Server *server) {
    server->status = RPC_IDLE;
}

void rpc_ServerTick(struct rpc_Server *server) {
    if (server->status == RPC_REQUEST_PENDING) {
        handle_request_cb(server);
    } else {
        idle_cb(server);
    } 
}

void rpc_SetServerIdleCallback(void (*cb)(struct rpc_Server *)) {
    if (cb) {
        idle_cb = cb;
    }
}
void rpc_ServerRequest(struct rpc_Server *server) {
    server->status = RPC_REQUEST_PENDING;
}
int rpc_RequestPending(struct rpc_Server *server) {
    return server->status == RPC_REQUEST_PENDING;
}

enum rpc_Status rpc_GetStatus(struct rpc_Server *server) {
    return server->status;
}

void rpc_WriteBuffer(struct rpc_Server *server, const char *buf, size_t count) {
    if (count > RPC_BUFFER_SIZE) {
        server->status = RPC_BUFFER_OVERFLOW;
    } else {
        for (size_t i = 0; i < count; i++) {
            server->buf[i] = buf[i];
        }
    }
}

void rpc_ReadBuffer(struct rpc_Server *server, char *buf, size_t count) {
    if (count > RPC_BUFFER_SIZE) {
        server->status = RPC_BUFFER_OVERFLOW;
    } else {
        for (size_t i = 0; i < count; i++) {
            buf[i] = server->buf[i];
        }
    }
}

static void  default_handle_request_cb(struct rpc_Server *server) {
    return;
}

static void default_idle_cb(struct rpc_Server *server) {
    /* go to sleep */
    return;
}

