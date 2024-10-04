#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h> // Inclui a biblioteca para controle de LCD_I2C

// Configurações - variáveis editáveis
const char* default_SSID = "SUA_INTERNET"; // Nome da rede Wi-Fi 
const char* default_PASSWORD = "SENHA_DA_SUA_INTERNET"; // Senha da rede Wi-Fi 
const char* default_BROKER_MQTT = "IP_PÚBLICO"; // IP do Broker MQTT 
const int default_BROKER_PORT = 1883; // Porta do Broker MQTT
const char* default_TOPICO_SUBSCRIBE = "/TEF/EnvM001/cmd"; // Tópico MQTT de escuta
const char* default_TOPICO_PUBLISH_1 = "/TEF/EnvM001/attrs"; // Tópico para envio de estado
const char* default_TOPICO_PUBLISH_2 = "/TEF/EnvM001/attrs/l"; // Tópico para luminosidade
const char* default_TOPICO_TEMP = "/TEF/EnvM001/attrs/t"; // Tópico para temperatura
const char* default_TOPICO_UMID = "/TEF/EnvM001/attrs/h"; // Tópico para umidade
const char* default_ID_MQTT = "fiware_001"; // ID do cliente MQTT
const int default_D4 = 2; // Pino do LED onboard

// Configuração do LCD_I2C
#define LCD_ADDRESS 0x27 // Endereço I2C do LCD
#define LCD_COLUMNS 16 // Número de colunas do LCD
#define LCD_ROWS 2 // Número de linhas do LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS); // Inicializa o LCD I2C

// Configurações do sensor DHT
#define DHTPIN 4 // Pino onde o DHT está conectado
#define DHTTYPE DHT22 // Tipo do sensor DHT (DHT22)
DHT dht(DHTPIN, DHTTYPE); // Cria uma instância do sensor DHT

WiFiClient espClient; // Cliente Wi-Fi
PubSubClient MQTT(espClient); // Cliente MQTT
char EstadoSaida = '0'; // Estado da saída (LED)

void initSerial() {
    Serial.begin(115200); // Inicializa a comunicação serial a 115200 bps
}

void initWiFi() {
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(default_SSID);
    Serial.println("Aguarde");
    reconectWiFi(); // Chama a função para conectar ao Wi-Fi
}

void initMQTT() {
    MQTT.setServer(default_BROKER_MQTT, default_BROKER_PORT); // Configura o broker MQTT
    MQTT.setCallback(mqtt_callback); // Define a função de callback para mensagens recebidas
}

void setup() {
    InitOutput(); // Inicializa a saída (LED)
    initSerial(); // Inicializa a comunicação serial
    initWiFi(); // Conecta ao Wi-Fi
    initMQTT(); // Inicializa a conexão MQTT
    dht.begin(); // Inicializa o sensor DHT
    lcd.init(); // Inicia o LCD I2C
    lcd.backlight(); // Liga a luz de fundo do LCD
    delay(5000);
    MQTT.publish(default_TOPICO_PUBLISH_1, "s|on"); // Publica que o sistema está ligado
}

void loop() {
    VerificaConexoesWiFIEMQTT(); // Verifica as conexões Wi-Fi e MQTT
    EnviaEstadoOutputMQTT(); // Envia o estado da saída via MQTT
    handleLuminosity(); // Lida com a leitura da luminosidade
    handleTemperatureAndHumidity(); // Lida com temperatura e umidade
    MQTT.loop(); // Mantém a conexão MQTT ativa
}

