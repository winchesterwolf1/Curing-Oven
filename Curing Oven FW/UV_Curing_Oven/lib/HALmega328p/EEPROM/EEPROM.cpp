#include "EEPROM.h"
#include <avr/io.h>
#include <avr/interrupt.h>


EEPROM::EEPROM() {

}

EEPROM::~EEPROM() {

}

// Function to initialize EEPROM (if needed)
void EEPROM::Init(void) {

}

// Function to write a byte to EEPROM at specified address
void EEPROM::WriteByte(uint16_t address, uint8_t data) {
    // Create a critical section
    cli();

    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));

    /* Set up address and Data Registers */
    EEAR = address;
    EEDR = data;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);

    // End the critical section
    sei();
}

// Function to read a byte from EEPROM at specified address
uint8_t EEPROM::ReadByte(uint16_t address) {
    // Create a critical section
    cli();

    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE));
    /* Set up address register */
    EEAR = address;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* save data from Data Register */
    uint8_t result = EEDR;

    // End the critical section
    sei();

    // Return the result
    return result;
}

// Function to write multiple bytes to EEPROM starting at specified address
void EEPROM::WriteBytes(uint16_t address, uint8_t* data, uint16_t length) {
    for(int i=0; i < length; i++)
    {
        WriteByte(address+i, data[i]);
    }
}

// Function to read multiple bytes from EEPROM starting at specified address
void EEPROM::ReadBytes(uint16_t address, uint8_t* data_out, uint16_t length) {
    for(int i=0; i < length; i++)
    {
        *(data_out + i) = ReadByte(address+i);
    }
}
