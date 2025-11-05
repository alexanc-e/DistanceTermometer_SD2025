/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#ifndef _BLESERVER_H_
#define _BLESERVER_H_

#include <Arduino.h>

//Muestra a través de las clase definida en el .cpp cuando un cliente se ha conecta correctamente
bool BLE_conectado(void);
//Se inicializa el servidor BLE junto a sus característiscas
void BLE_inicializar(void);
//El servidor BLE es visible  
void BLE_iniciarPublicidad(void);
//El servidor BLE deja de estar publicitado y por lo tanto el cliente no podrá encontrarlo
void BLE_detenerPublicidad(void);
//Fuerza la desconexión del cliente en el caso de que haya un cliente conectado
void BLE_desconectarCliente(void);
//Variables:
//   mensaje: cadena de texto tipo char de 20 elementos que almacena la información enviada al cliente
// Esta función se utiliza para enviar información al cliente
void BLE_enviar(const char *mensaje);

#endif