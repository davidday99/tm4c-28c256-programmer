#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <stddef.h>
#include "conf.h"

char *read_buf(char *buf);
char *write_buf(char *buf, char src, size_t size);
int get_rx_buf_count(void);
int get_tx_buf_count(void);

#endif /* __TRANSPORT_H__ */

