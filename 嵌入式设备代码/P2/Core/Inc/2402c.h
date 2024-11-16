#include "main.h"
void startI2c(void);
void stopI2c(void);
void SDA_Input_Mode(void);
void SDA_Output_Mode(void);
uint8_t SDA_Input(void);
uint8_t waitACK(void);
void SCL_Output(uint8_t value);
void I2C_SDA_Output(uint8_t value);
void I2C_SendByte(uint8_t data);
uint8_t I2C_ReceiveByte(void);
void I2C_Write(uint8_t* pucbuf,uint8_t ucAddr,uint8_t unNum);
void I2C_Read(uint8_t* pubBuf,uint8_t ucAddr,uint8_t ucNum);
void I2C_SendNotAck(void);
void I2C_SendAck(void);
