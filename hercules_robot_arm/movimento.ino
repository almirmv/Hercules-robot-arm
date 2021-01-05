void atualiza_movimento(){
switch(estado_movimento){
  case 0: //PARADO
    if(!servos_habilitados){      //se servos não estao habilitados...
      servos.wakeup();            //inicializa os pulsos dos servos
      servos_habilitados=1;
      }
    for(int i=0; i<NUMSERVOS; i++){                                           //mapeia a entrada dos knobs diretamente com as saidas...
      servo_value[i] = map(knobs[i], 0, 1024, servo_min[i], servo_max[i]);    //map(value, fromLow, fromHigh, toLow, toHigh)
      }
    
    set_servos();                     //enviar as posicoes atualizadas para os servos
    if(btn_aprender == 1){            //clique rapido no botao aprender?
      estado_movimento=1;             //vai para aprender via knobs
      frame_atual=0;
      ultimo_frame=0;
      }
    if(btn_aprender == 2){            //clique lomgo no botao aprender?
      frame_atual=0;
      ultimo_frame=0;
      estado_movimento=2;             //vai para aprender manual 
      }
    if(btn_executar == 1){            //executar?
      if(ultimo_frame > 0){           //possui algo para executar?
        frame_atual=0;                //iniciar do primeiro frame.
        calc_tempo_frame();           //calcular o tempo do primeiro frame.
        lastTimeCalcServo=millis();   //atualiza o timer dos servos...
        estado_movimento=3;           //DA O PLAY \o/
        }
      }
    
  break;
  case 1: //APRENDER KNOBS
    if(!servos_habilitados){      //se servos não estao habilitados...
      servos.wakeup();            //inicializa os pulsos dos servos
      servos_habilitados=1;
    }
    for(int i=0; i<NUMSERVOS; i++){                                           //mapeia a entrada dos knobs diretamente com as saidas...
      servo_value[i] = map(knobs[i], 0, 1024, servo_min[i], servo_max[i]);    //map(value, fromLow, fromHigh, toLow, toHigh)
      }
    set_servos();                                                             //enviar as posicoes atualizadas para os servos
    
    if(btn_aprender == 1){
      //guardar as posiçoes...
      Serial.print("FRAME ");
      Serial.print(frame_atual);
      Serial.print(" [");
      for(int i=0; i< NUMSERVOS; i++){                                                    //mapeia a entrada dos knobs diretamente com as saidas...
        Serial.print(" ");
        frames[i+1][frame_atual] = map(knobs[i], 0, 1024, servo_min[i], servo_max[i]);    //map(value, fromLow, fromHigh, toLow, toHigh)
        Serial.print(frames[i+1][frame_atual]);
      }
      Serial.println("]");
      frame_atual++;
      ultimo_frame++;
      Serial.println("posição guardada!");
    }
    if(btn_executar == 1){
      //finalizar aprendizado...
      if(frame_atual > 0){                   //volta ambos pois apontavam pra posição nao preenchida
        frame_atual--;
        ultimo_frame--;
      }
      Serial.println("finalizando aprendizado!");
      estado_movimento=0;       //volta para PARADO
    }
  break;
  case 2: //APRENDER MANUAL
    if(servos_habilitados){      //se servos estao habilitados...
      servos.sleep();            //desliga os pulsos dos servos
      servos_habilitados=0;
      }
    if(btn_aprender == 1){
      //guardar as posiçoes...
      
      Serial.print("FRAME ");
      Serial.print(frame_atual);
      Serial.print(" [");
      for(int i=0; i< NUMSERVOS; i++){                                                    //mapeia a entrada dos knobs diretamente com as saidas...
        Serial.print(" ");
        frames[i+1][frame_atual] = map(pots[i], servo_min_manual[i], servo_max_manual[i], servo_min[i], servo_max[i]);    //map(value, fromLow, fromHigh, toLow, toHigh)
        servo_value[i] = frames[i+1][frame_atual];    //atualiza tambem a posiçao atual do servo...
        Serial.print(frames[i+1][frame_atual]);
      }
      Serial.println("]");
      frame_atual++;
      ultimo_frame++;
      Serial.println("posição guardada!");
    }
    if(btn_executar == 1){
      //finalizar aprendizado...
      if(frame_atual > 0){
        frame_atual--;
        ultimo_frame--;
      }
      
      Serial.println("finalizando aprendizado!");
      estado_movimento=0;       //volta para PARADO
    }  
      
  break;
  case 3: //EXECUTAR MOVIMENTOS
    calc_nova_pos();              //recalcula posiçoes e frames...
    set_servos();                 //envia as posicoes atualizadas para os servos
    
    
    
    if(btn_executar == 1){        //STOP?
      frame_atual=0;              //volta para primeiro frame.
      calc_tempo_frame();         //calcular o tempo do primeiro frame.
      estado_movimento=0;         //PARADO...
    }
  break;
  }  
   

}

