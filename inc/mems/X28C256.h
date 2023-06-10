#include <stdint.h>

#define ADDRESS_WIDTH 15
#define DATA_WIDTH 8

typedef struct _GpioPin {
    uint32_t port;
    uint8_t pin;
} GpioPin_t;

typedef struct _DeviceX28C256 {
    uint32_t ports[10];
    GpioPin_t A[ADDRESS_WIDTH];
    GpioPin_t IO[DATA_WIDTH];
    GpioPin_t WEn;
    GpioPin_t OEn;
    GpioPin_t CEn;
} DeviceX28C256_t;

int init_X28C256(DeviceX28C256_t *device);

int read_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t *buf, int count);

int write_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t *buf, int count);

int fill_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t value, int count);

int erase_X28C256(DeviceX28C256_t *device);

