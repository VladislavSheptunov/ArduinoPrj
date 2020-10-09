#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>

typedef enum {
    MEM_INIT_FLAG    = 0x00000000,         // 4 byte
    MEM_BACKLIGHT    = MEM_INIT_FLAG + 4,  // 1 byte
    MEM_DISPLAY_TIME = MEM_BACKLIGHT + 1,  // 4 byte
};

void MEM_readData(uint32_t address, uint8_t *data, uint32_t size);
void MEM_writeData(uint32_t address, uint8_t *data, uint32_t size);

uint8_t MEM_readUint8(uint32_t address);
void MEM_writeUint8(uint32_t address, uint8_t data);

uint16_t MEM_readUint16(uint32_t address);
void MEM_writeUint16(uint32_t address, uint16_t data);

uint32_t MEM_readUint32(uint32_t address);
void MEM_writeUint32(uint32_t address, uint32_t data);

void MEM_fill(uint32_t address, uint8_t value, uint32_t size);

#endif // _MEMORY_H_
