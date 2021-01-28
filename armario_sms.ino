#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);  // Criando um LCD de 16x4 no endereço 0x20 Ligado no SCL(21) e SDA(20) 
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <BlynkSimpleEthernet.h>
#include <SoftwareSerial.h>

#define BLYNK_PRINT Serial
#define W5100_CS  10
#define SDCARD_CS 4
#define qtdportas 9
#define numero_de_andares 22
#define aptos_por_andar 3

char auth[] = "5uw5iWa9jj5Mo6r6lE-WErakPto15Msh";
SoftwareSerial serialGSM(18,19); // RX - Conectar no TX do SIM800L, TX - Conectar no RX do SIM800L
String telefoneSMS; String telefoneBase = "+5561992248399"; String telefonecondomino; String mensagemSMS; String Dados;
String unidade;String Numtelstring; String TelAux; String telefone1; char Numtelchar;String telefone;String PIN = " ";char PIN_SD;
String key[9];
String numero[11];
//String key[1] = " ";String key[2] = " ";String key[3] = " ";String key[5] = " ";
//String key[5] = " ";String key[6] = " ";String key[7] = " ";String key[8] = " ";
String teclaacionada2 = " ";
String PINPORTA[10]={"        ","        ","        ","        ","        ","        ","        ","        ","        ","        "}; // DESPRESANDO O PINPORTA[0] PARA SER DIDÁTICO
String CONFIGURACAO;String senhamaster;String senhavisualiza;String senhaabreportas;String senhazerapins;
String senhaalteracelular;
int pinosLinhas[]  = {40,42,44,46};
int pinosColunas[] = {41,43,45};
int pinValue;
int i, j, c, k, h,z;
char teclas[4][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};
char teclaacionada1;int numporta;int position = 0; //VARIÁVEL PARA LEITURA DE POSIÇÃO DA TECLA PRESSIONADA
int portaaberta[10];  int porta[10];
int func_pin = 0;
int abastece = 0;
int contador = 0;
int abreportas = 0;
int andar;
int apto;
bool unidade_existe = true;
 int salvando;





void configuraGSM();
//void enviaSMS(String telefone, String mensagem);
WidgetBridge Porta1(V30);WidgetBridge Porta2(V31);WidgetBridge Porta3(V32);
WidgetBridge Porta4(V34);WidgetBridge Porta5(V35);WidgetBridge Porta6(V36);
WidgetBridge Porta7(V37);WidgetBridge Porta8(V38);WidgetBridge Porta9(V39);

BlynkTimer timer;

void blynkAnotherDevice() // Here we will send HIGH or LOW once per second
{
  Serial.println("Passei no BlynkAnotherDevice");
  if (portaaberta[1] == 1) {Porta1.virtualWrite(V10, 1); Serial.println("passei dentro da porta 1");portaaberta[1] = 0;}
  if (portaaberta[2] == 1) {Porta2.virtualWrite(V2, 2); Serial.println("passei dentro da porta 2"); portaaberta[2] = 0;}
  if (portaaberta[3] == 1) {Porta3.virtualWrite(V3, 3); Serial.println("passei dentro da porta 3"); portaaberta[3] = 0;}
  if (portaaberta[4] == 1) {Porta4.virtualWrite(V4, 4); Serial.println("passei dentro da porta 4"); portaaberta[4] = 0;}
  if (portaaberta[5] == 1) {Porta5.virtualWrite(V5, 5); Serial.println("passei dentro da porta 5"); portaaberta[5] = 0;}
  if (portaaberta[6] == 1) {Porta6.virtualWrite(V6, 6); Serial.println("passei dentro da porta 6"); portaaberta[6] = 0;}
  if (portaaberta[7] == 1) {Porta7.virtualWrite(V7, 7); Serial.println("passei dentro da porta 7"); portaaberta[7] = 0;}
  if (portaaberta[8] == 1) {Porta8.virtualWrite(V8, 8); Serial.println("passei dentro da porta 8"); portaaberta[8] = 0;}
  if (portaaberta[9] == 1) {Porta9.virtualWrite(V9, 9); Serial.println("passei dentro da porta 9"); portaaberta[9] = 0;}
  Blynk.run();
}

