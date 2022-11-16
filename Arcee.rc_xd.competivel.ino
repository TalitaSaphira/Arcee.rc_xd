// ARCEE.RC_XD
// Microprocessador: Arduino MEGA

// ==== Pinos utilizados pela ponte H ===
int ENA = 6;      // motor da direita em uma porta pwm
int ENB = 4;      // motor da esquerda em uma porta pwm
int IN1 = 2;     // motor da direita, girar para frente   //Portas digitais
int IN2 = 26;     // motor da direita, girar para trás
int IN3 = 30;     // motor da esquerda, girar para frente
int IN4 = 34;     // motor da esquerda, girar para trás

// ==== Pinos utilizados pelos sensores ===
int sensor1 = A0;       // frente e meio               // Portas analógicas
int sensor2 = A1;       // frente e direita            
int sensor3 = A2;       // frente e direita externa    //Mudar o valor dos sensores? (ao invés de valere, 0 ou 1 valem de 0 a 5)
int sensor4 = A3;       // frente e esquerda
int sensor5 = A4;       // frente e esquerda externa
int sensor6 = A5;       // atrás e direita
int sensor7 = A6;       // atrás e esquerda

int s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0, s7 = 0;  //Recebem o valor do sensor
int media_sensor1 = 45;  //Diferencia a linha branca da preta
int media_sensor2 = 450;
int media_sensor3 = 450;
int media_sensor4 = 45;
int media_sensor5 = 450;

// === Constantes e variáveis para PID ===
double Kp = 1;       // valor a ser definido por meio de testes
double Ki = 1;       // valor a ser definido por meio de testes
double Kd = 1;       // valor a ser definido por meio de testes
int erro, erro1 = 0;
int proporcional, integral, derivada;
double PID;
byte velA, velB;


//OBS: podem ser definidas velocidades padrão diferentes para os motores caso necessário;
//OBS: no primeiro teste a variável velocidade deve ser reduzida;
//OBS: a variável "byte" armazena valores numéricos de 8-bit sem sinal, que vão de 0 à 255;


// === Velocidade do motor, delay, curva de 90º ===
int quad_A;               // quadrados à direita
int quad_B;               // quadrados à esquerda
byte velocidade = 150;    // controle de velocidade
int delay90 = 65;         //Tempo para realizar a curva de 90º
int prox_quad = 60;       //Tempo para chegar de um quadrado ao outro
int delay_faixa = 100;    //Tempo para chegar na faixa de pedestres
int delay_inverte = 65;   //Tempo de inversão dos motores para parada brusca
int parada_brusca = 60;   //Intensidade de rotação dos motores ao contrário para uma parada brusca;     //Testar com o valor de 255        

void setup() {
  pinMode (ENA, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (sensor1, INPUT);
  pinMode (sensor2, INPUT);
  pinMode (sensor3, INPUT);
  pinMode (sensor4, INPUT);
  pinMode (sensor5, INPUT);
  pinMode (sensor6, INPUT);
  pinMode (sensor7, INPUT);

  delay (10000);

  //Serial.begin (9600);  //Abre o monitor serial

  // === Inicialicando o Arcee ===
  
  
  /* === Dar uma volta no mesmo lugar ===
  analogWrite (ENA, velocidade);
  digitalWrite (IN1, 1);
  digitalWrite (IN2, 0);
  analogWrite (ENB, velocidade);
  digitalWrite (IN3, 0);
  digitalWrite (IN4, 1);

 // OBS: definir o tempo do robô girar ou criar uma condição de parada
 // OBS: não deixar que o robô já começe torto na linha
  */
  
}
// === Leitura dos sensores ===
  void le_sensores (){
    int m1=0, m2=0, m3=0, m4=0, m5=0;
  for (int i =0; i<=9; i++){
    m1 += analogRead (sensor1);
    m2 += analogRead (sensor2);
    m3 += analogRead (sensor3);
    m4 += analogRead (sensor4);
    m5 += analogRead (sensor5);
    delay (1);
  }
  m1 = m1/10;
  m2 = m2/10;
  m3 = m3/10;
  m4 = m4/10;
  m5 = m5/10;
  
    s1 = m1;    
    s2 = m2;
    s3 = m3;
    s4 = m4;
    s5 = m5;

  }

 // === Controle PID ===
  void calcula_erro (){
    if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 0;
    else if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 1;
    else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 > media_sensor4) && (s5 <= media_sensor5) ) erro = -1;
    else if ((s1 <= media_sensor1) && (s2 > media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 2;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 > media_sensor4) && (s5 <= media_sensor5) ) erro = -2;
    else if ((s1 <= media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 3;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ) erro = -3;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 4;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ) erro = -4;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = 5;
    else if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ) erro = -5;
  }
  void calcula_PID (){
    if (erro = 0)
        integral = 0;
    proporcional = erro;
    integral += erro;
    if (integral > 255)
        integral = 255;
    else if (integral < -255)
        integral = -255;
    derivada = erro - erro1;
    PID = (Kp*proporcional) + (Ki*integral) + (Kd*derivada);
    erro1 = erro;
  }
  void controla_motor (){
    if (PID >= 0){    //vira para a direita
        velA = velocidade - PID;
        velB = velocidade;
    }
    else {            //vira para a esquerda
        velA = velocidade;
        velB = velocidade + PID;
    }
    analogWrite (ENA, velA);
    digitalWrite (IN1, HIGH);
    digitalWrite (IN2, LOW);
    analogWrite (ENB, velB);
    digitalWrite (IN3, HIGH);
    digitalWrite (IN4, LOW);
  }

