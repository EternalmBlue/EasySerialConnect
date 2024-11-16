#include "2402c.h"

void startI2c(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);//SCL
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);//SDA
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
}

void stopI2c(void)
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
}

void SDA_Input_Mode(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SDA_Output_Mode(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint8_t SDA_Input(void)
{
    return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7);
}

uint8_t waitACK(void)
{
  uint8_t errorTime = 5;
  SDA_Input_Mode();
  HAL_Delay(1);
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
  HAL_Delay(1);
  while (SDA_Input())
  {
    errorTime--;
    if (errorTime == 0)
    {
      SDA_Output_Mode();
      stopI2c();
      return 1;
    }
  }
  SDA_Output_Mode();
  return 0;
}

void SCL_Output(uint8_t value)
{
  if (value)
  {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET);
  }else
  {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET);
  }
}

void I2C_SDA_Output(uint8_t value)
{
  if (value)
  {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
  }else
  {
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
  }
}

void I2C_SendByte(uint8_t data)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    SCL_Output(0);
    HAL_Delay(1);
    I2C_SDA_Output(data & 0x80);
    HAL_Delay(1);
    SCL_Output(1);
    HAL_Delay(1);
  }
  SCL_Output(0);
}

uint8_t I2C_ReceiveByte(void)
{
  uint8_t i = 8;
  uint8_t cR_Byte = 0;
  SDA_Input_Mode();
  while (i--)
  {
    cR_Byte <<= 1;
    SCL_Output(0);
    HAL_Delay(1);
    SCL_Output(1);
    HAL_Delay(1);
    cR_Byte |= SDA_Input();
  }
  SCL_Output(0);
  HAL_Delay(1);
  SDA_Output_Mode();
  return cR_Byte;  
}

void I2C_Write(uint8_t* pucbuf,uint8_t ucAddr,uint8_t ucNum)
{
  startI2c();
  I2C_SendByte(0xa0);
  if(waitACK()) return;
  I2C_SendByte(ucAddr);
  if(waitACK()) return;
  while (ucNum--)
  {
    I2C_SendByte(*pucbuf++);
    if(waitACK()) return;
  }
  stopI2c();
  HAL_Delay(1);
}

void I2C_Read(uint8_t* pubBuf,uint8_t ucAddr,uint8_t ucNum)
{
  startI2c();
  I2C_SendByte(0xa0);
  if(waitACK()) return;
  I2C_SendByte(ucAddr);
  if(waitACK()) return;
  startI2c();
  I2C_SendByte(0xa1);
  if (waitACK()) return;
  while (ucNum--)
  {
    *pubBuf++ = I2C_ReceiveByte();
    if (ucNum)
    {
      I2C_SendAck();
    }else
    {
      I2C_SendNotAck();
    }
  }
  stopI2c();
  HAL_Delay(1);

}

void I2C_SendAck(void)
{
  I2C_SDA_Output(0);
  HAL_Delay(1);
  SCL_Output(1);
  HAL_Delay(1);
  SCL_Output(0);
  HAL_Delay(1);
}

void I2C_SendNotAck(void)
{
  I2C_SDA_Output(1);
  HAL_Delay(1);
  SCL_Output(1);
  HAL_Delay(1);
  SCL_Output(0);
  HAL_Delay(1);
}