BLYNK_CONNECTED() {
  Porta1.setAuthToken("BobF0jCZEgLcW5ePBHkWfG3BY6QZjBM1");
  Porta2.setAuthToken("hon18fUPahi8DINI9MUdpN6hBXgMsiSp");
  Porta3.setAuthToken("xJ-svKj5uJXem-etcpFR5-v8SoG28b7I");
  Porta4.setAuthToken("XxwDpf7C9fyDcpOzF7UGfo0iNWsXDPBt");
  Porta5.setAuthToken("jSFUZRC0TCDKT0vEEw58ykvxqcWV-w_v");
  Porta6.setAuthToken("VSMAHIZeqOj2JBF4lBybW7UqlXWLowOg");
  Porta7.setAuthToken("6Z0vo7MHtjhpao0Yvy4gQjiev6itX7Cf");
  Porta8.setAuthToken("2ujDYyKOq9URMdKl2IcNe1Ydi7aoJ_qM");
  Porta9.setAuthToken("ztXyjm1KnOz8ZbswFohdlMsZEAjFic6K");
}

BLYNK_WRITE(V1)
{
  pinValue = portaaberta[1];
  Serial.print("V1: ");
  Serial.println(pinValue);
}

BLYNK_WRITE(V11)
{
  Serial.print("V11: "); Serial.println(param.asInt());
   
//FUNÇÃO PIN 
 if(func_pin == 1 && contador == param.asInt()){
   
    func_pin = 0;
    lcd.clear();         
    lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
    lcd.setCursor(0,2); lcd.print("PORTA"); lcd.setCursor(6,2); lcd.print(contador); lcd.setCursor(8,2); lcd.print("LIBERADA"); 
    lcd.setCursor(0,3); lcd.print("RETIRE O VOLUME");delay(4000);
    lcd.clear();
    enviaSMS(telefoneBase, "Le Grand Valle: Senha "+PINPORTA[contador]+" foi utilizada");// Manda Mensagem para a base que a senha foi utilizada
    PINPORTA[contador]="        ";
    PIN = "";
    gravasenhasSD();
    lcd.clear();
    lcd.setCursor(4,0); lcd.print("DELACROY"); 
    lcd.setCursor(4,1); lcd.print("SMARTBOX");
    lcd.setCursor(0,2); lcd.print("APERTE UMA TECLA"); 
    lcd.setCursor(1,3); lcd.print("PARA CONTINUAR");
 }
   



//FUNÇÃO ABASTECER

 
 if(abastece == 1 && numporta == param.asInt()){
    PINPORTA[numporta] = String(key[1])+String(key[2])+String(key[3])+String(key[4])+String(random(1000,9999));
    unidade = String(key[1])+String(key[2])+String(key[3])+String(key[4]);
    lcd.clear();
    lcd.clear();lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
    lcd.setCursor(1,3); lcd.print("PORTA"); lcd.setCursor(7,3); lcd.print(param.asInt()); lcd.setCursor(9,3); lcd.print("ABERTA");
    gravasenhasSD(); 
    mensagemSMS = "Uma encomenda foi inserida na Delacroy Smartbox no Le Grande Valle. PIN: "+PINPORTA[numporta];
    enviaSMS(telefoneBase, mensagemSMS);
    recuperatelefone();
    mensagemSMS = "Prezado Condomino do Le Grand Valle, tem uma encomenda para voce na Delacroy Smartbox. Utilize o PIN: "+PINPORTA[numporta];
    enviaSMS(telefonecondomino, mensagemSMS);
    lcd.clear();lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
    lcd.setCursor(4,2);lcd.print("SMS PARA"); lcd.setCursor(1,3);lcd.print(telefonecondomino);delay(1500);
    telefonecondomino = "";

    //COLOCAR LER TECLA PARA VOLTAR AO ABASTECER();
      lcd.clear();
      lcd.setCursor(3,0); lcd.print("ABASTECER"); 
      lcd.setCursor(2,1); lcd.print("OUTRA PORTA"); 
      lcd.setCursor(1,2); lcd.print("1 - SIM");
      lcd.setCursor(1,3); lcd.print("2 - NAO");
 }

  if(abreportas == 1){
     lcd.setCursor(1,3);
     lcd.print("PORTA");  lcd.setCursor(7,3);lcd.print(param.asInt()); lcd.setCursor(9,3); lcd.print("ABERTA");
     if(param.asInt() == 9){
      abreportas = 0;
     }
  }
}

