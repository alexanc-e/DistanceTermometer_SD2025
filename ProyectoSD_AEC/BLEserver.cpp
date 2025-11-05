/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#include <Arduino.h>
#include "BLEserver.h"
#include "Configuracion.h"
#include <NimBLEDevice.h>
// -------------------------------
// Objetos BLE globales
// -------------------------------
NimBLEServer* pServer = nullptr;
NimBLECharacteristic* pCharacteristic = nullptr;
bool dispositivoConectado = false;
// -------------------------------
// Callbacks personalizados
// -------------------------------
//Definimos el siguiente callback para saber si hay un cliente conectado o no, para ello nos aprovechamos cuando se
//sobreescribe (override) el estado de conexión con las funciones onConnect y onDisconnect
class MyServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer* pServer) override {
    dispositivoConectado = true;
    debugln("📶 Cliente BLE conectado");
  }

  void onDisconnect(NimBLEServer* pServer) override {
    dispositivoConectado = false;
    debugln("❌ Cliente BLE desconectado");
    NimBLEDevice::startAdvertising();
  }
};

bool BLE_conectado()
{
  return dispositivoConectado;
}
void BLE_inicializar()
{
  NimBLEDevice::init(BLE_DEVICE_NAME);
  NimBLEDevice::setPower(ESP_PWR_LVL_P7); // Potencia de transmisión

  pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Crear servicio
  NimBLEService* pService = pServer->createService(BLE_SERVICE_UUID);

  // Crear característica con Notify
  pCharacteristic = pService->createCharacteristic(
    BLE_CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::NOTIFY | NIMBLE_PROPERTY::READ
  );

  // Valor inicial
  pCharacteristic->setValue("STATE:READY");

  // Iniciar servicio
  pService->start();

  // Iniciar publicidad
  BLE_iniciarPublicidad();

  debugln("✅ BLE inicializado y publicando");
}
void BLE_iniciarPublicidad()
{
  NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();
  debugln("📢 Publicidad BLE iniciada");
}
void BLE_detenerPublicidad()
{
  NimBLEDevice::getAdvertising()->stop();
  debugln("🛑 Publicidad BLE detenida");
}
void BLE_desconectarCliente()
{
  if (dispositivoConectado) {
    debugln("🔌 Desconectando cliente BLE...");
    pServer->disconnect(0); // Desconecta el cliente activo (único)
    delay(300);
  } else {
    debugln("No hay cliente conectado actualmente");
  }
}
void BLE_enviar(const char *mensaje) {
  if (!pServer) {
    debugln("❌ pServer no inicializado");
    return;
  }
  if (!pCharacteristic) {
    debugln("❌ pCharacteristic no inicializado");
    return;
  }
  if (!dispositivoConectado) {
    debugln("⚠️ Cliente no conectado");
    return;
  }
  size_t len = strlen(mensaje);
  pCharacteristic->setValue((uint8_t*)mensaje,len);
  pCharacteristic->notify();
  debug("📤 Enviado → "); debugln(mensaje);
  delay(500);
}
