#include <Keypad.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
String SenValue;
String VerSenValue;
String NovSenValue;
const byte numRows = 4;
const byte numCols = 4;
String DigiSenha;
String Senha = "1234";
String PrimeiraSenha = "1234";
const int buzzerPin = 23;
int endereco = 0;
const int solenoidePin = 22;
String SenhaStatus = "";
const char* ssid = "iPodc_2.4";
const char* password = "etnethome";
char keymap[numRows][numCols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
void acionarBuzzerErro(){
    for (int i = 0; i < 2; ++i){
        tone(buzzerPin, 1000, 200);
        delay(300);
    }
}

void acionarBuzzer(){
    tone(buzzerPin, 1000, 200);
}
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    
    <title>Cofre inteligente</title>
   <meta charset="UTF-8">
        <meta name="viewport"content="width=device-width,initial-scale=1.0">
        <script src='https://kit.fontawesome.com/a076d05399.js' crossorigin='anonymous'></script>
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/css/bootstrap.min.css">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js">
  
        </script>
  
      <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.6/umd/popper.min.js">
  
      </script>
  
      <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0-beta.2/js/bootstrap.min.js">
  
      </script>
    <style>
      body{
        text-align: center;
      }
      #AC{
        border-radius: 25em;
        background-color: green;
        color: white;
        font-weight: bold;
        border: 2px solid gray;
        font-size: 15px;
        filter: drop-shadow(2px 2px 2px gray);
      }
      #MS{
        border-radius: 25em;
        background-color: green;
        color: white;
        font-weight: bold;
        border: 2px solid gray;
        font-size: 15px;
        filter: drop-shadow(2px 2px 2px gray);
      }
      #ES{
        border-radius: 25em;
        background-color: green;
        color: white;
        font-weight: bold;
        border: 2px solid gray;
        font-size: 15px;
        filter: drop-shadow(2px 2px 2px gray);
      }
      #SenhaCofre{
        border: 2px solid gray;
        border-radius: 25em;
        filter: drop-shadow(2px 2px 2px gray);
        width: 250px;
      }
      #Cofre{
        width: 285px;
        height: 375px;
        background-color: greenyellow;
        border: 5px solid gray;
        margin-left: auto;
        margin-right: auto;
        border-radius: 5%;
      }
      #MudaSenhaCofre{
        border: 2px solid gray;
        border-radius: 25em;
        filter: drop-shadow(2px 2px 2px gray);
        width: 250px;
      }
      #VerificaSenhaCofre{
        border: 2px solid gray;
        border-radius: 25em;
        filter: drop-shadow(2px 2px 2px gray);
        width: 250px;
      }
      #ASE{
        color: white;
        font-weight: bold;
        filter: drop-shadow(1px 1px 1px black);
      }
      #StatusSenha{
        color: white;
        font-weight: bold;
        filter: drop-shadow(1px 1px 1px black);
      }
    </style>
  </head>
  <body>

<br>
<div class="container" id="Cofre">
  <br>
        <input id="SenhaCofre" type="password" maxlength="4" placeholder="SENHA DO COFRE" onkeyup="DigitaSenha()"><br><br>
   
        <button id="AC" disabled onclick="EnviarSenha()">ABRIR COFRE</button><br><br>
        <input id="VerificaSenhaCofre" type="password" maxlength="4" placeholder="DIGITE A SENHA ANTIGA" onkeyup="DigitaVerifiSenha()" ><br><br>
   
        <button id="ES" disabled onclick="VerificarSenha()">ENVIAR SENHA</button><br><br>
        <label id="ASE">A SENHA ESTÁ</label> <label id="StatusSenha"></label>
        <input id="MudaSenhaCofre" type="password" maxlength="4" placeholder="DIGITE A NOVA SENHA" onkeyup="DigitaNovaSenha()" ><br><br>
   
        <button id="MS" disabled onclick="MudarSenha()">MUDAR SENHA</button>