//senhazerapins = "68426451 - senhamaster = "32165498" - senhavisualiza = "86794532" - senhaabreportas = "35795155" - senhaalteracelular = "98756743"
File Senhas_Inicio;
File Angulos;
File FileTelefoneSMS;

void setup()
{
  timer.setInterval(1000L, blynkAnotherDevice);
  lcd.setBacklight(HIGH);
  lcd.begin(16,4);  lcd.clear();
  lcd.setBacklight(HIGH);
  lcd.begin(16,4);  lcd.clear();
  for(i = 0; i <= 8; i++){//zerando vetor de keys
  key[i] = " ";  
  } 

  lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(3,1); lcd.print("SULUCOES EM");
  lcd.setCursor(4,2); lcd.print("SEGURANCA"); lcd.setCursor(0,3); lcd.print("Tel:061-35518688");
  delay(1000);
  Serial.begin(9600);
  serialGSM.begin(9600); 
  configuraGSM();
  //Inicio da Rotina de Recuperação de Senhas do SD
  // dentro do arquino senha.txt deve conter uma sequencia de senhas com o formato abaixo que será chamado como a rotina abaixo por ordem
  //68426451;32165498;86794532;35795155;        ;        ;        ;        ;        ;        ;        ;        ;        */*/*/
  //observando que para zerar as senhas deve-se colocar 8 espaços "       "
  
  //////Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    lcd.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  Senhas_Inicio = SD.open("senhas.txt", FILE_READ);
  Serial.print("my file no inicio: ");//////Serial.println(Senhas_Inicio);
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  if (Senhas_Inicio) {
    // read from the file until there's nothing else in it:
    while (Senhas_Inicio.available()) {
      PIN_SD = Senhas_Inicio.read();
      CONFIGURACAO = CONFIGURACAO + String(PIN_SD);
    }
      //Serial.println("Senhas:"+CONFIGURACAO);
      senhazerapins = (CONFIGURACAO.substring(0,8));
      senhamaster = (CONFIGURACAO.substring(9,17));
      senhavisualiza = (CONFIGURACAO.substring(18,26));
      senhaabreportas = (CONFIGURACAO.substring(27,35));
      senhaalteracelular = (CONFIGURACAO.substring(36,44));
      j = 45;
      for(i = 1; i<=qtdportas; i++){//le as senhas do sd
        PINPORTA[i] = (CONFIGURACAO.substring(j,j+8));
        j+=9;
      }
      
     Senhas_Inicio.close();
     Serial.println("senhazerapins = "+senhazerapins);
     Serial.println("senhamaster = "+senhamaster);
     Serial.println("senhavisualiza = "+senhavisualiza);
     Serial.println("senhaabreportas = "+senhaabreportas);
     Serial.println("senhaalteracelular  = "+senhaalteracelular);
     for(i = 1; i<=qtdportas; i++){
       Serial.print("PINPORTA["); Serial.print(i); Serial.print("]: ") ; Serial.println(PINPORTA[i]);
     }

  }
  CONFIGURACAO = "";
  PIN_SD = ' ';

  
  randomSeed(analogRead(0)); // randomSeed() basicamente "embaralha" a função random()
  //Serial.print("teclaacionada2:");Serial.print(teclaacionada2);

  
  for (int nL = 0; nL <= 3; nL++) {
     pinMode(pinosLinhas[nL], OUTPUT);
     digitalWrite(pinosLinhas[nL], HIGH);
  }
  for (int nC = 0; nC <= 2; nC++) {
     pinMode(pinosColunas[nC], INPUT_PULLUP);
  }
  
  lcd.clear();

  Blynk.begin(auth);
  Blynk.run();
  delay(500);
  Blynk.virtualWrite(V1, "<"+PINPORTA[1]+">");
  Blynk.virtualWrite(V2, "<"+PINPORTA[2]+">");
  Blynk.virtualWrite(V3, "<"+PINPORTA[3]+">");
  Blynk.virtualWrite(V4, "<"+PINPORTA[4]+">");
  Blynk.virtualWrite(V5, "<"+PINPORTA[5]+">");
  Blynk.virtualWrite(V6, "<"+PINPORTA[6]+">");
  Blynk.virtualWrite(V7, "<"+PINPORTA[7]+">");
  Blynk.virtualWrite(V8, "<"+PINPORTA[8]+">");
  Blynk.virtualWrite(V9, "<"+PINPORTA[9]+">");
  Serial.println("OK Setup");
}

