package com.kozmoi.uar.views

import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.*
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.kozmoi.uar.R

@Preview
@Composable
fun MainScreen(
    ipAddress: TextFieldValue = TextFieldValue("192.168.1.112"),
    port: TextFieldValue = TextFieldValue("25565"),
    radarData: String = "Esperando establecer conexión...",
    connectionError: String = "",
    onIpAddressChange: (TextFieldValue) -> Unit = {},
    onPortChange: (TextFieldValue) -> Unit = {},
    onConnect: (String, String) -> Unit = { _, _ -> }
) {
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Top
    ) {
        Spacer(modifier = Modifier.weight(.7f))
        Image(painter = painterResource(id = R.drawable.logo), contentDescription = "Logo")

        Spacer(modifier = Modifier.height(48.dp))

        TextField(
            maxLines = 1,
            value = ipAddress,
            onValueChange = onIpAddressChange,
            label = { Text("Dirección IP del Servidor UAR-S") }
        )
        Spacer(modifier = Modifier.height(8.dp))
        TextField(
            maxLines = 1,
            value = port,
            onValueChange = onPortChange,
            label = { Text("Puerto del Servidor UAR-S") }
        )

        Spacer(modifier = Modifier.height(16.dp))

        Button(onClick = { onConnect(ipAddress.text, port.text) }) {
            Text("Conectar con el Servidor UAR-S")
        }

        Spacer(modifier = Modifier.height(16.dp))

        Column(modifier = Modifier.verticalScroll(rememberScrollState())) {
            Image(modifier = Modifier.fillMaxWidth(), painter = painterResource(id = R.drawable.temp), contentDescription = "Logo")
            Spacer(modifier = Modifier.height(16.dp))
            Text(
                text = "Estudiantes de la Carrera de Ingeniería en Sistemas Computacionales | 26 de Octubre del 2024 - UCENM",
                style = MaterialTheme.typography.titleSmall,
                textAlign = TextAlign.Justify
            )

            Spacer(modifier = Modifier.height(16.dp))

            Text(
                text = "Este grupo está conformado por estudiantes comprometidos y apasionados por la tecnología. Trabajamos juntos para lograr nuestros objetivos académicos y contribuir al desarrollo de soluciones innovadoras.",
                textAlign = TextAlign.Justify
            )

            Spacer(modifier = Modifier.height(16.dp))

            Text(modifier = Modifier.fillMaxWidth(), text = "Integrantes del Equipo de Desarrollo:", textAlign = TextAlign.Start)

            val members = listOf(
                "1. Carlos Espinoza" to "- 122590039",
                "2. Kassandra Casselman" to "- 223590007",
                "3. Ismael Martinez" to "- 124590048",
                "4. Hazael Funez" to "- 124590021",
                "5. Jorge Sanchez" to "- 124590051"
            )

            // Mostrar la lista de miembros con nombre alineado a la izquierda y número a la derecha
            members.forEach { (name, number) ->
                Row(
                    modifier = Modifier.fillMaxWidth().padding(vertical = 4.dp),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Text(
                        text = name,
                        modifier = Modifier.weight(1f),
                        textAlign = TextAlign.Start
                    )
                    Text(
                        text = number,
                        modifier = Modifier.weight(1f),
                        textAlign = TextAlign.Start
                    )
                }
            }
        }
        if (connectionError.isNotEmpty())
            ShowError(connectionError);
        Spacer(modifier = Modifier.weight(1f))
    }
}

@Composable
fun ShowError(connectionError: String) {
    val snackbarHostState = remember { SnackbarHostState() }

    LaunchedEffect(connectionError) {
        if (connectionError.isNotEmpty()) {
            snackbarHostState.showSnackbar(
                connectionError,
                actionLabel = "Ok",
                duration = SnackbarDuration.Long
            )
        }
    }
}
