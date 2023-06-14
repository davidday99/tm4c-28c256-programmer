#ifndef __RPC_H__ 
#define __RPC_H__

enum rpc_Command {
    RPC_CMD_READ,
    RPC_CMD_WRITE,
    RPC_CMD_ERASE,
};

struct rpc_RequestPacket {
    enum rpc_Command command;
    int argc;
    char buf[];
};

union rpc_Request {
    struct rpc_RequestPacket request;
    char buf[sizeof(struct rpc_RequestPacket)];
};


#endif /* __RPC_H__ */