void loop()
{

  if(abastece == 1){
      Serial.println("ESTOU EM ABASTECE == 1");
      lcd.clear();
      lcd.setCursor(3,0); lcd.print("ABASTECER"); 
      lcd.setCursor(2,1); lcd.print("OUTRA PORTA"); 
      lcd.setCursor(1,2); lcd.print("1 - SIM");
      lcd.setCursor(1,3); lcd.print("2 - NAO");
      lertecla();
      Serial.println(teclaacionada2);
      if(teclaacionada2 == String(1)){
        Serial.println("DENTRO DE TECLAACIONADA2");
        Serial.println(teclaacionada2);
        teclaacionada2 = " ";
        abastecer();  
        loop(); 
      }   
      else{
        abastece = 0;
      }
  }
  lcd.clear();
  lcd.setCursor(4,0); lcd.print("DELACROY"); 
  lcd.setCursor(4,1); lcd.print("SMARTBOX");
  lcd.setCursor(0,2); lcd.print("APERTE UMA TECLA"); 
  lcd.setCursor(1,3); lcd.print("PARA CONTINUAR");
  lertecla();
  teclaacionada2 = " ";
  key[h] = " ";

  digiteopin();
  if (PIN != senhamaster && PIN != senhavisualiza && PIN != senhaabreportas && PIN != senhazerapins && PIN != senhaalteracelular && PIN != PINPORTA[1] && PIN != PINPORTA[2] && PIN != PINPORTA[3] && PIN != PINPORTA[4] 
      && PIN != PINPORTA[5] && PIN != PINPORTA[6] && PIN != PINPORTA[7] && PIN != PINPORTA[8] && PIN != PINPORTA[9])
  {
      lcd.clear();
      lcd.setCursor(2,1);
      lcd.print("PIN INVALIDO");
      lcd.setCursor(0,3);
      lcd.print("TENTE NOVAMENTE");
      delay(1000);
  }
  else{  
  
      if (PIN == senhavisualiza){
        func_pin = 0; abastece = 0; abreportas = 0;
        lcd.clear();
        lcd.setCursor(4,0); lcd.print("DELACROY"); 
        lcd.setCursor(4,1); lcd.print("SMARTBOX");
        lcd.setCursor(0,2); lcd.print("1 - TODAS"); 
        lcd.setCursor(0,3); lcd.print("2 - ESCOLHER");
        lertecla();

        if(teclaacionada2 == String(1)){
          teclaacionada2 = " ";
          for(i = 1; i<=qtdportas; i++){
          Serial.print("PINPORTA["); Serial.print(i); Serial.print("]: ") ; Serial.println(PINPORTA[i]);
          lcd.clear();
          
          lcd.setCursor(3,1); lcd.print("PIN");  lcd.setCursor(7,1); lcd.print("PORTA"); lcd.setCursor(13,1); lcd.print(i);
          lcd.setCursor(4,3);
          lcd.print(PINPORTA[i]);
          delay(1500);//CODIGO FINAL, O QUE NAO ESTA COMENTADO SERA PARA TESTES
          }
        } 
        Serial.print("TECLA ACIONADA: "); Serial.println(teclaacionada2);
        if(teclaacionada2 == String(2)){
          teclaacionada2 = " ";
          lcd.clear();
          lcd.setCursor(1,1); lcd.print("DIGITE O NUMERO"); 
          lcd.setCursor(4,2); lcd.print("DA PORTA");
          lcd.setCursor(8,3); lertecla(); lcd.print(teclaacionada2);
          delay(1000);
          for(i = 1; i<=qtdportas; i++){
            if(teclaacionada2 == String(i)){
                lcd.clear();
                lcd.setCursor(2,1); lcd.print("PIN");  lcd.setCursor(6,1); lcd.print("PORTA"); lcd.setCursor(12,1); lcd.print(i);
                lcd.setCursor(4,3);
                lcd.print(PINPORTA[i]);
                delay(3000);//CODIGO FINAL, O QUE NAO ESTA COMENTADO SERA PARA TESTES
              }  
          }

        }

       }

    
      if (PIN == senhaabreportas){
          abreportas = 1;
          Serial.println("ABRE PORTAS");
          lcd.clear(); 
          lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
          i=0;
          for(i=1; i<=qtdportas+1; i++){
              portaaberta[i]= 1; blynkAnotherDevice(); delay(2000);
              contador = i-1;
              func_pin = 0; abastece = 0;
                 
              }
    
      }
    
      if (PIN == senhamaster){ //Chave mestra para abastecer
         abastecer(); //inicia a rotina abastecer
      }
    
      if (PIN == senhazerapins){
         func_pin = 0; abastece = 0; abreportas = 0;
         zerapins();
      }

      if (PIN == senhaalteracelular){
          func_pin = 0; abastece = 0; abreportas = 0;
          lcd.clear();
          lcd.setCursor(4,0); lcd.print("SMARTBOX"); 
          lcd.setCursor(4,1); lcd.print("DELACROY");
          lcd.setCursor(0,2);
          lcd.print("DIGITE A UNIDADE");
          int pos=6;
          i=0;
          for(i=1; i<=4; i++){
              lertecla();
              delay(100);
              key[i]=teclaacionada2;
              teclaacionada2 = " ";
              lcd.setCursor(pos,3);
              lcd.print(key[i]);
              pos = pos + 1;
         
          }
          delay(500);
          andar = (key[1].toInt()*10)+ key[2].toInt();
          apto = (key[3].toInt()*10)+key[4].toInt();
          if(andar > numero_de_andares || andar < 1){
                if((andar == 11 && apto == 11) || (andar == 22 && apto == 22) || (andar == 33 && apto == 33) || (andar == 44 && apto == 44) || (andar == 55 && apto == 55)|| (andar == 66 && apto == 66)|| (andar == 77 && apto == 77)){
                  Serial.println("apartamento teste");
                  unidade_existe = true;
                }else{
                  unidade_existe = false;  
                }
          }else{
            if(apto > aptos_por_andar || apto < 1){
                if((andar == 11 && apto == 11) || (andar == 22 && apto == 22) || (andar == 33 && apto == 33) || (andar == 44 && apto == 44) || (andar == 55 && apto == 55)){
                  Serial.println("apartamento teste");
                  unidade_existe = true;
                }else{
                  unidade_existe = false;  
                }
            }
          }
          unidade = String(key[1])+String(key[2])+String(key[3])+String(key[4]);
          recuperatelefone();
          
          Serial.print("Telefone Condomino");Serial.println(telefonecondomino);
          if(unidade_existe == false){
              lcd.clear();
              lcd.setCursor(4,0); lcd.print("SMARTBOX"); 
              lcd.setCursor(4,1); lcd.print("DELACROY");
              lcd.setCursor(5,2); lcd.print("UNIDADE");
              lcd.setCursor(1,3); lcd.print("NAO ENCONTRADA");
              delay(3000);
          }
          else{
          
              lcd.clear();
              lcd.setCursor(4,0); lcd.print("SMARTBOX"); 
              lcd.setCursor(4,1); lcd.print("DELACROY");
              lcd.setCursor(1,2); lcd.print("TELEFONE ATUAL");
              lcd.setCursor(1,3); lcd.print(telefonecondomino);
              delay(3000);
              lcd.clear();
              lcd.setCursor(1,0); lcd.print("DIGITE O NUMERO"); 
              lcd.setCursor(4,1); lcd.print("EXEMPLO");
              lcd.setCursor(2,2); lcd.print("5561912345678");
              lcd.setCursor(1,3);
              for (h = 1; h<= 13; h++){
                  if(h==1){lcd.print("+");}
                  lertecla();delay(100);numero[h]=teclaacionada2;teclaacionada2 = " ";lcd.print(numero[h]);
              }
              delay(300);
              telefonecondomino = String(numero[1])+String(numero[2])+String(numero[3])+String(numero[4])+String(numero[5])+String(numero[6])+
              String(numero[7])+String(numero[8])+String(numero[9])+String(numero[10])+String(numero[11])+String(numero[12])+String(numero[13]);
              Serial.print("TEL CONDOMINO = "); Serial.println(telefonecondomino);
              int cont = 0;
              
              for(i = 7; i <= 22; i++){
                  if(i != 9 && i!= 12 && i!= 22){
                  TelAux[i] = telefonecondomino[cont];
                  Serial.print("TelAux[i]");Serial.println(TelAux[i]);
                  cont = cont + 1;
                }
              }
              Serial.print("TAM TELAUX = "); Serial.println(TelAux.length());
              Serial.println(TelAux);
                  

 
              TelAux.substring(8,9) = telefonecondomino[0]+telefonecondomino[1];
              TelAux.substring(11,12) = telefonecondomino[2]+telefonecondomino[3];

              
              lcd.clear();
              lcd.setCursor(4,0); lcd.print("CONFIRMA");
              lcd.setCursor(2,1); lcd.print(telefonecondomino);
              lcd.setCursor(1,2); lcd.print("1 - SIM");
              lcd.setCursor(1,3); lcd.print("2 - NAO");
              Serial.println("LER TECLA");
              lertecla();
              
              if(teclaacionada2 == String(1)){
                  //FUNÇÃO SALVAR TELEFONE
              


             
                  FileTelefoneSMS = SD.open(unidade+".txt", FILE_WRITE);
                  if (FileTelefoneSMS){
                      FileTelefoneSMS.print("\n");
                      FileTelefoneSMS.print(TelAux);
                      FileTelefoneSMS.close();
                     
                  }

                  
                  Serial.print("TEL AUX = "); Serial.println(TelAux);
                  lcd.clear();
                  salvando = 1;
                  recuperatelefone();
                  salvando = 0;
                  lcd.clear();
                  lcd.setCursor(1,1); lcd.print("TELEFONE SALVO"); 
                  lcd.setCursor(2,2); lcd.print("COM SUCESSO");delay(2000);
                  lcd.clear();
                  lcd.setCursor(1,1); lcd.print("TELEFONE ATUAL");
                  lcd.setCursor(1,3); lcd.print(telefonecondomino);delay(3000);
              }
          }
    }
    
      for (i=1; i<=qtdportas; i++){
          if (PIN == PINPORTA[i]){
            portaaberta[i] = 1;
            blynkAnotherDevice();
            contador = i;
            func_pin = 1;
            abastece = 0; abreportas = 0;
           }
     }
 }
}

