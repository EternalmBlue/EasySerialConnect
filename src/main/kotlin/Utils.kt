package `fun`.eternalblue.easyserialconnect

import com.fazecast.jSerialComm.SerialPort
import kotlinx.coroutines.*

object Utils
{
    suspend fun rightSerial(): MutableMap<SerialPort, Boolean>
    {
        val ports = SerialPort.getCommPorts()
        val results = mutableMapOf<SerialPort, Boolean>()
        withContext(Dispatchers.IO)
        {
            coroutineScope()
            {
                ports.map()
                {
                    port ->
                    async()
                    {
                        val result = waitACK(port) // 等待响应
                        println(result.toString())
                        results[port] = result // 存储结果
                    }
                }.awaitAll()
            }
        }
        return results
    }

    private suspend fun waitACK(serialPort: SerialPort): Boolean
    {
        val message = "ACK\n" //需要\n或\r 作为结束
        val response = StringBuilder()
        if (!serialPort.isOpen) serialPort.openPort()
        val job = CoroutineScope(Dispatchers.IO).launch()
        {
            println("$serialPort opened")
            serialPort.outputStream.write((message).toByteArray())
            serialPort.outputStream.flush() // 刷新输出流
            val buffer = ByteArray(1024)
            var bytesRead: Int

            // 等待响应
            serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 0, 0)
            while (isActive)
            {
                println("$serialPort read $buffer")
                bytesRead = serialPort.inputStream.read(buffer)
                if (bytesRead > 0)
                {
                    response.append(String(buffer, 0, bytesRead))
                    if (response.contains("\n"))
                    {
                        break // 找到换行符，结束读取
                    }
                }
            }
        }
        delay(2000)
        job.cancel()
        return if (response.toString().trim() == message)
        {
            true
        } else
        {
            println("Operation timed out for port: ${serialPort.systemPortName}") // 输出超时信息
            return false // 超时或失败则返回 false
        }.also {
            if (serialPort.isOpen)
            {
                serialPort.closePort() // 确保在完成后关闭串口
            }
        }
    }
}