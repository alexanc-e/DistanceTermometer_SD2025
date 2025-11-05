/*
------------------------------------------------------------------------------------
  Creado en 2025
  por Alejandro Escalante Clemente
  como estudiante de Grado de Ingenieria de Sonido e Imagen en Telecomumicaciones
------------------------------------------------------------------------------------ 
*/
#include "RGB.h"

void RGB_init()
{
  ledcSetup(LED_R_CHANNEL, FREQ_PWM, RESOLUTION);
  ledcAttachPin(LED_R_PIN,LED_R_CHANNEL);
  ledcSetup(LED_G_CHANNEL, FREQ_PWM, RESOLUTION);
  ledcAttachPin(LED_G_PIN,LED_G_CHANNEL);
  ledcSetup(LED_B_CHANNEL, FREQ_PWM, RESOLUTION);
  ledcAttachPin(LED_B_PIN,LED_B_CHANNEL);
}

void RGB_setColor (int Red, int Green, int Blue, float brillo)
{
  ledcWrite(LED_R_CHANNEL,255-(Red*brillo/100));
  ledcWrite(LED_G_CHANNEL,255-(Green*brillo/100));
  ledcWrite(LED_B_CHANNEL,255-(Blue*brillo/100));

}
void RGB_blink (int Red, int Green, int Blue, int freq, int brillo)
{
  ledcWrite(LED_R_CHANNEL,255-(Red*brillo/100));
  ledcWrite(LED_G_CHANNEL,255-(Green*brillo/100));
  ledcWrite(LED_B_CHANNEL,255-(Blue*brillo/100));

  delay(1000/freq);

  RGB_off();

  delay(1000/freq);
}
void RGB_off(void) 
{
  ledcWrite(LED_R_CHANNEL,255);
  ledcWrite(LED_G_CHANNEL,255);
  ledcWrite(LED_B_CHANNEL,255);

}

