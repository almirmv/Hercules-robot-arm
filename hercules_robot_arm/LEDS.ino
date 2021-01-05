 void atualiza_leds(){
  if(millis() - tempo_pixels > 10){
    atualiza_led_painel();                          //chamando essa função a cada 10ms...
    delay_led++;
    //pixels.clear(); // Set all pixel colors to 'off'
    
    switch(efeito_atual_led){
      case 0:
        // "COLOR WIPE" Fill along the length of the strip in various colors...============================================================
        switch(etapa_efeito){
          case 0: //VAI PREENCHER TODA A FITA COM A COR ESPECIFICADA---------------------------------
            rgbcolor = pixels.Color(255, 0, 0);      //VERMELHO
            pixels.setPixelColor(led_atual, rgbcolor);    //Set pixel's color (in RAM)
            pixels.show();                                //Update strip to match
            if(delay_led > 15){                            //150ms?
              delay_led=0;
              led_atual++;                                //quando passar 50ms no ultimo led... vai incrementar para NUMPIXELS
            }
              
            if(led_atual == NUMPIXELS){                    //acabou a fita?
              etapa_efeito++;
              led_atual=0;
            }
          break;
          case 1: //VAI PREENCHER TODA A FITA COM A COR ESPECIFICADA-----------------------------------
            rgbcolor = pixels.Color(0, 255, 0);      //VERDE
            pixels.setPixelColor(led_atual, rgbcolor);    //Set pixel's color (in RAM)
            pixels.show();                                //Update strip to match
            if(delay_led > 15){                            //150ms?
              delay_led=0;
              led_atual++;                                //quando passar 50ms no ultimo led... vai incrementar para NUMPIXELS
            }
              
            if(led_atual == NUMPIXELS){                    //acabou a fita?
              etapa_efeito++;
              led_atual=0;
            }
          break;
          case 2: //VAI PREENCHER TODA A FITA COM A COR ESPECIFICADA-----------------------------------
            rgbcolor = pixels.Color(0, 0, 255);      //AZUL
            pixels.setPixelColor(led_atual, rgbcolor);    //Set pixel's color (in RAM)
            pixels.show();                                //Update strip to match
            if(delay_led > 15){                            //150ms?
              delay_led=0;
              led_atual++;                                //quando passar 50ms no ultimo led... vai incrementar para NUMPIXELS
            }
              
            if(led_atual == NUMPIXELS){                    //acabou a fita?
              etapa_efeito++;
              led_atual=0;
            }
          break;
          case 3: //VAI PREENCHER TODA A FITA COM A COR ESPECIFICADA-----------------------------------
            rgbcolor = pixels.Color(255, 165, 0);         //LARANJA
            pixels.setPixelColor(led_atual, rgbcolor);    //Set pixel's color (in RAM)
            pixels.show();                                //Update strip to match
            if(delay_led > 15){                           //150ms?
              delay_led=0;
              led_atual++;                                //quando passar 50ms no ultimo led... vai incrementar para NUMPIXELS
            }
              
            if(led_atual == NUMPIXELS){                   //acabou a fita?
              efeito_atual_led=1;                         //AVANÇA PARA NOVO EFEITO \o/
              delay_led=0;
              etapa_efeito=0;
              led_atual=0;
              rgbcolor=0;
            }
          break;
          }//FIM "COLOR WIPE"
      break;
      
      case 1: //RAINBOWFADE ===========================================================================================
        if(delay_led > 5){                              //50ms?
          
            rgbcolor += 256;                               //Color wheel has a range of 65536 but it's OK if we roll over
            uint32_t pixelHue = pixels.ColorHSV(rgbcolor, 255, 255);
            pixels.fill(pixelHue);                         //aplica a todos a mesma cor
            pixels.show();                                //Update strip to match
            delay_led=0;
            etapa_efeito++;
        }     
       
        if(etapa_efeito > 256){                         //uma volta no HUE vai ter 256 etapas...
            efeito_atual_led=0;                         //AVANÇA PARA NOVO EFEITO \o/
            delay_led=0;
            etapa_efeito=0;
            led_atual=0;
        }
              
      break;
      }
    
    
    
    
    
    
    
    
    /*
    //hue de 0 a 65536 (por 360 graus da 182,44)
    //saturation de 0 a 255
    //value de 0 a 255
    
//    a = 255;
    rgbcolor = pixels.ColorHSV(7463, 255, a);        //passa de HUE para RGB   //strip.ColorHSV(hue, saturation, value)
    pixels.fill(rgbcolor);                           //aplica a todos a mesma cor
   
    for(i_led=0; i_led < 5; i_led++){
      pixels.setPixelColor(i_led, rgbcolor);   //laranja
      }
   
    //pixels.setPixelColor(i_led, rgbcolor);   //laranja
    //pixels.setPixelColor(i_led, pixels.Color(255, 165, 0));   //laranja
    pixels.show();   // Send the updated pixel colors to the hardware.
    a++;
    if(a == 255){
      a=0;
      }
     */
    tempo_pixels = millis();
  }
}
//----------------------------------------------------------------------------------------------------------------
//atualizar LED DOS BOTÕES APRENDER E EXECUTAR
void atualiza_led_painel(){
  tic_led_botao++;                            //cada tic tem 10ms...
  if(tic_led_botao >= 30){                      //300ms ?
    tic_led_botao=0;
    led_blink_fast=!led_blink_fast;             //alterna
    }
 
  switch(estado_movimento){
    case 0: //PARADO
      digitalWrite(PIN_LED_APRENDER, LOW);      //ambos os leds apagados indicando "free to play" via knobs
      if(ultimo_frame > 0){
        digitalWrite(PIN_LED_EXECUTAR, HIGH);   //existem movimentos para executar!
      }else{
        digitalWrite(PIN_LED_EXECUTAR, LOW);    //
      }
    break;
    
    case 1: //APRENDER KNOBS (500ms on 500ms off)
      digitalWrite(PIN_LED_APRENDER, HIGH);               //ON
      digitalWrite(PIN_LED_EXECUTAR, LOW);                //desliga led de executar
    break;
    
    case 2: //APRENDER MANUAL (200ms OFF 200ms ON)
      digitalWrite(PIN_LED_APRENDER, led_blink_fast);     //300ms ON e 300ms off
      digitalWrite(PIN_LED_EXECUTAR, LOW);                //desliga led de executar
      
    break;
    case 3: //EXECUTAR MOVIMENTOS (200ms OFF 200ms ON)
      digitalWrite(PIN_LED_APRENDER, LOW);                //desliga led de aprender
      digitalWrite(PIN_LED_EXECUTAR, led_blink_fast);     //300ms ON e 300ms off
    break;
  }//FIM SWITCH  

}

//----------------------------------------------------------------------------------------------------------------
