#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/hw_memmap.h"
#include "mems/X28C256.h"
#include "tm4c/UART.h"
#include "tm4c/PLL.h"
#include "string.h"
#include "mems/eeprom.h"

extern void EnableInterrupts(void);

uint8_t zerobuf[64] = {0};
char serialrxbuf[1024];
char serialtxbuf[1024];
uint16_t address;
uint16_t size;

DeviceX28C256_t device = {
    .ports = {
        SYSCTL_PERIPH_GPIOA,
        SYSCTL_PERIPH_GPIOB,
        SYSCTL_PERIPH_GPIOC,
        SYSCTL_PERIPH_GPIOD,
        SYSCTL_PERIPH_GPIOE,
        SYSCTL_PERIPH_GPIOF
    },
    .A = {
        {GPIO_PORTB_BASE, GPIO_PIN_5},
        {GPIO_PORTB_BASE, GPIO_PIN_0},
        {GPIO_PORTB_BASE, GPIO_PIN_1},
        {GPIO_PORTE_BASE, GPIO_PIN_4},
        {GPIO_PORTE_BASE, GPIO_PIN_5},
        {GPIO_PORTB_BASE, GPIO_PIN_4},
        {GPIO_PORTA_BASE, GPIO_PIN_5},
        {GPIO_PORTA_BASE, GPIO_PIN_6},
        {GPIO_PORTA_BASE, GPIO_PIN_7},
        {GPIO_PORTF_BASE, GPIO_PIN_1},
        {GPIO_PORTE_BASE, GPIO_PIN_3},
        {GPIO_PORTE_BASE, GPIO_PIN_2},
        {GPIO_PORTE_BASE, GPIO_PIN_1},
        {GPIO_PORTD_BASE, GPIO_PIN_3},
        {GPIO_PORTD_BASE, GPIO_PIN_2},
    },
    .IO = {
        {GPIO_PORTA_BASE, GPIO_PIN_3},
        {GPIO_PORTA_BASE, GPIO_PIN_4},
        {GPIO_PORTB_BASE, GPIO_PIN_6},
        {GPIO_PORTB_BASE, GPIO_PIN_7},
        {GPIO_PORTC_BASE, GPIO_PIN_5},
        {GPIO_PORTC_BASE, GPIO_PIN_4},
        {GPIO_PORTE_BASE, GPIO_PIN_0},
        {GPIO_PORTB_BASE, GPIO_PIN_2},

    },
    .CEn = {GPIO_PORTA_BASE, GPIO_PIN_2},
    .OEn = {GPIO_PORTD_BASE, GPIO_PIN_6},
    .WEn = {GPIO_PORTC_BASE, GPIO_PIN_7},
};

eeprom_t x28c256 = {
    .read = read_X28C256,
    .write = write_X28C256,
    .erase = NULL,
};


int main(void){
    uint8_t  wbuf[] = {0xa2, 0x05, 0xca, 0xd0, 0xfd};
    uint8_t addr[] = {0x00, 0x80};
    

    PLL_Init(Bus80MHz);

    UART_Init();

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);

    init_X28C256(&device);

    while (1) {
        UART_InString(serialrxbuf, sizeof(serialrxbuf));
        if (strcmp(serialrxbuf, "WRITE") == 0) {
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
            UART_OutCRLF();
            address = UART_InUDec();
            UART_OutCRLF();
            size = UART_InUDec();
            UART_OutCRLF();
            size = size < sizeof(serialrxbuf) ? size : sizeof(serialrxbuf);
            UART_InString(serialrxbuf, size);
            write_X28C256(&device, address, (uint8_t *) serialrxbuf, size); 
            GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
            UART_OutCRLF();
            UART_OutUDec(size);
            UART_OutCRLF();
        } else if (strcmp(serialrxbuf, "READ") == 0) {
            UART_OutCRLF();
            address = UART_InUDec();
            UART_OutCRLF();
            size = UART_InUDec();
            UART_OutCRLF();
            size = size < sizeof(serialtxbuf) ? size : sizeof(serialtxbuf);
            read_X28C256(&device, address, (uint8_t *) serialtxbuf, size);
            serialtxbuf[size] = '\0';
            UART_OutString(serialtxbuf);
            UART_OutCRLF();
        } else {
            UART_OutCRLF();
            UART_OutString("Unknown command.");
            UART_OutCRLF();
        }
    }

    while (1)
        ;
}

