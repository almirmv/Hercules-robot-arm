/*
  Nome do Projeto:  Projeto Hércules, braço robótico
  Nome do Aquivo:   hercules_robot_arm.ino                 
  Dependencias:     Adafruit_PWMServoDriver.h        Fonte: Instalar pela IDE Arduino
                    Adafruit_NeoPixel.h              Fonte: Instalar pela IDE Arduino
                    
  link:             
  Board:            ARDUINO MEGA 2560 / Programador AVRISP mkll
  Compilador        N/A
  IDE:              Arduino IDE 1.8.12
  Hardware:         PCA9685(16 servo controller), RGB WS2812(RGB endereçável "neopixel"),  
  Escrito por:      Almir M Ventura
  Modificado por:   N/A 
  Data:             04/10/2020
  Uso:              N/A
  Desenhos          N/A  Fonte:
  Copyright @       N/A
  
  Este programa é software livre;
  a distribuíção toda ou parcial é permitida pela licença MIT.
    
  REVISIONS: (latest entry first)
  04/01/2021   -  hercules_robot_arm.ino    alterações em comentáros no código
  04/10/2020   -  hercules_robot_arm.ino
   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Descrição:  
  
  Firmware para controle de braço robótico com 5 servos. Possui função de aprendizagem ou por modo manual (usando as mão para posicionar o braço) ou usando Knobs.
  Cada servo foi modificado sendo possível agora saber a posição do servo em tempo real.
  No modo manual a pessoa movimenta o braço com a mão e pressiona o botao para o robo gravar a posição;
  OBS: nese modo os servos ficam deasabilitados para evitar danifica-los.
  Possui interpolação de movimento quando está executando os movimentos. Cada junta tem seu angulo relativo calculado e o maior deles define o tempo do movimento.
  O restantes calculam proporcionalmente velocidades mais lentas para terminarem ao mesmo tempo o frame atual.   
    
  */

#include <Servo.h>                    //para controlar os servos
#include <Wire.h>                     //para comunicação I2C
#include <Adafruit_PWMServoDriver.h>  //para controlar 16 servos com a placa PCA9685
#include <Adafruit_NeoPixel.h>        //para os leds RGB WS2812 Instalar "Adafruit NeoPixel by Adafruit"


#define NUMPIXELS           8         //quantos leds existem na fita
#define NUMSERVOS           6         //quantos servos estao conectados ao PCA9685
#define INTERVALO_SERVOS    5         //quantos ms para recalcular e atulizar servos.

#define DEBOUNCE            50        //tempo ms de debounce do botao
#define SHORT_PRESS_TIME    300       //tempo ms para detecçao de clique curto
#define LONG_PRESS_TIME     900       //tempo ms para detecção de clique longo

//pinos que fazem parte dos botoes de Aprender e executar
#define LED_STRIP_PIN      53         //pino da tira de neopixel
#define PIN_LED_APRENDER   51
#define PIN_BTN_APRENDER   49

#define PIN_LED_EXECUTAR   47
#define PIN_BTN_EXECUTAR   45

//pinos dos knobs da interface com usuário (Analogicos)
#define PIN_KNOB_SERVO_0  5
#define PIN_KNOB_SERVO_1  4
#define PIN_KNOB_SERVO_2  3
#define PIN_KNOB_SERVO_3  2
#define PIN_KNOB_SERVO_4  1
#define PIN_KNOB_SERVO_5  0

//pinos que fazem leituras dos pots internos dos servos (analogicos) **************** checar isso no ofr ta usadno 0 a 4 ¬¬
#define PIN_POT_SERVO_0   6
#define PIN_POT_SERVO_1   7
#define PIN_POT_SERVO_2   8
#define PIN_POT_SERVO_3   9
#define PIN_POT_SERVO_4   10
#define PIN_POT_SERVO_5   11

//Declarando variáveis 
int a=0;
int efeito_atual_led=0;     //efeito atual do led
int etapa_efeito=0;         //etapa do efeito atual
int led_atual=0;            //indice do led atual
int delay_led=0;            //quantos ticks de 10ms se passaram dentro da rotina de LEDS
uint32_t rgbcolor;
int estado_movimento=0;