////////////////// DIGITE O PIN //////////////////
void digiteopin()
{
  lcd.clear();
  lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX"); 
  lcd.setCursor(1,2);
  Serial.println("Digite Seu Pin");
  lcd.print("DIGITE SEU PIN");
  lcd.setCursor(4,3);
  for (h = 1; h<= 8; h++){
      lertecla();delay(100);key[h]=teclaacionada2;teclaacionada2 = " ";lcd.print(key[h]);
  }
  delay(300);
  PIN = String(key[1])+String(key[2])+String(key[3])+String(key[4])+String(key[5])+String(key[6])+String(key[7])+String(key[8]);
}

////////////////// ABASTECER //////////////////
void abastecer(){

  Serial.println("Abastecer");
  numporta = numporta + 1;
  if(numporta == 10){
  numporta = 1; 
  }
   
  if (PINPORTA[numporta] == "        "){
      lcd.clear();
      lcd.setCursor(4,0); lcd.print("SMARTBOX"); 
      lcd.setCursor(4,1); lcd.print("PORTA "); lcd.print(numporta);
      lcd.setCursor(0,2);
      lcd.print("DIGITE A UNIDADE");
      int pos=6;
      for(i=1; i<=4; i++){
          
          lertecla();
          delay(100);
          key[i]=teclaacionada2;
          teclaacionada2 = " ";
          lcd.setCursor(pos,3);
          lcd.print(key[i]);
          pos = pos + 1;
     
      }
      delay(500);
      
    if (String(key[1]) == "*" ||  String(key[2]) == "*" ||  String(key[3]) == "*" || String(key[4]) == "*"
     || String(key[1]) == "#" ||   String(key[2]) == "#" || String(key[3]) == "#" || String(key[4]) == "#"){
        
        lcd.clear();
        PIN = " ";
        lcd.setCursor(1,2);
        lcd.print("PORTA CANCELADA");
        delay(1000);
    }
    else{
      
        abastece = 1;
        lcd.clear();lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
        portaaberta[numporta] = 1; blynkAnotherDevice();
        Blynk.run();
        timer.run();
    }
 }
  else
  {

    if(PINPORTA[1] != "        " && PINPORTA[2] != "        " && PINPORTA[3] != "        " && PINPORTA[4] != "        " &&
      PINPORTA[5] != "        " && PINPORTA[6] != "        " && PINPORTA[7] != "        " && PINPORTA[8] != "        " &&
      PINPORTA[9] != "        "){
          
          lcd.clear();
          lcd.setCursor(0,1);lcd.print("TODAS AS PORTAS");
          lcd.setCursor(1,2);lcd.print("ESTAO OCUPADAS");delay(3000);
          abastece = 0;
    }
    else{
   
      lcd.clear();lcd.setCursor(4,0); lcd.print("DELACROY"); lcd.setCursor(4,1); lcd.print("SMARTBOX");
      lcd.setCursor(0,3);lcd.print("PORTA "+String(numporta)+" OCUPADA");delay(800);
      abastecer();
    }
   }
}  