int o = 1;
void loop() {
 
  // === Calibração dos sensores ===
   
   //Serial.print(0);
   //Serial.print(" ");
   //Serial.print(200);
   //Serial.print(" ");
   //Serial.print(a1*analogRead(sensor1));
   //Serial.println(" ");
   //Serial.println(a2*analogRead(sensor2));
   //Serial.print(" ");
   //Serial.print(a3*analogRead(sensor3));
   //Serial.print(" ");
   //Serial.print(a4*analogRead(sensor4));
   //Serial.print(" ");
   //Serial.print(a5*analogRead(sensor5));
   //Serial.print(" ");
   //Serial.print(a6*analogRead(sensor6));
  //Serial.print(" ");
  // Serial.print(a7*analogRead(sensor7));
   
 
  //Obs: Para realizar a calibração dos sensores, basta ir alterando o valor do número que multiplica o analogRead +
  //Obs: Esse número deve ser passado para a função lê sensores;
  
  le_sensores();     //lê o sinal de todos os sensores
  
  // === Movimentação do Arcee ===
  if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5)){    //Segue reto
     analogWrite (ENA, velocidade-18);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay(1);
  }
  else if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5)){  //Curva leve direita
     analogWrite (ENA, velocidade);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade-18);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay(1);
  }
  else if ((s1 <= media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5)){  //Curva acentuada direita
     analogWrite (ENA, velocidade);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade-34);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay(1);
  }
  else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 > media_sensor4) && (s5 <= media_sensor5)){  //Curva leve esquerda
      analogWrite (ENA, velocidade-36);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay(1);
  }
   else if ((s1 <= media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5)){  //Curva acentuada esquerda
     analogWrite (ENA, velocidade-52);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay(1);
  }
  if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){    //CRUZANMENTO
    analogWrite (ENA, velocidade);
    digitalWrite (IN1, HIGH);            // liga o motor 1
    digitalWrite (IN2, LOW);             // para frente
    analogWrite (ENB, velocidade);
    digitalWrite (IN3, HIGH);            // liga o motor 2
    digitalWrite (IN4, LOW);             //´para frenre
    // Ao encontrar um cruzamento Arcee deve seguir reto;
  }
  else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ){    //BIFUCAÇÃO EM T
    // Opção 1 - virar à direita
    analogWrite (ENA, parada_brusca);
    digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
    digitalWrite (IN2, HIGH);     
    analogWrite (ENB, 255);              // velocidade máxima
    digitalWrite (IN3, HIGH);            // liga o motor 2
    digitalWrite (IN4, LOW);
    delay (delay90);                          // tempo no qual a curva será efetuada;          
    }
    // Opção 2 - virar à esquerda
    /*
    analogWrite (ENA, 255);              // velocidade máxima
    digitalWrite (IN1, HIGH);            // liga o motor 1
    digitalWrite (IN2, LOW);             
    analogWrite (ENB, parada_brusca);
    digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
    digitalWrite (IN4, HIGH);
    delay (delay90);                          // tempo no qual a curva será efetuada;
    }            
    */
    // O caminho que acarreta no menor trajeto será informado na reunião antes da competição
  else if ((s1 > media_sensor1) && (s2<= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){    //CURVA DE 90° À DIREITA E ROTATÓRIA COM SAÍDA À DIREITA
    quad_A = 0;   // encontou o primeiro quadrado, porém esse não conta como saida da rotatória;
    delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
    le_sensores();
    if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) ){    //Não acha mais quadrados
      analogWrite (ENA, parada_brusca);
      digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
      digitalWrite (IN2, HIGH);           
      analogWrite (ENB, 255);              // velocidade máxima
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW); 
      delay (delay90);
    }
    else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){
      quad_A = 1;
      delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
      le_sensores();
      if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
        analogWrite (ENA, parada_brusca);
        digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
        digitalWrite (IN2, HIGH);          
        analogWrite (ENB, 255);              // velocidade máxima
        digitalWrite (IN3, HIGH);            // liga o motor 2
        digitalWrite (IN4, LOW); 
        delay (delay90);
      }
      else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){
        quad_A = 2;
        delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
        le_sensores();
        if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
          analogWrite (ENA, parada_brusca);
          digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
          digitalWrite (IN2, HIGH);           
          analogWrite (ENB, 255);              // velocidade máxima
          digitalWrite (IN3, HIGH);            // liga o motor 2
          digitalWrite (IN4, LOW); 
          delay (delay90);
        }
        else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){
          quad_A = 3;
          delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
          le_sensores();
          if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
            analogWrite (ENA, parada_brusca);
            digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
            digitalWrite (IN2, HIGH);        
            analogWrite (ENB, 255);              // velocidade máxima
            digitalWrite (IN3, HIGH);            // liga o motor 2
            digitalWrite (IN4, LOW); 
            delay (delay90);
          }
        }
      }
    }
  }
  else if ((s1> media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){
    if (quad_A == 1){
      analogWrite (ENA, parada_brusca);
      digitalWrite (IN1, LOW);             // inverte a rotação do motor 1 (parada brusca)
      digitalWrite (IN2, HIGH);          
      analogWrite (ENB, 255);              // velocidade máxima
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW); 
      delay (delay90);
    }
    else{
      analogWrite (ENA, velocidade);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);           // para fente
      analogWrite (ENB, velocidade);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);            // para frenre
      delay (delay90);
    }
    quad_A -= 1;
  }
  else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ){    //CURVA DE 90° À ESQUERDA E ROTATÓRIA COM SAÍDA À ESQUERDA
    quad_B = 0;
    delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
    le_sensores();
    if ((sensor1 > media_sensor1) && (sensor4 > media_sensor4) && (sensor5 > media_sensor5)){
      analogWrite (ENA, 255);              // velocidade máxima
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             
      analogWrite (ENB, parada_brusca);
      digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
      digitalWrite (IN4, HIGH);   
      delay (delay90);    
    }
    else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ){
      quad_B = 1;
      delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
      le_sensores();
      if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
        analogWrite (ENA, 255);              // velocidade máxima
        digitalWrite (IN1, HIGH);            // liga o motor 1
        digitalWrite (IN2, LOW);             
        analogWrite (ENB, parada_brusca);
        digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
        digitalWrite (IN4, HIGH);   
        delay (delay90);        
      }
      else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ){
        quad_B = 2;
        delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
        le_sensores();
        if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
          analogWrite (ENA, 255);              // velocidade máxima
          digitalWrite (IN1, HIGH);            // liga o motor 1
          digitalWrite (IN2, LOW);             
          analogWrite (ENB, parada_brusca);
          digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
          digitalWrite (IN4, HIGH);  
          delay (delay90);
        }
        else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 > media_sensor5) ){
          quad_B = 3;
          delay (prox_quad);   // tempo para sair do quadrado e chegar a linha ou próx. quadrado
          le_sensores();
          if ((s1 > media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
            analogWrite (ENA, 255);              // velocidade máxima
            digitalWrite (IN1, HIGH);            // liga o motor 1
            digitalWrite (IN2, LOW);             
            analogWrite (ENB, parada_brusca);
            digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
            digitalWrite (IN4, HIGH);       
            delay (delay90);
          }
        }
      }
    }
  }
  else if ((s1 > media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){
    if (quad_B == 1){
      analogWrite (ENA, 255);              // velocidade máxima
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             
      analogWrite (ENB, parada_brusca);
      digitalWrite (IN3, LOW);             // inverte a rotação o motor 2 (parada brusca)
      digitalWrite (IN4, HIGH);  
      delay (delay90);
    }
    else{
      analogWrite (ENA, velocidade);
      digitalWrite (IN1, HIGH);            // liga o motor 1
      digitalWrite (IN2, LOW);             // para a frente
      analogWrite (ENB, velocidade);
      digitalWrite (IN3, HIGH);            // liga o motor 2
      digitalWrite (IN4, LOW);             // para a frente
      delay (delay90);
    }
  }
  else if ((s1 <= media_sensor1) && (s2 > media_sensor2) && (s3 > media_sensor3) && (s4 > media_sensor4) && (s5 > media_sensor5) ){    //FAIXA DE PEDESTRE : INVERSÃO DAS CORES DA PISTA
    delay (delay_faixa);                // dar tempo do robô chegar na outra pista antes de parar
    analogWrite (ENA, parada_brusca);
    digitalWrite (IN1, LOW);            // inverte a rotação do motor 1
    digitalWrite (IN2, HIGH);           
    analogWrite (ENB, parada_brusca);
    digitalWrite (IN3, LOW);            // inverte a rotação do motor 2
    digitalWrite (IN4, HIGH);
    delay (delay_inverte);              // matem a inversão por 0.065 segundos
    analogWrite (ENA, 0);
    digitalWrite (IN1, LOW);            // para o motor 1
    digitalWrite (IN2, LOW);           
    analogWrite (ENB, 0);
    digitalWrite (IN3, LOW);            // para o motor 2
    digitalWrite (IN4, LOW);          
    delay (6000);                       // pausa de no minímo 6 seg na faixa
     analogWrite (ENA, velocidade);
    digitalWrite (IN1, HIGH);           // liga o motor 1
    digitalWrite (IN2, LOW);            // para frente
    analogWrite (ENB, velocidade);
    digitalWrite (IN3, HIGH);           // liga o motor 2
    digitalWrite (IN4, LOW);            // para frenre
    delay (350);                        // dar tempo do Arcee sair da faixa antes de começar a receber os sinais dos sensores de novo.
  }
  else if ((s1<= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){    //FIM DA LINHA
    delay (200);                       // pausa antes de desligar os motores
    le_sensores();
    if ((s1 <= media_sensor1) && (s2 <= media_sensor2) && (s3 <= media_sensor3) && (s4 <= media_sensor4) && (s5 <= media_sensor5) ){    // se mesmo após o delay todos os sensores continuarem 0 o robô para
      analogWrite (ENA, parada_brusca);
      digitalWrite (IN1, LOW);            // inverte a rotação do motor 1
      digitalWrite (IN2, HIGH);           
      analogWrite (ENB, parada_brusca);
      digitalWrite (IN3, LOW);            // inverte a rotação do motor 2
      digitalWrite (IN4, HIGH);
      delay (delay_inverte);              // matem a inversão por 0.080 segundos
      analogWrite (ENA, 0);
      digitalWrite (IN1, LOW);            // para o motor 1
      digitalWrite (IN2, LOW);           
      analogWrite (ENB, 0);
      digitalWrite (IN3, LOW);            // para o motor 2
      digitalWrite (IN4, LOW);
      delay (6400000);                    // faz o arcee ficar parado por 2 horas após encontrar o fim da linha 
    }           
  }
 
  else {      // realiza curvas leves e acentuadas, andar em linha reta;
    calcula_erro();
    calcula_PID();
    controla_motor();
  }
}
