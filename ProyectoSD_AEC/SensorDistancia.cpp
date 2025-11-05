/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#include <Arduino.h>
#include <Wire.h>
#include "SensorDistancia.h"

// === Objeto global del sensor ===
VL53L0X sensorVL;

void sensorDistancia_init() {
  debugln("[VL53L0X] Inicializando sensor de distancia...");

  // Encender sensor
  pinMode(VL53L0X_XSHUT, OUTPUT);
  digitalWrite(VL53L0X_XSHUT, HIGH);
  delay(100);

  // Iniciar bus I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  // Reiniciar sensor
  digitalWrite(VL53L0X_XSHUT, LOW);
  delay(20);
  digitalWrite(VL53L0X_XSHUT, HIGH);
  delay(100);

  // Inicializar
  if (!sensorVL.init()) {
    debugln("❌ [VL53L0X] Error al inicializar sensor.");
    while (1);
  }

  // Configuración adicional
  sensorVL.setTimeout(VL53L0X_TIMEOUT_MS);
  sensorVL.setMeasurementTimingBudget(VL53L0X_TIMING_BUDGET_US);

  debugln("✅ [VL53L0X] Sensor inicializado correctamente.");
}

// === Leer varias muestras y devolver promedio en cm ===
float sensorDistancia_leer() {
  float suma = 0;
  int validas = 0;

  for (int i = 0; i < NUM_MUESTRAS_DIST; i++) {
    uint16_t distancia = sensorVL.readRangeSingleMillimeters();

    if (!sensorVL.timeoutOccurred() && distancia != 65535) {
      suma += distancia / 10.0f;  // mm → cm
      validas++;
    } else {
      debugln("⚠️ [VL53L0X] Timeout o lectura inválida.");
    }

    delay(100);
  }

  if (validas == 0) return -1.0f;  // error
  return suma / validas;
}

// === Comprobar si la distancia está dentro del rango óptimo ===
bool sensorDistancia_enRango(float distancia) {
  if (distancia < 0) return false;
  float minDist = VL53L0X_DISTANCIA_OPTIMA - VL53L0X_TOLERANCIA_CM;
  float maxDist = VL53L0X_DISTANCIA_OPTIMA + VL53L0X_TOLERANCIA_CM;
  return (distancia >= minDist && distancia <= maxDist);
}
