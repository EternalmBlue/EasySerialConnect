# EasySerialConnect

EasySerialConnect 是我学习嵌入式的UART/USART 后发现网络上缺少 简单易用的 通过UART串口发送字符串的程序。故闲着没事自写了一个（我好不容易写好了嵌入式的代码逻辑，但是没找到软件实现电脑与嵌入式设备的通信) 本程序主要用于学习（应该不能有人又学java/kotlin 还学c 还搞嵌入式吧？？.....）

## 特性

- **串口列表**：自动检测并列出所有可用的串口设备。
- **串口检测**：通过向嵌入式设备代码中加入指定代码逻辑(详情见下) 可以实现设备与本程序的联合 自动识别响应的嵌入式设备。
- **数据传输**：向识别出的嵌入式设备发送字符串。

## 使用
需要java8以上环境

## 运行
   ```bash
   java -jar EasySerialConnect.jar
   ```
## C语言(嵌入式端)代码
使用cubeMX配置usart
   ```C
/* USER CODE BEGIN PFP */
void usartFunc(void);
/* USER CODE END PFP */
//声明函数
```
```C
void usartFunc(void) 
{
    // 读取UART接收的数据
    if (usartReceive(&usartBuffer) > 0) 
    {
        // 检查是否可以存储接收到的数据
        if (usartDataNum < sizeof(usartData)) 
        {
            if (usartBuffer == '\n' || usartBuffer == '\r') 
            { 
                usartData[usartDataNum] = '\0'; 
                usartDataNum = 0; // 重置计数器，准备接收下一条
                if (strcmp(usartData,"ACK") == 0)
                {
                  usartSend();
                }
                //lcdClearScreen(0x0000);
                return;
            }
            usartData[usartDataNum++] = usartBuffer; // 将接收到的字节存储到 usartData 中
        }
    }
}
//以上代码放入main.c中，记得声明函数
```
```C
  while (1)
  {
    /* USER CODE END WHILE */
    usartFunc(); //加入到嵌入式设备主循环
    lcdFunc();
    keyFunc();
    /* USER CODE BEGIN 3 */
  }
```

初始化usart后在usart.c中加入两个方法(记得在usart.h中声明方法)
````C
/* USER CODE BEGIN 1 */
uint8_t usartReceive(uint8_t* usartBuffer) 
{
    if (HAL_UART_Receive(&huart1, usartBuffer, 1, 0) == HAL_OK) 
    {
        return 1; // 返回成功接收到的数据字节数（1字节）
    }
    return 0; // 如果没有成功接收，返回0
}

void usartSend(void)
{
    char* data = "ACK\n"; // 使用字符指针
    for (int i = 0; i < 4; i++)
    {
      HAL_UART_Transmit(&huart1, (uint8_t*)data+i, 1,0);
      HAL_Delay(10);
    }
}
/* USER CODE END 1 */
````
项目所有代码已放入<<嵌入式设备代码>> 文件中
