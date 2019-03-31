#ifndef _MCP23x17_REG_H_
#define _MCP23x17_REG_H_

/*
 * Description:
 * mapping MCP23S17 registers
 */

// MCP23x17 Registers
#define IODIRA                  0x00
#define IODIRB                  0x01
#define IPOLA                   0x02
#define IPOLB                   0x03
#define GPINTENA                0x04
#define GPINTENB                0x05
#define DEFVALA                 0x06
#define DEFVALB                 0x07
#define INTCONA                 0x08
#define INTCONB                 0x09
#define IOCON                   0x0A
#define IOCONB                  0x0B
#define GPPUA                   0x0C
#define GPPUB                   0x0D
#define INTFA                   0x0E
#define INTFB                   0x0F
#define INTCAPA                 0x10
#define INTCAPB                 0x11
#define GPIOA                   0x12
#define GPIOB                   0x13
#define OLATA                   0x14
#define OLATB                   0x15

// Bits in the IOCON register

#define IOCON_UNUSED            0x01
#define IOCON_INTPOL            0x02
#define IOCON_ODR               0x04
#define IOCON_HAEN              0x08
#define IOCON_DISSLW            0x10
#define IOCON_SEQOP             0x20
#define IOCON_MIRROR            0x40
#define IOCON_BANK_MODE         0x80


// SPI Command codes

#define CMD_WRITE               0x40
#define CMD_READ                0x41

#endif
