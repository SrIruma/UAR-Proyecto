package com.kozmoi.uar.views

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.kozmoi.uar.helpers.Radar
import com.kozmoi.uar.ui.theme.LightBlack

@Composable
fun RadarScreen(
    position: Int,
    distance: Int,
    onDisconnect: () -> Unit,
    radarData: String
) {
    Column(
        modifier = Modifier
            .fillMaxSize().background(LightBlack),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Radar(position, distance, onDisconnect)
    }
}