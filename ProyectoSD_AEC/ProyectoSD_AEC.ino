/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#include <Arduino.h>
#include "Configuracion.h"
#include "RGB.h"
#include "SensorDistancia.h"
#include "sensorTemperatura.h"
#include "BLEserver.h"

// ----------------------------
// Definición de estados
// ----------------------------

enum Estado {
  IDLE, // Estado de espera a la pulsación de encendido
  BLE_ADVERTISING, // Estado de espera a cliente
  BLE_CONNECTED, // Estado de conexión con cliente, da pie a la medición de distancia
  DISTANCE_READING,// Estado de busqueda de la distancia óptima
  TEMP_RESULT,// Estado de medición de temperatura y envio de resultado
  WAIT_NEXT_ACTION,//Estado de espera para nueva medición o apagado automático
  SLEEP_MODE// Estado de reposo
};
Estado estadoActual = IDLE;

// ----------------------------
// Variables globales de control
// ----------------------------
unsigned long lastActionTime = 0;   // Para detectar inactividad (>30s)
unsigned long distanceTimeout = 0;  // Para detectar sin distancia (>20s)
unsigned long measureStartTime = 0; // Para medir los 5s de temperatura
unsigned long botonStartTime = 0; // Para medir cuanto tiempo el boton permanece pulsado

bool botonPresionado = false;

VL53L0X sensorDist;

void setup() 
{
  Serial.begin(115200);
  BLE_inicializar();    // Configura servidor BLE (NimBLE)
  RGB_init();           // Configura LED RGB
  sensorDistancia_init();
  sensorTemperatura_init();
  pinMode(BOTON_PIN, INPUT_PULLUP);
  // BLE_enviar("STATE:READY");
  RGB_off();
  Serial.println("Inicialización terminada correctamente");
}

void loop() {
  manejarBoton();
  switch (estadoActual) {

    // -------------------------
    case IDLE:
      if (digitalRead(BOTON_PIN) == HIGH) {
        estadoActual = BLE_ADVERTISING;
        BLE_iniciarPublicidad();
        RGB_setColor(0, 0, 255); // Azul parpadeante
      }
      break;

    // -------------------------
    case BLE_ADVERTISING:
      if (BLE_conectado()) {
        estadoActual = BLE_CONNECTED;
        BLE_enviar("STATE:Dispositivo Conectado");
        RGB_setColor(0, 0, 255); // Azul fijo
        delay(1000);
      }
      else{
        RGB_blink(0, 0, 255, 2); // Azul parpadeante
      }
      break;

    // -------------------------
    case BLE_CONNECTED:
      delay(1000);
      estadoActual = DISTANCE_READING;
      distanceTimeout = millis();
      break;

    // -------------------------
    case DISTANCE_READING: {
      BLE_enviar("STATE:Midiendo Distancia . . .");
      float distancia = sensorDistancia_leer();
      char msg[20];
      sprintf(msg, "DIST:%.1f", distancia);
      BLE_enviar(msg);
      RGB_blink(0,255,0,2);//Verde intermitente mientras busca medida correcta
      if (sensorDistancia_enRango(distancia)) {
        BLE_enviar("STATE:Distancia Correcta");
        RGB_setColor(0, 255, 0); // Verde
        estadoActual = TEMP_RESULT;
        delay(500);
        measureStartTime = millis();
        BLE_enviar("STATE:Midiendo Temperatura . . .");
      } else if (millis() - distanceTimeout > TIEMPO_SIN_DISTANCIA) {
        estadoActual = SLEEP_MODE; // Sin distancia por 2 min
      }
      break;
    }

    // -------------------------
    case TEMP_RESULT: 
    {
      RGB_off();
      float temp = sensorTemperatura_leerTempCorporal();
      char msg[50];
      if (temp == -1.0f)
      {
        BLE_enviar("MSG:Error al medir la temperatura.Reiniciando medición.");
        delay(10);
        RGB_blink(255,0,0,3);//Rojo intermitente --> Error
        BLE_enviar("STATE:Volviendo a medir . . .");
        delay(1000);
        break;
      }
    
      if (temp < TEMP_VERDE_MAX) 
      { 
        sprintf(msg,"RES:VER%.2f",temp); //Temperatura Normal
        BLE_enviar(msg); 
        RGB_setColor(0, 255, 0); // Verde
        delay(1000);
      }         
      else if (temp < TEMP_AMARILLO_MAX) 
      {
        sprintf(msg,"RES:AMA%.2f",temp); //Temperatura Elevada
        BLE_enviar(msg);
        RGB_setColor(255, 255, 0); // Amarillo
        delay(1000);
      }   
      else if (temp < TEMP_NARANJA_MAX) 
      {
        sprintf(msg,"RES:NAR%.2f",temp); //Fiebre Moderada
        BLE_enviar(msg);
        RGB_setColor(255, 128, 0); // Naranja
        delay(1000);
      }   
      else 
      {
        sprintf(msg, "RES:RED%.2f",temp); //Fiebre Elevada
        BLE_enviar(msg); 
        RGB_setColor(255, 0, 0); // Rojo
        delay(1000);
      } 

      BLE_enviar("STATE:Pulse para nueva medición . . .");
      estadoActual = WAIT_NEXT_ACTION;
      delay(500);
      lastActionTime = millis();
      break;
    }

    // -------------------------
    case WAIT_NEXT_ACTION:
      RGB_blink(255,255,255,2); // intermitente blanco
      if (millis() - lastActionTime > TIEMPO_INACTIVIDAD) {
        estadoActual = SLEEP_MODE; // Inactividad 
      }
      else if (digitalRead(BOTON_PIN) == HIGH)
      {
        estadoActual = DISTANCE_READING;
        BLE_enviar("NEW:Nueva Medición");
      }
      break;
    // -------------------------
    case SLEEP_MODE:
      BLE_enviar("STATE:SLEEP");
      BLE_detenerPublicidad();
      BLE_desconectarCliente();
      RGB_off();
      delay(500);
      estadoActual = IDLE;
      break;
      //Añadir una función para echar al cliente
  }
}
void manejarBoton(void) {
  int lectura = digitalRead(BOTON_PIN);

  if (lectura == HIGH && !botonPresionado) {
    botonPresionado = true;
    botonStartTime = millis();
  }

  if (lectura == LOW && botonPresionado) 
  {
    botonPresionado = false;
    unsigned long duracion = millis() - botonStartTime;

    if (duracion < TIEMPO_BOTON_LARGO) 
    {
      // Pulsación corta → Nueva medición
      if (estadoActual == WAIT_NEXT_ACTION)
      {
        estadoActual = DISTANCE_READING;
        BLE_enviar("NEW:Nueva Medición");
      }
    }
    else 
    {
      // Pulsación larga → Reposo
      estadoActual = SLEEP_MODE;
      BLE_enviar("STATE:SLEEP");
      RGB_off();
      delay(3000);
    }
  }
}