int servo_num = 0;                                          //servo escolhido
int servo_min_manual[]={444, 422, 100, 480, 130, 470};      //limites inferiores do servos
int servo_max_manual[]={100, 106, 424, 130, 400, 315};      //limites inferiores do servos
int servo_min[]={2600, 2600, 600, 2500, 750, 2800};            //limites inferiores do servos
int servo_max[]={600, 600, 2600, 700, 2600, 1900};       //limites máximos
float servo_value[]={1500, 1500, 1500, 1500, 1500, 1880};     //guarda o angulo atual dos servos. valores atuais em us.
int pots[6];                                                // salva leitura dos potenciometros internos do servos
int knobs[6];                                               // salva as leituras dos knobs 
float vel_servos[6];                                        //Velocidade de cada servo no frame. Calculado no inicio de cada frame

int potpin = 0;                           // analog pin used to connect the potentiometer
int val;                                  // variable to read the value from the analog pin
int frame_atual=0;                        // aponta para o frame atual
int ultimo_frame=0;                       // aponta para o ultimo frame aprendido
int frames[7][30];                        // guarda tempo movimento em ms e 6 servos por 30 posições da aprendizagem 

bool servos_habilitados=0;                // flag indicando se os servos estao habilitados ou nao
bool led_blink_fast = false;
bool isPressing_aprender=0, isPressing_executar=0;
int btn_aprender=0, btn_executar=0;
int tic_led_botao=0;
int tempo_restante=0;

unsigned long tempo_pixels;
unsigned long tempo_relatorio;
unsigned long lastTimeCalcServo;
unsigned long pressedTime_aprender;
unsigned long pressDuration_aprender;
unsigned long pressedTime_executar;
unsigned long pressDuration_executar;
//OBJETOS
Adafruit_PWMServoDriver servos = Adafruit_PWMServoDriver();    // called this way, it uses the default address 0x40
Adafruit_NeoPixel pixels(NUMPIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
//========================================================================================================
void setup() {
  Serial.begin(115200);
  tempo_relatorio=millis();
  
  pinMode(PIN_BTN_APRENDER, INPUT_PULLUP);
  pinMode(PIN_BTN_EXECUTAR, INPUT_PULLUP);
  pinMode(PIN_LED_APRENDER, OUTPUT);
  pinMode(PIN_LED_EXECUTAR, OUTPUT);
  //SERVOS
  servos.begin();                       // iniciando PCA9685...
  servos.setPWMFreq(60);                // Analog servos run at ~60 Hz updates
  delay(10);
  
  //LEDS
  tempo_pixels=millis();
  pixels.begin();                       // INITIALIZE NeoPixel strip object (REQUIRED)
  //pixels.clear();                       // Set all pixel colors to 'off'
  pixels.show();                        // Turn OFF all pixels ASAP
  pixels.setBrightness(100);             // Set BRIGHTNESS to about 1/5 (max = 255)
}

//========================================================================================================
void loop() {
  atualiza_entradas();    //le entradas
  atualiza_leds();        //atualiza leds \o/
  atualiza_movimento();   //verifica qual o estado e executa as tarefas
  if(millis() - tempo_relatorio > 1000){
    tempo_relatorio=millis();
    Serial.println();
    Serial.print("[INFO] ");
    Serial.print("Maquina estado: ");
    switch(estado_movimento){
      case 0:
        Serial.print("Parado. ");
      break;
      case 1:
        Serial.print("Aprender Knobs.");
      break;
      case 2:
        Serial.println("Aprender Manual.");
        for(int i=0; i<6; i++){
          Serial.print(" ");
          Serial.print(pots[i]);
          }
        //map(pots[i], servo_min_manual[i], servo_max_manual[i], servo_min[i], servo_max[i])
      break;
      case 3:
        Serial.print("Executando Movimentos.");
      break;
    }
    /*
    Serial.println();
    Serial.print("[INFO] ");
    Serial.print("botao Aprender: ");
    Serial.print(btn_aprender);
    Serial.print(" botao Executar: ");
    Serial.println(btn_executar);
    */
    
    for(int i=0;i<6; i++){
      Serial.print(servo_value[i]);
      Serial.print(" ");
      }
    Serial.println();
    
    }
}
