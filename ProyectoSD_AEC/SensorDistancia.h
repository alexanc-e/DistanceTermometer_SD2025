/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#ifndef _SENSORDISTANCIA_H_
#define _SENSORDISTANCIA_H_

#include <Arduino.h>
#include <VL53L0X.h>
#include "Configuracion.h"

//Inicializa el sensor de Distancia VL53L0X
void sensorDistancia_init();
//Variables:
//   muestras: variable entera que determina la cantidad de muestras que se recogen para hacer el valor medio
// Se leen un número de muestras especificado y se devuelve el valor medio de las muestras
float sensorDistancia_leer();
// Entrada: Valor de Distancia
//Comprueba si ese valor de distancia de entrada se encuentra dentro del rango de medición óptima
bool sensorDistancia_enRango(float distancia);

#endif