void reconectWiFi() {
    if (WiFi.status() == WL_CONNECTED)
        return; // Se já estiver conectado, não faz nada
    WiFi.begin(default_SSID, default_PASSWORD); // Tenta conectar ao Wi-Fi
    while (WiFi.status() != WL_CONNECTED) { // Aguarda conexão
        delay(100);
        Serial.print("."); // Exibe ponto enquanto aguarda
    }
    Serial.println();
    Serial.println("Conectado com sucesso na rede ");
    Serial.print(default_SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP()); // Exibe o IP obtido
    digitalWrite(default_D4, LOW); // Garante que o LED inicie desligado
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    String msg; // String para armazenar a mensagem recebida
    for (int i = 0; i < length; i++) {
        char c = (char)payload[i]; // Converte o payload para char
        msg += c; // Adiciona o char à string da mensagem
    }
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg); // Exibe a mensagem recebida

    // Controla o LED com base na mensagem recebida
    if (msg.equals(String("EnvM001@on|"))) {
        digitalWrite(default_D4, HIGH); // Liga o LED
        EstadoSaida = '1'; // Atualiza o estado
    } else if (msg.equals(String("EnvM001@off|"))) {
        digitalWrite(default_D4, LOW); // Desliga o LED
        EstadoSaida = '0'; // Atualiza o estado
    }
}

void VerificaConexoesWiFIEMQTT() {
    if (!MQTT.connected())
        reconnectMQTT(); // Reconecta se não estiver conectado ao MQTT
    reconectWiFi(); // Verifica a conexão Wi-Fi
}

void EnviaEstadoOutputMQTT() {
    if (EstadoSaida == '1') {
        MQTT.publish(default_TOPICO_PUBLISH_1, "s|on"); // Publica que o LED está ligado
        Serial.println("- Led Ligado");
    } else {
        MQTT.publish(default_TOPICO_PUBLISH_1, "s|off"); // Publica que o LED está desligado
        Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!"); // Confirma o envio do estado
    delay(1000); // Espera 1 segundo antes de repetir
}

void InitOutput() {
    pinMode(default_D4, OUTPUT); // Configura o pino do LED como saída
    digitalWrite(default_D4, HIGH); // Inicia com o LED desligado
}

void reconnectMQTT() {
    while (!MQTT.connected()) { // Tenta reconectar enquanto não estiver conectado
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(default_BROKER_MQTT);
        if (MQTT.connect(default_ID_MQTT)) {
            Serial.println("Conectado com sucesso ao broker MQTT!"); // Confirma a conexão
            MQTT.subscribe(default_TOPICO_SUBSCRIBE); // Inscreve-se no tópico de comando
        } else {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Haverá nova tentativa de conexão em 2s");
            delay(2000); // Espera 2 segundos antes da próxima tentativa
        }
    }
}

void handleLuminosity() {
    const int potPin = 34; // Pino do sensor de luminosidade
    int sensorValue = analogRead(potPin); // Lê o valor do sensor
    int luminosity = map(sensorValue, 0, 4095, 0, 100); // Mapeia o valor para 0-100
    String mensagem = String(luminosity); // Converte o valor para string
    Serial.print("Valor da luminosidade: ");
    Serial.println(mensagem.c_str()); // Exibe o valor da luminosidade
    MQTT.publish(default_TOPICO_PUBLISH_2, mensagem.c_str()); // Publica o valor da luminosidade
}

void handleTemperatureAndHumidity() {
    float humidity = dht.readHumidity(); // Lê a umidade
    float temperature = dht.readTemperature(); // Lê a temperatura em Celsius

    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Falha na leitura do DHT!"); // Verifica se a leitura falhou
        return; // Sai da função se houve falha
    }

    // Exibe os dados no LCD
    lcd.clear(); // Limpa o LCD
    lcd.setCursor(0, 0); // Define o cursor na primeira linha
    lcd.print("Temp: " + String(temperature) + "C"); // Mostra a temperatura
    lcd.setCursor(0, 1); // Define o cursor na segunda linha
    lcd.print("Hum: " + String(humidity) + "%"); // Mostra a umidade

    // Envia os dados para os tópicos correspondentes
    MQTT.publish(default_TOPICO_UMID, String(humidity).c_str()); // Publica a umidade
    MQTT.publish(default_TOPICO_TEMP, String(temperature).c_str()); // Publica a temperatura

    Serial.print("Umidade: ");
    Serial.print(humidity);
    Serial.print("%, Temperatura: ");
    Serial.print(temperature);
    Serial.println("°C"); // Exibe os dados no console serial
}
