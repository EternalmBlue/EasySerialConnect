package `fun`.eternalblue.easyserialconnect

import com.fazecast.jSerialComm.SerialPort
import kotlinx.coroutines.*

object Utils
{
    //返回所有端口和所有端口的响应情况
    suspend fun rightSerial(ports: Array<SerialPort>): MutableMap<SerialPort, Boolean>
    {
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
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_SEMI_BLOCKING, 3000, 3000)
        withContext(Dispatchers.IO)
        {
            try
            {
                serialPort.outputStream.write(message.toByteArray())
                serialPort.outputStream.flush()
            } catch (e: Exception) {
                return@withContext false
            }
        }
        val job = CoroutineScope(Dispatchers.IO).launch()
        {
            val buffer = ByteArray(2048)
            var bytesRead: Int = -1
            // 等待响应
            serialPort.baudRate = 9600
            while (isActive)
            {
                try
                {
                    bytesRead = serialPort.inputStream.read(buffer)
                } catch (_: Exception)
                {

                }
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
        delay(4000)
        job.cancelAndJoin()
        return if (response.toString() == "ACK\n")
        {
            true .also { if(serialPort.isOpen) serialPort.closePort() }
        }else
        {
            false .also { if(serialPort.isOpen) serialPort.closePort() }
        }
    }
}
