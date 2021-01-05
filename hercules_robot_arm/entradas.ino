void atualiza_entradas(){
  //faz leitura dos knobs da interface
  for(int i=0; i<6; i++){
    knobs[i]= analogRead(i); //lê e salva o valor de cada knob
    }
  
  //faz leitura dos potenciometros internos dos servos e descobre sua real posição    
  for(int i=6; i<12; i++){
    pots[i-6]= analogRead(i); //lê e salva o valor do pot interno de cada servo
    }  
  
  //faz leitura dos botoes
  //btn_aprender = digitalRead(PIN_BTN_APRENDER);
  //btn_executar = digitalRead(PIN_BTN_EXECUTAR);
  
  //calcula esalva em "timer_btn_aprender" a quantidade de ms que o botao ficou pressionado-----------------------------------
  //Rotina não blocante. Qualquer tempo acima de zero indica que o botao esta sendo pressionado. 
  if(!digitalRead(PIN_BTN_APRENDER)){                             //botao pressionado?
    if(isPressing_aprender){                                      //ja estava pressionado?
      pressDuration_aprender = millis() - pressedTime_aprender;   //atualiza duração do tempo de botao pressionado
      if(pressDuration_aprender > LONG_PRESS_TIME){           //se atingir a marca do long press... atualiza flag do baotoa com tipo 2 de aperto. 
        btn_aprender=2;                     
      }
    }else{
      pressedTime_aprender = millis();                            //botao acabou de ser pressionado pega o tempo inicial do aperto...
      isPressing_aprender = true;                                 //indicar que foi pressionad pro proximo loop
    }
  }else{                                                          //botao nao pressionado...
    if(isPressing_aprender){                                      //estava pressionado?
      isPressing_aprender = false;                                //acabou de ser liberado! indicar que foi liberado pro proximo loop
      if(pressDuration_aprender > DEBOUNCE && pressDuration_aprender < SHORT_PRESS_TIME){        //se for maior que Debounce e menor que shortpress... atualiza flag do botao com tipo 1 de aperto. 
        btn_aprender=1;                     
      }else{
        btn_aprender=0;
        }
    }else{
      btn_aprender=0;
      }
  }
//calcula e salva em "timer_btn_aprender" a quantidade de ms que o botao ficou pressionado-----------------------------------
  //Rotina não blocante. Qualquer tempo acima de zero indica que o botao esta sendo pressionado. 
  if(!digitalRead(PIN_BTN_EXECUTAR)){                             //botao pressionado?
    if(isPressing_executar){                                      //ja estava pressionado?
      pressDuration_executar = millis() - pressedTime_executar;   //atualiza duração do tempo de botao pressionado
      if(pressDuration_executar > LONG_PRESS_TIME){           //se atingir a marca do long press... atualiza flag do baotoa com tipo 2 de aperto. 
        btn_executar=2;                     
      }
    }else{
      pressedTime_executar = millis();                            //botao acabou de ser pressionado pega o tempo inicial do aperto...
      isPressing_executar = true;                                 //indicar que foi pressionad pro proximo loop
    }
  }else{                                                          //botao nao pressionado...
    if(isPressing_executar){                                      //estava pressionado?
      isPressing_executar = false;                                //acabou de ser liberado! indicar que foi liberado pro proximo loop
      if(pressDuration_executar > DEBOUNCE && pressDuration_executar < SHORT_PRESS_TIME){        //se for mair que Debounce e menor que shortpress... atualiza flag do botao com tipo 1 de aperto. 
        btn_executar=1;                     
      }else{
        btn_executar=0;
        }
    }else{
      btn_executar=0;
      }
  }

/*
  //calcula esalva em "timer_btn_executar" a quantidade de ms que o botao ficou pressionado----------------------------------
  //Rotina não blocante. Qualquer tempo acima de zero indica que o botao esta sendo pressionado. 
  if(!digitalRead(PIN_BTN_EXECUTAR)){                             //botao pressionado?
    if(btn_executar_old){                                         //ja estava pressionado?
      timer_btn_executar = millis() - inicio_timer_btn_executar;  //atualiza duração do tempo de botao pressionado
    }else{
      inicio_timer_btn_executar = millis();                       //botao acabou de ser pressionado
      btn_executar_old=1;                                         //indicar que foi pressionad pro proximo loop
    }
  }else{                                                          //botao nao pressionado...
    if(btn_executar_old){                                         //estava pressionado?
      btn_executar_old=0;                                         //acabou de ser liberado! indicar que foi liberado pro proximo loop
      timer_btn_executar=0;                                       //atualiza duração do tempo de botao pressionado para zero 
    }
  }
*/
}