</div>
     
    

  <script>

    document.getElementById("ES").style.opacity = 0.5;
    function VerificarSenha(){
      var VerSenhaValue = document.getElementById("VerificaSenhaCofre").value;
      $.ajaxSetup({timeout:1000});
      $.get("/VerSenha?value="+VerSenhaValue);
      {Connection: close};
      
    }
    function DigitaVerifiSenha(){
      if((document.getElementById("VerificaSenhaCofre").value.length == 4)){
      document.getElementById("ES").disabled = false;
    }else{
      document.getElementById("ES").disabled = true;
    }
    if(document.getElementById("ES").disabled == true){
      document.getElementById("ES").style.opacity = 0.5;
    }else{
      document.getElementById("ES").style.opacity = 1;
    }
    }
    document.getElementById("AC").style.opacity = 0.5;
    function DigitaSenha(){
      
    if((document.getElementById("SenhaCofre").value.length == 4)){
      document.getElementById("AC").disabled = false;
    }else{
      document.getElementById("AC").disabled = true;
    }
    if(document.getElementById("AC").disabled == true){
      document.getElementById("AC").style.opacity = 0.5;
    }else{
      document.getElementById("AC").style.opacity = 1;
    }
    }


    document.getElementById("MS").style.opacity = 0.5;
    function DigitaNovaSenha(){
      
      if((document.getElementById("MudaSenhaCofre").value.length == 4)&&(document.getElementById("StatusSenha").innerHTML == "CORRETA")){
        document.getElementById("MS").disabled = false;
      }else{
        document.getElementById("MS").disabled = true;
      }
      if(document.getElementById("MS").disabled == true){
        document.getElementById("MS").style.opacity = 0.5;
      }else{
        document.getElementById("MS").style.opacity = 1;
      }
      }
    
    
    function EnviarSenha(){
      var SenhaValue = document.getElementById("SenhaCofre").value;
      $.ajaxSetup({timeout:1000});
      $.get("/Senha?value="+SenhaValue);
      {Connection: close};
    }
    function MudarSenha(){
      var SenhaValueNova = document.getElementById("MudaSenhaCofre").value;
      $.ajaxSetup({timeout:1000});
      $.get("/MudaSenha?value="+SenhaValueNova);
      {Connection: close};
    }
    var recebStatus;
    setInterval(function ( ) {
            const reqSS = new XMLHttpRequest();
            reqSS.onreadystatechange = function(){
            if(this.readyState == 4 && this.status == 200){
              recebStatus = reqSS.responseText;
                document.getElementById("StatusSenha").innerHTML = recebStatus;
              
              
            }
            }
            reqSS.open('GET','/SenhaStatus',true);
            reqSS.send();
            }, 600 ) ;
    </script>

</body>
</html>
)rawliteral";


AsyncWebServer server(80);
AsyncEventSource events("/events");
byte colPins[numCols] = {16, 4, 2, 15};
byte rowPins[numRows] = {19, 18, 5, 17};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  ledcSetup(0, 5000, 8);
  ledcAttachPin(buzzerPin, 0);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  

 
  EEPROM.begin(64);
  if (EEPROM.readString(0) == "") {
    EEPROM.writeString(0, PrimeiraSenha);
    EEPROM.commit();
  }

  // Lê a senha da EEPROM
  Senha = EEPROM.readString(0);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
    events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    
    client->send("hello!", NULL, millis(), 10000);
  });
  server.on("/Senha", HTTP_GET, [] (AsyncWebServerRequest *request) { 
         
   if (request->hasParam("value")) {

      SenValue = request->getParam("value")->value();
      
      
      Serial.println(SenValue);  
      if ((SenValue == Senha)) {
        Serial.println("ACESSO PERMITIDO");
        DigiSenha = "";
        acionarBuzzer();
        pinMode(solenoidePin, OUTPUT);
        delay(6000);
        pinMode(solenoidePin, INPUT);
      } else {
        Serial.println("ACESSO NEGADO");
        DigiSenha = "";
        acionarBuzzerErro();
        delay(2000);
      }
    }
    
  });
   server.on("/VerSenha", HTTP_GET, [] (AsyncWebServerRequest *request) { 
         
   if (request->hasParam("value")) {

      VerSenValue = request->getParam("value")->value();
      
      
      Serial.println(VerSenValue);  
      if ((VerSenValue == Senha)) {
        SenhaStatus = "C";
      } else {
        SenhaStatus = "I";
      }
    }
    
  });
   server.on("/MudaSenha", HTTP_GET, [] (AsyncWebServerRequest *request) { 
         
   if (request->hasParam("value")) {

      NovSenValue = request->getParam("value")->value();
      
      
      Serial.println(NovSenValue);  
  
      Senha = NovSenValue;

      EEPROM.writeString(0, Senha);
      EEPROM.commit();
      
      Serial.println(Senha);
    }
    
  });
  server.on("/SenhaStatus", HTTP_GET, [](AsyncWebServerRequest *request){
    if(SenhaStatus == "C"){
      request->send_P(200, "text/plain", String("CORRETA").c_str());
    }
    if(SenhaStatus == "I"){
      request->send_P(200, "text/plain", String("INCORRETA").c_str());
    }
    
  });
  server.addHandler(&events);
  
  server.begin();
}



void loop() {
  char keypressed = myKeypad.getKey();
  //Serial.println(Senha);
  if (keypressed != NO_KEY) {
    DigiSenha += keypressed;
    Serial.println(DigiSenha);
    if (DigiSenha.length() > 5) {
      DigiSenha = "";
    }

    if (keypressed == 'D') {
      int conf = DigiSenha.toInt();
      Serial.println(conf);

      if (conf == Senha.toInt()) {
        Serial.println("ACESSO PERMITIDO");
        DigiSenha = "";
        acionarBuzzer();
         pinMode(solenoidePin, OUTPUT);
        delay(6000);
         pinMode(solenoidePin, INPUT);
      } else {
        Serial.println("ACESSO NEGADO");
        DigiSenha = "";
        acionarBuzzerErro();
        delay(2000);
      }
    }

    if (keypressed == '*') {
      Senha = DigiSenha;
      int Analisa = Senha.toInt();
      EEPROM.writeString(0, String(Analisa));
      EEPROM.commit();
      
      Serial.println(Senha);
      DigiSenha = "";
    }

  }
}
