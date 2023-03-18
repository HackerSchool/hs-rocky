#include <Arduino.h>
#include <HardwareSerial.h>
#include <Servo.h>

static void rodarPara(uint8_t motor_num, int angulo, int tempo_espera);
static void rodarGraus(uint8_t motor_num, int graus, int tempo_espera, bool sentido_horario);
static void indicarMotor(uint8_t numero);

Servo servo;
const int pino_sinal_motor = 9;
const unsigned num_motores = 17;

int angulo_atual = 0;

void setup(){
  ::Serial.begin(9600);
  ::servo.attach(::pino_sinal_motor);
}

void loop(){
  ::rodarPara(pino_sinal_motor, 0, 2000);
  ::delayMicroseconds(400);
  ::rodarPara(pino_sinal_motor, 180, 2000);
  ::delay(1000);
  ::rodarGraus(pino_sinal_motor, 56, 2000, false);
  ::delayMicroseconds(400);
  ::rodarGraus(pino_sinal_motor, 34, 2000, true);
  ::delay(2000);
}

static void rodarPara(uint8_t motor_num, int angulo, int tempo_espera) {
  const int soma_humana = angulo / 18;
  int tempo_espera_atual = tempo_espera;
  bool somar = ::angulo_atual < angulo;

  //::indicarMotor(motor_num);

  int i = 0;
  if (somar) {
    for (i = 0 ; i <= angulo; ++i) {
      ::servo.write(::angulo_atual);
      (::angulo_atual < (angulo/2)) ? (tempo_espera_atual -= soma_humana) : (tempo_espera_atual += soma_humana);
      ::delayMicroseconds(static_cast<unsigned>(tempo_espera_atual));
    }
  } else {
    for (i = ::angulo_atual ; i >= angulo; --i) {
      ::servo.write(::angulo_atual);
      (::angulo_atual < (angulo/2)) ? (tempo_espera_atual -= soma_humana) : (tempo_espera_atual += soma_humana);
      ::delayMicroseconds(static_cast<unsigned>(tempo_espera_atual));
    }
  }
  ::angulo_atual = i;
}

static void rodarGraus(uint8_t motor_num, int graus, int tempo_espera, bool sentido_horario) {
  const int soma_humana = graus / 18;
  int tempo_espera_atual = tempo_espera;

  //::indicarMotor(motor_num);

  if (sentido_horario) {
    for (int i = graus ; i > 0; --i) {
      ::servo.write(i);
      (i < (graus/2)) ? (tempo_espera_atual -= soma_humana) : (tempo_espera_atual += soma_humana);
      ::delayMicroseconds(static_cast<unsigned>(tempo_espera_atual));
    }
  } else {
    for (int i = 0 ; i <= graus; ++i) {
      ::servo.write(i);
      (i < (graus/2)) ? (tempo_espera_atual -= soma_humana) : (tempo_espera_atual += soma_humana);
      ::delayMicroseconds(static_cast<unsigned>(tempo_espera_atual));
    }
  }
}

static void indicarMotor(uint8_t numero) {
  for (uint8_t bit_num = 0; bit_num < ::num_motores; ++bit_num) {
    ::digitalWrite(bit_num, (numero >> bit_num) % 2);
  }
}
