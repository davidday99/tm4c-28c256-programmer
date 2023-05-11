#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "inc/sysctl.h"
#include "inc/gpio.h"
#include "inc/hw_memmap.h"
#include "X28C256.h"

uint8_t zerobuf[64] = {0};
uint8_t buf[1000];

int main(void){
    uint8_t  wbuf[] = {0xa2, 0x05, 0xca, 0xd0, 0xfd};
    uint8_t addr[] = {0x00, 0x80};
    
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

    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC))
        ;

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);

    init_X28C256(&device);
    for (int i = 0; i < 512; i++)
        write_X28C256(&device, i*64, zerobuf, 64);
    write_X28C256(&device, 0, wbuf, sizeof(wbuf));
    write_X28C256(&device, 0x7ffc, addr, sizeof(addr));
    read_X28C256(&device, 0x7ffc, buf, 4);
    fill_X28C256(&device, sizeof(wbuf), 0xea, 50);
    read_X28C256(&device, 0, buf, 100);

    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);

    while (1)
        ;
}

