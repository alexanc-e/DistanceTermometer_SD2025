/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#ifndef _RGB_H_
#define _RGB_H_

#include <Arduino.h>
#include "Configuracion.h"

//Estas funciones están en lógica negativa debido a que se usa un LED RGB de Ánodo Común.
//Nomenclatura:
// AC: Ánodo Común

// Se inicializa los pines del led RGB
void RGB_init(void);
/*
Variables de entrada:
 Red:  Variable entera entre el 0 y el 255 que muestra la cantidad de rojo en el LED
 Green:  Variable entera entre el 0 y el 255 que muestra la cantidad de verde en el LED
 Blue:  Variable entera entre el 0 y el 255 que muestra la cantidad de azul en el LED
 brillo: Variable entera entre 0 y 100 que define la cantidad de brillo que queremos añadir o quitar
Con esta función se define el color deseado de salida en nomenclatura RGB y en logica negativa
*/
void RGB_setColor(int Red, int Green, int Blue, float brillo = 100);
/*
Variables de entrada:
 Red:  Variable entera entre el 0 y el 255 que muestra la cantidad de rojo en el LED
 Green:  Variable entera entre el 0 y el 255 que muestra la cantidad de verde en el LED
 Blue:  Variable entera entre el 0 y el 255 que muestra la cantidad de azul en el LED
 freq: variable entera que indica la frecuencia de parpadeo
 brillo: Variable entera entre 0 y 100 que define la cantidad de brillo que queremos añadir o quitar
Con esta función se define el color deseado de salida en nomenclatura RGB y en logica negativa
*/
void RGB_blink(int Red, int Green, int Blue, int freq, int brillo = 100);
// Se apaga el led
void RGB_off(void);

#endif