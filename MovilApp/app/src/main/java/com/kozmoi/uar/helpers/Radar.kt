package com.kozmoi.uar.helpers

import androidx.compose.foundation.*
import androidx.compose.foundation.layout.*
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.*
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.unit.*
import androidx.compose.ui.*
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.geometry.Size
import androidx.compose.ui.graphics.*
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import com.kozmoi.uar.R
import java.lang.Math.toRadians
import kotlin.math.*

data class RadarLine(val angle: Int, val distance: Float, var alpha: Float)

private val lineHistory = mutableListOf<RadarLine>()

@Preview
@Composable
fun Radar(position: Int = 0, distance: Int = 0, onDisconnect: () -> Unit = {}) {
    Box(modifier = Modifier.fillMaxSize()) {
        Canvas(
            modifier = Modifier
                .fillMaxSize()
                .align(Alignment.Center)
        ) {
            drawRadar(position, distance, size)
        }
        Image(
            painter = painterResource(id = R.drawable.disconnect),
            contentDescription = "Disconnect",
            modifier = Modifier.size(40.dp).rotate(90f).align(Alignment.TopEnd)
                .padding(4.dp)
                .clickable { onDisconnect(); }
        )
    }
}

private fun DrawScope.drawRadar(iAngle: Int, iDistance: Int, size: Size) {
    val centerX = size.width / 55f
    val centerY = size.height / 2f // Ajuste para que no se dibuje demasiado abajo
    val maxDistance = 40f // Distancia máxima para el radar
    val radarSize = size.width * 1.7f
    val rotationAngle = 90f // Grados de rotación en sentido horario

    // Aplicar rotación
    drawContext.canvas.save() // Guardar el estado actual del canvas
    drawContext.canvas.rotate(rotationAngle, centerX, centerY) // Rotar el canvas

    // Dibuja los arcos del radar
    drawArc(
        color = Color.Black,
        startAngle = 180f,
        sweepAngle = 180f,
        useCenter = false,
        size = Size(radarSize, radarSize),
        topLeft = Offset(centerX - radarSize / 2, centerY - radarSize / 2)
    )

    // Dibuja las líneas del radar
    for (angle in 0..180 step 30) {
        drawLine(
            Color.White,
            Offset(centerX, centerY),
            Offset(
                centerX + radarSize / 2 * cos(toRadians(angle.toDouble())).toFloat(),
                centerY - radarSize / 2 * sin(toRadians(angle.toDouble())).toFloat()
            ),
            strokeWidth = 2f
        )
    }

    // Limitar el historial para evitar exceso de líneas (por ejemplo, 50)
    if (lineHistory.size > 90) {
        lineHistory.removeAt(0) // Eliminar las líneas más antiguas
    }

    // Dibujar todas las líneas del historial
    for (line in lineHistory) {
        val redStartX = centerX + line.distance * cos(toRadians(line.angle.toDouble())).toFloat()
        val redStartY = centerY - line.distance * sin(toRadians(line.angle.toDouble())).toFloat()

        val redEndX = centerX + (radarSize / 2) * cos(toRadians(line.angle.toDouble())).toFloat()
        val redEndY = centerY - (radarSize / 2) * sin(toRadians(line.angle.toDouble())).toFloat()

        val greenEndX = centerX + min(line.distance, radarSize / 2) * cos(toRadians(line.angle.toDouble())).toFloat()
        val greenEndY = centerY - min(line.distance, radarSize / 2) * sin(toRadians(line.angle.toDouble())).toFloat()

        // Dibujar la línea verde que varía según la distancia
        drawLine(
            Color.Green.copy(alpha = line.alpha),
            Offset(centerX, centerY),
            Offset(greenEndX, greenEndY),
            strokeWidth = 5f
        )

        // Dibujar la línea roja solo si la distancia supera el punto inicial
        if (line.distance < radarSize / 2) {
            drawLine(
                Color.Red.copy(alpha = line.alpha),
                Offset(redStartX, redStartY),
                Offset(redEndX, redEndY),
                strokeWidth = 5f
            )
        }

        // Reducir la opacidad para desvanecimiento
        line.alpha -= 0.01f
    }

    // Eliminar las líneas completamente desvanecidas (alpha <= 0)
    lineHistory.removeAll { it.alpha <= 0f }

    // Si la distancia está dentro del rango, añadimos la línea al historial
    val pixsDistance = (iDistance / maxDistance) * (radarSize / 2)
    lineHistory.add(RadarLine(iAngle, pixsDistance, 1f))

    // Dibuja las etiquetas de distancia
    for (d in 1..4) {
        drawContext.canvas.nativeCanvas.drawText(
            "${d * 10} cm",
            centerX + (d * (radarSize / 8)) * cos(toRadians(180.0)).toFloat(),
            centerY - (d * (radarSize / 8)) * sin(toRadians(180.0)).toFloat()- 5,
            android.graphics.Paint().apply { color = android.graphics.Color.CYAN; textSize = 30f }
        )
    }

    // Dibuja el ángulo y la distancia actuales
    drawContext.canvas.nativeCanvas.drawText(
        "Ángulo: $iAngle°",
        centerX + (0.8f * (radarSize / 8)) * cos(toRadians(0.180)).toFloat(),
        centerY - (0.8f * (radarSize / 8)) * sin(toRadians(0.180)).toFloat()-5,
        android.graphics.Paint().apply { color = android.graphics.Color.CYAN; textSize = 30f }
    )

    drawContext.canvas.nativeCanvas.drawText(
        "Distancia: $iDistance cm",
        centerX + (2 * (radarSize / 8)) * cos(toRadians(0.180)).toFloat(),
        centerY - (2 * (radarSize / 8)) * sin(toRadians(0.180)).toFloat(),
        android.graphics.Paint().apply { color = android.graphics.Color.CYAN; textSize = 30f }
    )

    // Dibuja las etiquetas de los ángulos
    val textPaint = android.graphics.Paint().apply {
        color = android.graphics.Color.YELLOW
        textSize = 35f
        textAlign = android.graphics.Paint.Align.CENTER
    }

    val angleLabels = listOf(0, 30, 60, 90, 120, 150, 180)
    for (angle in angleLabels) {
        val labelX = centerX + (radarSize / 1.92f) * cos(toRadians(angle.toDouble())).toFloat()
        val labelY = centerY - (radarSize / 2) * sin(toRadians(angle.toDouble())).toFloat() - 20

        drawContext.canvas.nativeCanvas.drawText(
            "$angle°",
            labelX,
            labelY,
            textPaint
        )
    }

    drawContext.canvas.restore() // Restaurar el estado del canvas
}
