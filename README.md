# EasySerialConnect

EasySerialConnect 是我学习嵌入式的UART/USART 后发现网络上缺少 简单易用的 通过UART串口发送字符串的程序。故闲着没事自写了一个（我好不容易写好了嵌入式的代码逻辑，但是没找到软件实现电脑与嵌入式设备的通信) 本程序主要用于学习（应该不能有人又学java/kotlin 还学c 还搞嵌入式吧？？.....）

## 特性

- **串口列表**：自动检测并列出所有可用的串口设备。
- **串口检测**：通过向嵌入式设备代码中加入指定代码逻辑(详情见下) 可以实现设备与本程序的联合 自动识别响应的嵌入式设备。
- **数据传输**：向识别出的嵌入式设备发送字符串。

## 安装

1. 下载并安装 [Java JRE 11](https://www.oracle.com/java/technologies/javase-jre11-downloads.html) 或更高版本。
2. 下载并解压 EasySerialConnect 的发布包。
3. 在终端或命令提示符中导航到解压的文件夹，运行以下命令启动程序：

   ```bash
   java -jar EasySerialConnect-1.0-SNAPSHOT.jar