//--------------------------------------------------------------------------------------------------
//atualiza os servos com a posição calculada mais recente
void set_servos(){     
  for(int i=0; i<6; i++){
    servos.writeMicroseconds(i, int(servo_value[i]));
  }
return;
}

//--------------------------------------------------------------------------------------------------
//calcula o tempo para executar o frame
void calc_tempo_frame(){

//descobrir o maior angulo
//busca pela maior diferença de angulo do frame atual com os angulos atuais dos servos
  int maior_diferenca=0;   //angulos em graus
  int diferenca;
   
  for(int i=0;i<6;i++){
    diferenca = abs(servo_value[i] - frames[i+1][frame_atual]);    //pos atual varia de 0a4, frames tem angulos de 1 a 5 pois pos 0 é do tempo.
    if(diferenca > maior_diferenca){
      maior_diferenca = diferenca;                                  //salva o maior angulo em ms
    }
  } 
  //OBS: como "angulo" varia geralmente de 500us a 2500us (2mil) para 180graus...
  //entao 1ms para cada us daria 2000ms para 180 graus
  if(maior_diferenca < 1000){
    frames[0][frame_atual] = 1000;                            //seta minimo de 1000ms por frame
  }else{
    frames[0][frame_atual] = maior_diferenca;    //tempo em ms= distancia(graus)*1000 / velocidade angular(graus por segundo)
    }
  tempo_restante=frames[0][frame_atual];                      //agora copia em tempo restante...
  //calcular as velocidades de cada servo durante esse frame.
  int tic = frames[0][frame_atual] / INTERVALO_SERVOS;
  for(int i=0; i < NUMSERVOS; i++){
    vel_servos[i] = float(frames[i+1][frame_atual] - servo_value[i]) / tic; 
  }

  
  
return;  
}

//--------------------------------------------------------------------------------------------------
//calcula nova posição dos servos
void calc_nova_pos(){
  if(millis()-lastTimeCalcServo < INTERVALO_SERVOS){      //se ainda nao deu o tempo apenas ignore.
    return;
  }
  lastTimeCalcServo=millis();                             //atualiza o timer e vamos calcular!...
  
  if(tempo_restante <= 0){                                //terminou o frame?
    for(int i=0; i<6; i++){
      servo_value[i] = frames[i+1][frame_atual];          //finaliza o frame com os angulos exatos pedidos.
    }
    frame_atual++;                                        //avança proximo frame
    if(frame_atual > ultimo_frame){                       //passou do ultimo frame?
      frame_atual=0;                                      //volta para o primeiro frame.
    }
    calc_tempo_frame();                                   //recalcula tempo para o novo frame...
  }
  
  //calcula o novo angulo para cada servo
  //angulo atual + ((angulo final-angulo atual)/tempo de movimento)
  for(int i=0; i<6; i++){
    servo_value[i] = servo_value[i] + vel_servos[i];  //a cada tic add deslocamento (velocidade no tempo)
    }
  tempo_restante -= INTERVALO_SERVOS;                //atualiza tempo restante
  /*
  Serial.print("F ");
  Serial.print(frame_atual);
  Serial.print(" T: ");
  Serial.print(tempo_restante);                      //tempo restante
  Serial.print("/");
  Serial.print(frames[0][frame_atual]);              //tempo total do frame
  Serial.print(" [");
  for(int i=0; i< NUMSERVOS; i++){                   // mostar posição "atual/destino" de cada servo
    Serial.print(" ");
    Serial.print(servo_value[i]);
    Serial.print("/");
    Serial.print(frames[i+1][frame_atual]);
  }
  Serial.println("]");
  */
}

//--------------------------------------------------------------------------------------------------
