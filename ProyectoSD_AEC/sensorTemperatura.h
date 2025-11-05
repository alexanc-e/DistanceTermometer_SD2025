/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#ifndef _SENSORTEMPERATURA_H_
#define _SENSORTEMPERATURA_H_

#include <Arduino.h>
#include "Configuracion.h"

//Inicializa el sensor de Temperatura MLX90614
void sensorTemperatura_init();
//Obtiene una lectura de temperatura del OBJETO (temperatura obtenida por IR)
float sensorTemperatura_leerObjeto();
//Obtiene una lectura de temperatura AMBIENTE (temperatura a la que se encuentra el sensor)
float sensorTemperatura_leerAmbiente();
//A traves de la temperatura de OJETO y AMBIENTE, calcula una aproximación medianamente precisa de la temperatura CORPORAL
float sensorTemperatura_leerTempCorporal();

#endif