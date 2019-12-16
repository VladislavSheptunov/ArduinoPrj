#include "memory.h"

#include <EEPROM.h>

void MEM_readData(uint32_t address, uint8_t *data, uint32_t size) {
    for (uint32_t it = 0; it < size; it++) {
        data[it] = EEPROM.read((int)(address + it));
    }
}

void MEM_writeData(uint32_t address, uint8_t *data, uint32_t size) {
    for (uint32_t it = 0; it < size; it++) {
        EEPROM.write(address + it, data[it]);
    }
}

uint8_t MEM_readUint8(uint32_t address) {
    uint8_t value;
    MEM_readData(address, &value, sizeof(value));

    return value;
}

void MEM_writeUint8(uint32_t address, uint8_t data) {
    MEM_writeData(address, &data, sizeof(data));
}

uint16_t MEM_readUint16(uint32_t address) {
    uint16_t value;
    MEM_readData(address, (uint8_t*)&value, sizeof(value));

    return value;
}

void MEM_writeUint16(uint32_t address, uint16_t data) {
    MEM_writeData(address, (uint8_t*)&data, sizeof(data));
}

uint32_t MEM_readUint32(uint32_t address) {
    uint32_t value;
    MEM_readData(address, (uint8_t*)&value, sizeof(value));

    return value;
}

void MEM_writeUint32(uint32_t address, uint32_t data) {
    MEM_writeData(address, (uint8_t*)&data, sizeof(data));
}

void MEM_fill(uint32_t address, uint8_t value, uint32_t size) {
    for (uint32_t it = 0; it < size; it++) {
        EEPROM.write(address + it, value);
    }
}
