# Servidor de Radar Ultrasónico UAR-S®

## Descripción
Este proyecto utiliza un sensor de radar ultrasónico para detectar objetos en su camino y enviar datos a una aplicación móvil. El sistema está compuesto por un microcontrolador Arduino, dos servidores TCP y una aplicación de escritorio llamada Visual Radar, así como una aplicación móvil desarrollada en Kotlin.

## Componentes del Proyecto
- **Código Arduino**: Contiene el código para el radar ultrasónico.
- **Servidores TCP**: 
  - **Version1**: Implementación básica del servidor TCP, desarrollado en C#.
  - **Version2**: Implementación mejorada del servidor TCP, desarrollado en C++.
- **Aplicación Móvil**: Desarrollada en Kotlin para visualizar los datos enviados desde el radar.
- **Visual Radar**: Aplicación gráfica de escritorio para representar los datos en tiempo real, desarrollado en C++.

## Requisitos
- **Arduino IDE**: Para cargar el código en el microcontrolador Arduino.
- **JDK**: Necesario para compilar y ejecutar la aplicación de Kotlin.
- **Bibliotecas de Raylib**: Utilizadas para la visualización en la aplicación de escritorio.

## Instalación
1. Clona este repositorio:
   ```bash
   git clone https://github.com/SrIruma/UAR-Proyecto.git
