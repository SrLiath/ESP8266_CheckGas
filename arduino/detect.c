#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "NOME_DA_REDE";
const char* password = "SENHA_DA_REDE";
const char* serverName = "URL_DO_SERVIDOR";

int gasPin = 4;    // Pino do sensor de gás
int buzzerPin = 5; // Pino do buzzer

void setup() {
  pinMode(gasPin, INPUT);       // Define o pino do sensor de gás como entrada
  pinMode(buzzerPin, OUTPUT);   // Define o pino do buzzer como saída

  WiFi.begin(ssid, password);   // Conecta ao Wi-Fi usando as credenciais fornecidas
  Serial.begin(115200);         // Inicia a comunicação serial para depuração

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");  // Aguarda a conexão com o Wi-Fi
  }

  Serial.println("Conectado ao WiFi");
}

void loop() {
  int gasValue = digitalRead(gasPin); // Lê o valor do sensor de gás

  if (gasValue == HIGH) {
    digitalWrite(buzzerPin, HIGH); // Ativa o buzzer
    sendAlert(); // Envia alerta para o celular
  } else {
    digitalWrite(buzzerPin, LOW); // Desativa o buzzer
  }

  delay(1000);
}

void sendAlert() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverName);                           // Configura a URL do servidor
    http.addHeader("Content-Type", "application/json");  // Define o tipo de conteúdo como JSON

    int httpResponseCode = http.POST("{\"message\":\"Gás detectado!\"}");  // Envia uma mensagem para o servidor

    if (httpResponseCode > 0) {
      Serial.print("Alerta enviado com sucesso, código de resposta: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Falha ao enviar alerta, código de resposta: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Encerra a conexão HTTP
  } else {
    Serial.println("Erro na conexão WiFi");
  }
}