////////////////// GRAVA SENHA //////////////////
void gravasenhasSD()
{
  Serial.println("Grava Senha");
  if (!SD.begin(SDCARD_CS))
  {
    lcd.clear();
    lcd.setCursor(1,1); lcd.print("SD CARD DEFEITO");
    lcd.setCursor(6,2); lcd.print("FALHA");
    lcd.setCursor(1,3); lcd.print("INICIALIZACAO");   delay(5000);
  }
  else
    {
        if (SD.exists("senhas.txt"))
        {
          SD.remove("senhas.txt");
        } 
        else 
        {
          lcd.print("PINS.txt doesn't exist.");
        }
        Senhas_Inicio = SD.open("senhas.txt", FILE_WRITE);
        if (Senhas_Inicio)
        {
          Senhas_Inicio.println(senhazerapins+";"+senhamaster+";"+senhavisualiza+";"+senhaabreportas+";"+senhaalteracelular+";"+PINPORTA[1]+";"+PINPORTA[2]+";"+PINPORTA[3]+";"+PINPORTA[4]+";"+PINPORTA[5]+";"+PINPORTA[6]+";"+PINPORTA[7]+";"+PINPORTA[8]+";"+PINPORTA[9]);
          Senhas_Inicio.close();
        }
              Blynk.virtualWrite(V10, "<"+PINPORTA[1]+">");
              Blynk.virtualWrite(V2, "<"+PINPORTA[2]+">");
              Blynk.virtualWrite(V3, "<"+PINPORTA[3]+">");
              Blynk.virtualWrite(V4, "<"+PINPORTA[4]+">");
              Blynk.virtualWrite(V5, "<"+PINPORTA[5]+">");
              Blynk.virtualWrite(V6, "<"+PINPORTA[6]+">");
              Blynk.virtualWrite(V7, "<"+PINPORTA[7]+">");
              Blynk.virtualWrite(V8, "<"+PINPORTA[8]+">");
              Blynk.virtualWrite(V9, "<"+PINPORTA[9]+">");
    }  
}

