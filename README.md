# Hercules Braço robótico
Firmware para controle e aprendizado de braço robótico com servos.

### Firmware
Possui 3 modos de operação:
- Manual por Knobs: você opera através dos potenciometros localizados na base do braço. Cada junta está mapeada em um pontenciometro.
- Aprendizagem por Knobs: Um clique simples no "Botão aprender" aciona a aprendizagem por knobs. Ajuste a posição do braço pelos knobs e a cada clique simples no "Botão aprender" fará o salvamento desta nova posição. Para finalizar o aprendizado apertar "Botão executar".
- Aprendizagem Manual: Um clique longo no "Botão aprender" aciona a aprendizagem manual. Os servos serão desabilitados. Com isso, você pode posicionar o braço com a mão na posição que quiser. Da mesma forma a cada clique simples no "Botão aprender" fará o salvamento desta nova posição.
- Modo play:  Caso existam movimentos salvos, o led do "Botão executar" ficará piscando indicando que o braço possui movimentos salvos e ao pressiona-lo serão executados. O braço ficará em loop executando de forma automática os movimentos gravados na aprendizagem. O movimento possui interpolação por tempo, sendo que cada junta possui velocidade diferente para atender a posição no final de cada posição.

### Hardware
- x4 MG996
- sg90 (garra)
- Arduino Mega 2560
- PCA985 (Adafruit_PWMServoDriver) para controle dos servos
- leds RGB WS2812 (Adafruit_NeoPixel) para iluminação RGB individual das letras H E R C U L E S que ficam na base frontal do braço.
- x2 botões com led embutido, para "Botão aprender" e "Botão executar" 
- Fonte 5v 5A.

