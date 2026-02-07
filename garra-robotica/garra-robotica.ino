#include <Servo.h>

// Definição das portas
const int PIN_SERVO = 12;
const int SW = 4;
const int VRX = A0;
const int VRY = A2;

// Limites de angulo e fechamento da garra
const int CLOSE = 170;
const int OPEN = 90;

// Variáveis de leitura
int leitura_SW;
int leitura_VRX;
int leitura_VRY;

// Definição da posição padrão do eixo Y
const int POSICAO_DEFAULT = 512;
const int DEADZONE = 40;
int posicao_referencia = POSICAO_DEFAULT;
int direcao = 0;

Servo servo_motor;

void setup() {
  servo_motor.attach(PIN_SERVO);
  pinMode(SW, INPUT_PULLUP);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
}

void movimentarGarra(int angulo) {
  angulo = map(angulo, 0, 1023, CLOSE, OPEN);
  servo_motor.write(angulo);
}

void loop() {
  leitura_VRY = analogRead(VRY);
  leitura_VRX = analogRead(VRX);
  leitura_SW = digitalRead(SW);

  // Alteração do direcao quando o eixo Y é manipulado
  if ((leitura_VRY > POSICAO_DEFAULT + DEADZONE) && direcao == 0)
    direcao = 1;
  else if ((leitura_VRY < POSICAO_DEFAULT - DEADZONE) && direcao == 0)
    direcao = -1;

  // Condição de movimento da garra
  if (leitura_SW == HIGH) {
    if (direcao == 1) {
      if (leitura_VRY >= posicao_referencia) {
        posicao_referencia = leitura_VRY;
      }

    } else if (direcao == -1) {
      if (leitura_VRY <= posicao_referencia) {
        posicao_referencia = leitura_VRY;
      }

    } else {
      posicao_referencia = leitura_VRX;
    }

  } else {
    // Se o botão é pressionado, a garra volta à posição padrão
    direcao = 0;
    posicao_referencia = POSICAO_DEFAULT;
  }

  movimentarGarra(posicao_referencia);
  delay(10);

}
