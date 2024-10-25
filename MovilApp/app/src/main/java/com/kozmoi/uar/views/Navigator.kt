package com.kozmoi.uar.views

import androidx.compose.runtime.*
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.platform.LocalContext
import com.kozmoi.uar.Logistic
import com.kozmoi.uar.helpers.ScreenState

@Preview
@Composable
fun Navigator() {
    var currentScreen by remember { mutableStateOf(ScreenState.Connection) }
    var ipAddress by remember { mutableStateOf(TextFieldValue("192.168.1.112")) }
    var port by remember { mutableStateOf(TextFieldValue("25565")) }
    var radarData by remember { mutableStateOf("Esperando datos...") }
    var connectionError by remember { mutableStateOf("") }
    var position by remember { mutableStateOf(0) }
    var distance by remember { mutableStateOf(0) }
    val context = LocalContext.current

    when (currentScreen) {
        ScreenState.Connection -> MainScreen(
            ipAddress = ipAddress,
            port = port,
            radarData = radarData,
            connectionError = connectionError,
            onIpAddressChange = { ipAddress = it },
            onPortChange = { port = it },
            onConnect = { ip, port ->
                Logistic.connectToServer(context, ip, port, { data, error ->
                    radarData = data
                    connectionError = error

                    val parts = data.split(",")
                    if (parts.size == 2) {
                        position = parts[0].toIntOrNull() ?: position
                        distance = parts[1].toIntOrNull() ?: 0
                    }
                }, onConnected = {
                    currentScreen = ScreenState.Content
                }, onDisconnected = {
                    currentScreen = ScreenState.Connection
                })
            }
        )
        ScreenState.Content -> RadarScreen(
            position = position,
            distance = distance,
            onDisconnect = {
                Logistic.disconnect()
                currentScreen = ScreenState.Connection
            },
            radarData = radarData
        )
    }
}
