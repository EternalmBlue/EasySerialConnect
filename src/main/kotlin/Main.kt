package `fun`.eternalblue.easyserialconnect

import com.fazecast.jSerialComm.SerialPort
import kotlinx.coroutines.*
import kotlinx.coroutines.flow.MutableStateFlow
import kotlin.coroutines.coroutineContext

//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
suspend fun main()
{
    val ports = SerialPort.getCommPorts()
    println("可用串口:")
    ports.forEach()
    {
        port -> println(port.systemPortName)
    }
    println(Utils.rightSerial())

    // 选择串口
    val serialPort = SerialPort.getCommPort("COM3") // 替换为您的串口名称
    serialPort.baudRate = 9600 // 设定波特率
    if (serialPort.openPort()) {
        println("串口已打开")
    } else {
        println("无法打开串口")
        return
    }

    val scanner = readln()
    while (true)
    {
        print("请输入要显示在 LCD 上的字符串（输入 'exit' 退出）：")
        if (scanner.equals("exit", ignoreCase = true))
        {
            break
        }

        // 发送字符串
        serialPort.outputStream.write((scanner + "\n").toByteArray())
        serialPort.outputStream.flush() // 刷新输出流
        println("已发送: $scanner")
    }

    serialPort.closePort()
    println("串口已关闭")
}




