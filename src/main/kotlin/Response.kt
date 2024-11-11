package `fun`.eternalblue.easyserialconnect

import kotlinx.coroutines.Job

object Response
{
    val portResponse : MutableMap<Job,Boolean> = mutableMapOf()
}