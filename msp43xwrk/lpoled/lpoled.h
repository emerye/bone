#ifndef _LPOLED_H_
#define	_LPOLED_H_


typedef enum SPI_ModeEnum{
    IDLE_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    TIMEOUT_MODE
} SPI_Mode;


void __delay_cycles(unsigned long);

SPI_Mode SPI_Master_Write(uint8_t *reg_data, uint8_t count);




#endif
