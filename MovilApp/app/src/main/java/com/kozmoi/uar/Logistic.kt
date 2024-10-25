package com.kozmoi.uar

import android.content.Context
import android.net.ConnectivityManager
import android.net.NetworkCapabilities
import android.os.Build
import android.util.Log
import kotlinx.coroutines.*
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.PrintWriter
import java.net.Socket

object Logistic {
    private var socket: Socket? = null
    private var output: PrintWriter? = null
    private var input: BufferedReader? = null
    private val coroutineScope = CoroutineScope(Dispatchers.IO)

    // Conectar al servidor
    fun connectToServer(
        context: Context,
        ip: String,
        port: String,
        onDataReceived: (String, String) -> Unit,
        onConnected: () -> Unit,
        onDisconnected: () -> Unit
    ) {
        coroutineScope.launch {
            try {
                socket = Socket(ip, port.toInt())
                output = PrintWriter(socket?.getOutputStream(), true)
                input = BufferedReader(InputStreamReader(socket?.getInputStream()))

                // Enviar información del dispositivo al servidor
                output?.println("IP: ${socket?.inetAddress?.hostAddress} Dispositivo: ${Build.MODEL} Android Version: ${Build.VERSION.RELEASE}")

                withContext(Dispatchers.Main) {
                    onConnected() // Notificar que se conectó exitosamente
                }

                // Escuchar datos del servidor
                while (true) {
                    val data = input?.readLine()
                    if (data != null) {
                        withContext(Dispatchers.Main) {
                            onDataReceived(data, "")
                        }
                    } else {
                        break
                    }
                }

            } catch (e: Exception) {
                e.printStackTrace()
                withContext(Dispatchers.Main) {
                    onDataReceived("", "Error: ${e.message}")
                }
            } finally {
                disconnect() // Asegúrate de desconectar al salir
                withContext(Dispatchers.Main) {
                    onDisconnected() // Notificar que se desconectó
                }
            }
        }
    }

    // Enviar datos al servidor
    fun sendData(data: String) {
        coroutineScope.launch {
            withContext(Dispatchers.IO) {
                output?.println(data)
            }
        }
    }

    // Desconectar del servidor
    fun disconnect() {
        coroutineScope.launch {
            try {
                input?.close()
                output?.close()
                socket?.close()
                socket = null
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }
}
