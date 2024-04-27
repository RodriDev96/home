// controle da casa pagina web 


#include <Arduino.h>
#include <WiFiManager.h>   // Inclui a biblioteca WiFiManager
#include <WebServer.h>
#include <ArduinoOTA.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Configurações dos pinos
const int relayPin1 = 2;   // Pino GPIO2
const int relayPin2 = 3;   // Pino GPIO3
const int switchPin1 = 4;  // Pino GPIO4
const int switchPin2 = 5;  // Pino GPIO5

// Configurações dos LEDs
const int ledRedPin = 12;    // Pino do LED vermelho
const int ledGreenPin = 13;  // Pino do LED verde
const int ledBluePin = 14;   // Pino do LED azul

// Instância do servidor web
WebServer server(80);

// Estados dos relés
bool relay1State = LOW;
bool relay2State = LOW;
unsigned long lastSwitchChangeTime = 0; // Último tempo de mudança do switch
bool lastSwitchState1 = HIGH; // Estado anterior do switch 1
bool lastSwitchState2 = HIGH; // Estado anterior do switch 2
unsigned long debounceDelay = 50; // Tempo de debounce (em milissegundos)

// Função para alternar os relés
void toggleRelayTask(void *parameter) {
  for (;;) {
    // Lê o estado atual dos switches
    bool currentSwitchState1 = digitalRead(switchPin1);
    bool currentSwitchState2 = digitalRead(switchPin2);

    // Verifica se o switch 1 mudou de estado
    if (currentSwitchState1 != lastSwitchState1 && millis() - lastSwitchChangeTime > debounceDelay) {
      relay1State = !relay1State;
      digitalWrite(relayPin1, relay1State);
      lastSwitchChangeTime = millis();
    }

    // Verifica se o switch 2 mudou de estado
    if (currentSwitchState2 != lastSwitchState2 && millis() - lastSwitchChangeTime > debounceDelay) {
      relay2State = !relay2State;
      digitalWrite(relayPin2, relay2State);
      lastSwitchChangeTime = millis();
    }

    // Atualiza os estados anteriores dos switches
    lastSwitchState1 = currentSwitchState1;
    lastSwitchState2 = currentSwitchState2;

    vTaskDelay(pdMS_TO_TICKS(10)); // Atraso para aliviar a CPU
  }
}

// Função para manipular as requisições do cliente
void handleRoot() {
  String page = "<html>\
<head>\
    <meta name='viewport' content='width=device-width, initial-scale=1'>\
    <meta http-equiv='refresh' content='5'> <!-- Atualização automática a cada 5 segundos -->\
    <style>\
        body { \
            font-family: Arial, sans-serif; \
            margin: 0; \
            padding: 0; \
            background-color: #f4f4f4; \
        }\
        .container { \
            max-width: 600px; \
            margin: 50px auto; \
            padding: 20px; \
            background-color: #fff; \
            border-radius: 5px; \
            box-shadow: 0 0 10px rgba(0,0,0,0.1); \
        }\
        h1 { \
            text-align: center; \
        }\
        .btn { \
            display: inline-block; \
            padding: 10px 20px; \
            margin: 10px; \
            text-decoration: none; \
            color: #fff; \
            border: none; \
            border-radius: 5px; \
            cursor: pointer; \
            width: 100px; /* Largura do botão */\
        }\
        .on { \
            background-color: #28a745; /* Cor quando ligado */\
        }\
        .off { \
            background-color: #dc3545; /* Cor quando desligado */\
        }\
\
        @media only screen and (max-width: 600px) {\
            /* Estilos específicos para telas menores que 600px de largura */\
            .container {\
                max-width: 90%; /* Ajuste a largura da container para 90% da largura da tela */\
            }\
            .btn {\
                width: 80%; /* Ajuste a largura do botão para 80% da largura da tela */\
                display: block; /* Altera o botão para ocupar a largura total da tela */\
                margin: 10px auto; /* Centraliza o botão horizontalmente */\
            }\
        }\
    </style>\
</head>\
<body>\
    <div class='container'>\
        <h1>CONTROLE</h1>\
        <p>LUZ CENTRAL: <P><button id='toggle1' class='btn ";
  page += relay1State ? "on" : "off";
  page += "' onclick='toggleRelay(1)'>";
  page += relay1State ? "Desligar" : "Ligar";
  page += "</button></p>\
        <p>LUZ SPOTS: <P><button id='toggle2' class='btn ";
  page += relay2State ? "on" : "off";
  page += "' onclick='toggleRelay(2)'>";
  page += relay2State ? "Desligar" : "Ligar";
  page += "</button></p>\
    </div>\
    <script>\
        function toggleRelay(relay) {\
            fetch('/toggle' + relay)\
                .then(response => response.text())\
                .then(data => {\
                    var button = document.getElementById('toggle' + relay);\
                    button.innerText = data;\
                    button.className = 'btn ' + (data === 'Ligar' ? 'off' : 'on');\
                })\
                .catch(error => console.log('Erro:', error));\
        }\
    </script>\
</body>\
</html>";

  server.send(200, "text/html", page); // Envia a página HTML como resposta
}

// Função para manipular a mudança de estado dos relés
void handleToggleRelay(int relay) {
  if (relay == 1) {
    relay1State = !relay1State;
    digitalWrite(relayPin1, relay1State);
    server.send(200, "text/plain", relay1State ? "Desligar" : "Ligar");
  } else if (relay == 2) {
    relay2State = !relay2State;
    digitalWrite(relayPin2, relay2State);
    server.send(200, "text/plain", relay2State ? "Desligar" : "Ligar");
  } else {
    server.send(400, "text/plain", "Comando inválido");
  }
}

// Função de configuração inicial
void setup() {
  Serial.begin(115200);

  // Inicia o WiFiManager
  WiFiManager wifiManager;

  // Se precisar resetar as configurações
  // wifiManager.resetSettings();

  // Inicia o autoConnect para recuperar e salvar as credenciais
  if (!wifiManager.autoConnect("ESP32-AP")) {
    Serial.println("Falha ao conectar-se e recuperar as credenciais. Reiniciando...");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Conectado ao Wi-Fi com sucesso!");

  // Configuração dos pinos
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);

  // Configurações do servidor web
  server.on("/", handleRoot);
  server.on("/toggle1", HTTP_GET, []() { handleToggleRelay(1); });
  server.on("/toggle2", HTTP_GET, []() { handleToggleRelay(2); });
  server.begin();
  Serial.println("Servidor web iniciado");

  // Inicia a atualização OTA (Over-The-Air)
  ArduinoOTA.begin();

  // Cria a tarefa para alternar os relés com o switch
  xTaskCreate(toggleRelayTask, "toggleRelayTask", 4096, NULL, 1, NULL);

  // Configurações do LED RGB
  setColor(0, 0, 255); // Azul indicando tentativa de conexão
}

// Função de loop principal
void loop() {
  // Lida com as requisições OTA (Over-The-Air)
  ArduinoOTA.handle();

  // Lida com as requisições do servidor web
  server.handleClient();

  // Verifica se está conectado ao Wi-Fi e atualiza o LED RGB
  if (WiFi.status() == WL_CONNECTED) {
    setColor(0, 255, 0); // Verde indicando conexão estabelecida
  } else {
    setColor(255, 0, 0); // Vermelho indicando falta de conexão
  }

  // Adicione outras tarefas ou código aqui, se necessário
}

// Função para definir a cor do LED RGB
void setColor(int red, int green, int blue) {
  analogWrite(ledRedPin, red);
  analogWrite(ledGreenPin, green);
  analogWrite(ledBluePin, blue);
}