////////////////// LER TECLA //////////////////
void lertecla()
{
   while (teclaacionada2 == " ")
    {
      for (int nL = 0; nL <= 3; nL++)
    {
      digitalWrite(pinosLinhas[nL], LOW);
      for (int nC = 0; nC <= 2; nC++)
      {
        if (digitalRead(pinosColunas[nC]) == LOW)
        {
          teclaacionada1=teclas[nL][nC];
          teclaacionada2 = String(teclaacionada1);
          while(digitalRead(pinosColunas[nC]) == LOW){}
        }
      }
      digitalWrite(pinosLinhas[nL], HIGH);
    }
   delay(10);
     Blynk.run();
   }
}

////////////////// RECUPERA TELEFONE //////////////////
void recuperatelefone()
{
  FileTelefoneSMS = SD.open(unidade+".txt");
  telefonecondomino = " ";
  int cont1 = 0;
  Numtelstring = "";
  if (FileTelefoneSMS) {
    while (FileTelefoneSMS.available()) {
      Numtelchar = FileTelefoneSMS.read();
      if(Numtelchar != '\n'){
        Numtelstring = Numtelstring + String(Numtelchar);
        if (salvando == 1){
           lcd.setCursor(0,2); lcd.print("SALVANDO NUMERO");         
        }
      } 
      else{
        cont1 = 1;
        Numtelstring = "";
        cont1 = 0;
      }       
    }
    if(cont1 == 0){
      cont1 = 0;
      TelAux = Numtelstring;
    } 
  
    if(Numtelchar == '\n'){
       if(Numtelstring.length()>5){
        TelAux = Numtelstring;
        Numtelstring =" "; 
      }
      else{
        Numtelstring = "";
        Numtelchar = "";
      }
    }
      Numtelstring = ' ';
      Numtelchar = ' ';
      
      FileTelefoneSMS.close();
      telefonecondomino = "+"+TelAux.substring(7,9)+TelAux.substring(10,12)+TelAux.substring(13,22);
  }   
}

