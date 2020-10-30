#include "mbed.h"

InterrcimatIn button(p5);
DigitalOut red_led(p6);
DigitalOut blue_led(p7);
PwmOut intensity(p8);

Timeout timeout;
Ticker ticker;

enum STATE
{
  baixo,
  cima,
  cair,
  subir,
  maximo,
  minino
};

int current_state;
bool stateButton;
float frequency;

void transitionStatePress(){
  timeout.attach(&piscarLed, 0.1);
  if (stateButton){
    switch (current_state)    {
    case cima:
      current_state = subir;
      printf("subir");
      break;
    case baixo:
      current_state = cair;
      printf("cair");
      break;
    case cair:
      if (intensity == 0.0){
        current_state = minino;
        printf("minino");
      }  else {
        intensity = intensity - 0.05;
        printf("intensity: %2f", intensity.read());
      }
      break;
    case subir:
      if (intensity == 1.0){
        current_state = maximo;
        printf("maximo");
      } else {
        intensity = intensity + 0.05;
        printf("Intensidade: %2f", intensity.read());
      }
      break;
    default:
      if (intensity >= 1.0){
        current_state = maximo;
        printf("maximo");
      } else if (intensity <= 0.0){
        current_state = minino;
        printf("minino");
      }
      break;
    }
  }
}

void inicio(){
  blue_led = 1.0;
  red_led = 0.0;
  intensity = 1.0;
  current_state = cima;
  printf("cima");
}

void pressioneBotao(){
  printf("Pressione o botao");
  stateButton = true;
  ticker.attach(&transitionStatePress, 1.0);
}

void liberarBotao(){
  printf("Liberar o botao");
  stateButton = false;
  ticker.detach();
  if (current_state == maximo || current_state == cima || current_state == cair){
    if (intensity > 0.0){
      blue_led = 0.0;
      red_led = 1.0;
      current_state = baixo;
      printf("baixo");
    }
  }
  else if (current_state == minino || current_state == baixo || current_state == subir){
    if (intensity < 1.0)
    {
      blue_led = 1.0;
      red_led = 0.0;
      current_state = cima;
      printf("cima");
    }
  }
}

void piscarLed(){
  switch (current_state)
  {
  case cair:
    frequency = 1;
    red_led = !red_led;
    break;
  case subir:
    frequency = 1;
    blue_led = !blue_led;
    break;
  case minino:
    frequency = 0.1;
    red_led = !red_led;
    break;
  case maximo:
    frequency = 0.1;
    blue_led = !blue_led;
    break;
  default:
    break;
  }
  timeout.attach(&piscarLed, frequency);
}

int main(){
  inicio();
  button.rise(&pressioneBotao);
  button.cair(&liberarBotao);
}
