#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>

class EEPROM
{
public:

    EEPROM();

    ~EEPROM();

    // Public methods
    void Init();

    void WriteByte(uint16_t address, uint8_t data);

    void WriteBytes(uint16_t address, uint8_t* data, uint16_t length);

    uint8_t ReadByte(uint16_t address);

    void ReadBytes(uint16_t address, uint8_t* data_out, uint16_t length);

private:
    // Private members
    
    // Private methods
};

#endif // EEPROM_H