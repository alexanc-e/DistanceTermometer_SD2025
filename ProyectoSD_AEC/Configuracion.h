/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <Arduino.h>
#include <VL53L0X.h>
// ===========================
// ⚙️ CONFIGURACIÓN GENERAL
// ===========================

// Modo debug (activa prints por Serial)
#define DEBUG true

// ===========================
// 🔘 PINES
// ===========================
#define BOTON_PIN 2       // Botón conectado a GND (usa INPUT_PULLUP)

#define LED_R_PIN 12
#define LED_R_CHANNEL 0
#define LED_G_PIN 13
#define LED_G_CHANNEL 1
#define LED_B_PIN 19
#define LED_B_CHANNEL 2
// VL53L0X (sensor de distancia)
#define VL53L0X_XSHUT 23   // Pin de habilitación del VL53L0X
// I2C: usar SDA y SCL por defecto (GPIO21 y GPIO22 en ESP32)
#define I2C_SDA           21
#define I2C_SCL           22

// ===========================
// 🚦 Variables RGB
// ===========================
#define FREQ_PWM 5000
#define RESOLUTION 8
// ===========================
// 📏 UMBRALES DE DISTANCIA
// ===========================
#define VL53L0X_TIMEOUT_MS   500
#define VL53L0X_TIMING_BUDGET_US 200000
#define VL53L0X_DISTANCIA_OPTIMA 10.0f   // cm (10 cm)
#define VL53L0X_TOLERANCIA_CM     0.5f
#define NUM_MUESTRAS_DIST 5

// ===========================
// 🌡️ UMBRALES DE TEMPERATURA (°C)
// ===========================
#define MLX90614_EMISSIVITY 0.98f
#define MLX90614_COEF_SENSIBILIDAD 0.2f
#define MLX90614_OFFSET 0.5f
#define TEMP_VERDE_MAX 36.0f
#define TEMP_AMARILLO_MAX 37.0f
#define TEMP_NARANJA_MAX 38.0f

// ===========================
// ⏱️ TIEMPOS DE CONTROL (ms)
// ===========================
#define TIEMPO_BOTON_LARGO   3000   // >3s = reposo
#define TIEMPO_SIN_DISTANCIA 120000  // 2 min sin lectura válida
#define TIEMPO_INACTIVIDAD   60000  // 60s sin acción → reposo
#define TIEMPO_MEDICION      5000   // 5s de lectura de temperatura

// ===========================
// 🔵 BLE CONFIG
// ===========================

// UUIDs personalizados (puedes modificarlos si lo deseas)
#define BLE_SERVICE_UUID        "12345678-1234-1234-1234-1234567890AB"
#define BLE_CHARACTERISTIC_UUID "ABCD1234-5678-90AB-CDEF-1234567890AB"

// Nombre del dispositivo BLE que verá el móvil
#define BLE_DEVICE_NAME "TermometroESP32"

// ===========================
// 💡 VARIABLES PROGRAMA
// ===========================

extern float distancia;
extern VL53L0X sensor;

// ===========================
// 💡 FUNCIONES AUXILIARES
// ===========================

#if DEBUG
  #define debugln(x) Serial.println(x)
  #define debug(x)   Serial.print(x)
#else
  #define debugln(x)
  #define debug(x)
#endif



#endif