////////////////// ENVIA SMS //////////////////
void enviaSMS(String telefone, String mensagem)
{
  //Serial.println("estou na enviasms");
  //Serial.println("Telefone usado:" +telefone);
  delay(300);
  serialGSM.print("AT+CMGS=\"" + telefone + "\"\n");
  //AT+CMGS = Send SMS message 
   serialGSM.print(mensagem + "\n");
  serialGSM.print((char)26);
  //Serial.println("Enviado SMS+enviado");
  telefone = "";
  mensagem = "";
}

////////////////// CONFIGURA GSM //////////////////
void configuraGSM()
{
  Serial.println("Configura GSM");
  serialGSM.print("AT+CMGF=1\n;AT+CNMI=2,2,0,0,0\n;ATX4\n;AT+COLP=1\n");

/*
  AT+CMGF=1 = Estabelece o formato da mensagem SMS como texto;
  AT+COLP=1 = Apresentação de identificação da linha conectada 1 = OK
*/
}

////////////////// ZERA PINS //////////////////
void zerapins()
{
  Serial.println("estou na zerapins");
  lcd.clear();
  lcd.setCursor(2,2);
  lcd.print("ZERANDO PINS");
  for(i=0; i<=qtdportas; i++){
  
      PINPORTA[i]="        ";
      gravasenhasSD();
  }
}
