#include <ESP8266WiFi.h>
const char WiFiAPPSK[] = "esp8266-12e";
int inPin = 16;   // Pino do botão de pressão que é instalado na porta
int val = 0;     // variable for reading the pin status
WiFiServer server(80);

void setupWiFi()
{
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "ThingDev-":
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
                 String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "ESP-" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
}

void initHardware()
{
  Serial.begin(115200);
}

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
  pinMode(inPin, INPUT);    // declara o botão de pressão como entrada
}

void loop(){
  // Verifica se um cliente está conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Prepara a resposta
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<meta http-equiv='refresh' content='5' />\r\n";
  //s += "\r\n";
  val = digitalRead(inPin);  // lê o estado do botão
  if (val >= 0)
  {
    //s += "O banheiro esta ";
    s += (val)?"Ocupado":"Desocupado";
  }
  else
  {
    s += "Leitura inválida";
  }
  s += "</html>\n";

  // Envia a resposta
  client.print(s);
  delay(1);
  Serial.println("Cliente desconectado");

  // O cliente esrá desconectado 
  // quando a função retornar, e o objeto cliente será destruído
}
