#include <stdbool.h>

#include "X28C256.h"
#include "inc/sysctl.h"
#include "inc/gpio.h"
#include "inc/hw_memmap.h"

static void enable_io_as_input(DeviceX28C256_t *device);
static void enable_io_as_output(DeviceX28C256_t *device);
static uint8_t read_io(DeviceX28C256_t *device);
static void set_io(DeviceX28C256_t *device, uint8_t value);
static void set_address(DeviceX28C256_t *device, uint16_t address);
static void write_byte(DeviceX28C256_t *device, uint16_t address, uint8_t value);
static void wait_on_data_polling_bit(DeviceX28C256_t *device, uint16_t address, uint8_t value);

void init_X28C256(DeviceX28C256_t *device) {
    for (uint32_t *port = device->ports; *port != 0; port++) {
        SysCtlPeripheralEnable(*port);
        while (!SysCtlPeripheralReady(*port))
            ;
    }

    GPIOPinTypeGPIOOutput(device->WEn.port, device->WEn.pin);
    GPIOPinTypeGPIOOutput(device->CEn.port, device->CEn.pin);
    GPIOPinTypeGPIOOutput(device->OEn.port, device->OEn.pin);

    GPIOPinWrite(device->WEn.port, device->WEn.pin, device->WEn.pin);
    GPIOPinWrite(device->CEn.port, device->CEn.pin, device->CEn.pin);
    GPIOPinWrite(device->OEn.port, device->OEn.pin, device->OEn.pin);

    for (int i = 0; i < ADDRESS_WIDTH; i++ ) {
        GPIOPinTypeGPIOOutput(device->A[i].port, device->A[i].pin);
    }    
}

void read_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t *buf, int count) {
    enable_io_as_input(device);

    GPIOPinWrite(device->CEn.port, device->CEn.pin, 0);
    GPIOPinWrite(device->OEn.port, device->OEn.pin, 0);

    for (int i = 0; i < count; i++) {
        // update address
        set_address(device, address + i);

        // wait max of OE/CE access time
        SysCtlDelay(5);
        
        // read IO lines
        buf[i] = read_io(device);
        // wait address access time
        SysCtlDelay(5);
        // repeat
    }

    GPIOPinWrite(device->CEn.port, device->CEn.pin, device->CEn.pin);
    GPIOPinWrite(device->OEn.port, device->OEn.pin, device->OEn.pin);
}

void write_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t *buf, int count) {
    int write_count;

    GPIOPinWrite(device->OEn.port, device->OEn.pin, device->OEn.pin);  // ensure OEn is high
    
    int i = 0;
    while (count > 0) {
        enable_io_as_output(device);
        write_count = count < 64 ? count : 64;
        count -= write_count;
        while (write_count-- > 0) {
            write_byte(device, address + i, buf[i]);
            i++;
        }
        // poll for write completion
        GPIOPinWrite(device->CEn.port, device->CEn.pin, device->CEn.pin);
        SysCtlDelay(50000);  // wait for programming cycle to begin
        wait_on_data_polling_bit(device, address + i - 1, buf[i - 1]);
    }
}



void fill_X28C256(DeviceX28C256_t *device, uint16_t address, uint8_t value, int count) {
    int write_count;

    GPIOPinWrite(device->OEn.port, device->OEn.pin, device->OEn.pin);  // ensure OEn is high
    
    int i = 0;
    while (count > 0) {
        enable_io_as_output(device);
        write_count = count < 64 ? count : 64;
        count -= write_count;
        // set address
        while (write_count-- > 0) {
            write_byte(device, address + i++, value);
        }
        // poll for write completion
        GPIOPinWrite(device->CEn.port, device->CEn.pin, device->CEn.pin);
        SysCtlDelay(50000);  // wait for programming cycle to begin
        wait_on_data_polling_bit(device, address + i - 1, value);
    }
}

static void enable_io_as_input(DeviceX28C256_t *device) {
    for (int i = 0; i < DATA_WIDTH; i++) {
        GPIOPinTypeGPIOInput(device->IO[i].port, device->IO[i].pin);
    }
}

static void enable_io_as_output(DeviceX28C256_t *device) {
    for (int i = 0; i < DATA_WIDTH; i++) {
        GPIOPinTypeGPIOOutput(device->IO[i].port, device->IO[i].pin);
    }
}

static uint8_t read_io(DeviceX28C256_t *device) {
    uint8_t data = 0;
    for (int i = 0; i < DATA_WIDTH; i++) {
        data |= (GPIOPinRead(device->IO[i].port, device->IO[i].pin) ? 1 : 0) << i;
    }
    return data;
}

static void set_io(DeviceX28C256_t *device, uint8_t value) {
    for (int i = 0; i < DATA_WIDTH; i++) {
        GPIOPinWrite(device->IO[i].port, device->IO[i].pin, (value & 1) ? device->IO[i].pin : 0);
        value >>= 1;
    }
}

static void set_address(DeviceX28C256_t *device, uint16_t address) {
    for (int i = 0; i < ADDRESS_WIDTH; i++) {
        GPIOPinWrite(device->A[i].port, device->A[i].pin, address & 1 ? device->A[i].pin : 0);
        address >>= 1;
    }

}

static void write_byte(DeviceX28C256_t *device, uint16_t address, uint8_t value) {
    set_address(device, address);
    set_io(device, value);
    GPIOPinWrite(device->CEn.port, device->CEn.pin, 0);
    GPIOPinWrite(device->WEn.port, device->WEn.pin, 0);
    SysCtlDelay(2);  // WE pulse width
    GPIOPinWrite(device->WEn.port, device->WEn.pin, device->WEn.pin);
    GPIOPinWrite(device->CEn.port, device->CEn.pin, device->CEn.pin);
}

static void wait_on_data_polling_bit(DeviceX28C256_t *device, uint16_t address, uint8_t value) {
    uint8_t buf[1];
    uint8_t comp = ~value;

    do {
        SysCtlDelay(50);
        read_X28C256(device, address, buf, 1);
    } while (buf[0] == comp);
}

