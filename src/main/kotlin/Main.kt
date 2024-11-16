package `fun`.eternalblue.easyserialconnect

import com.fazecast.jSerialComm.SerialPort

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
suspend fun main()
{
    val ports = SerialPort.getCommPorts()
    if (ports.isEmpty())
    {
        println("无可用串口")
        return
    }
    println("可用串口:")
    ports.forEach()
    {
        port ->
        print("${port.systemPortName} ")
    }
    println("\n正在判断正确的连接设备")
    println("需要嵌入式设备响应指定的message")
    println("正在等待回复")
    val result = Utils.rightSerial(ports).filter { it.value }
    if (result.isEmpty()) println("未检测到响应的设备").also { return }
    result.forEach()
    {
        println("响应端口: ${it.key.systemPortName}")
        val serialPort = SerialPort.getCommPort(it.key.systemPortName)
        serialPort.baudRate = 9600
        while (true)
        {
            println("键入quit退出程序")
            print("请输入你想要发送的字符(不用添加\\n或\\r 如:ACK):")
            var message = readln()
            message += "\n"
            if (message.contains("quit", true)) return
            if (!serialPort.isOpen) serialPort.openPort()
            serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 3000, 3000)
            try
            {
                serialPort.outputStream.write(message.toByteArray())
                serialPort.outputStream.flush()
                println("发送成功")
            } catch (e: Exception)
            {
                println("发送失败")
                println(e.message)
                return
            }
        }
    }
}





