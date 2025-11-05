/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#include "SensorTemperatura.h"

#include <Wire.h>
#include <Adafruit_MLX90614.h>


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void sensorTemperatura_init()
{
  debugln("[MLX90614] Inicializando sensor de temperatura...");

  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(100000);

  if (!mlx.begin())
  {
    debugln("Error de conexión al sensor MLX");
    while(1);
  }
  mlx.writeEmissivity(MLX90614_EMISSIVITY);

  debugln("✅ MLX90614 listo.");

}
float sensorTemperatura_leerObjeto()
{
  float temp = mlx.readObjectTempC();
  if (isnan(temp))
  {
    debugln("⚠️ Error leyendo temperatura de objeto.");
    return -1.0f;
  }
  return temp;
}
float sensorTemperatura_leerAmbiente()
{
  float temp = mlx.readAmbientTempC();
  if (isnan(temp))
  {
    debugln("⚠️ Error leyendo temperatura de ambiente.");
    return -1.0f;
  }
  return temp;
}
float sensorTemperatura_leerTempCorporal()
{
  int abortar = 0; 
  float tObj = sensorTemperatura_leerObjeto();
  float tAmb = sensorTemperatura_leerAmbiente();
  while (tObj == -1.0f || tAmb == -1.0f)
  {
    tObj = sensorTemperatura_leerObjeto();
    tAmb = sensorTemperatura_leerAmbiente();

    abortar++;
    if (abortar == 10)
    {
      debugln("⚠️ Error leyendo temperatura Corporal. Valores no válidos.");
      return -1.0f;
    }
  }
  if (tObj < 0 || tAmb <0)
  {
    debugln("⚠️ Error leyendo temperatura corporal. Fuera de rango.");
    debug(" tObj = ");debug(tObj);debug(" | ");debug("tAmb = ");debugln(tAmb);
    return -1.0f;
  }
  float tCorporal = tObj + (tAmb - 25.0f) * MLX90614_COEF_SENSIBILIDAD + MLX90614_OFFSET;

  return tCorporal;

}