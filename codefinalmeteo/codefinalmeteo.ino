#include <SPI.h>
#include <Client.h>
#include <Ethernet.h>
#include <Server.h>
#include <Wire.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(80);
IPAddress ip(192,168,0,11);
EthernetClient client;

float donneetemp=0;
int donneelumi=0;
int donneepress=0;
int donneehumi=0;
int donneealti=0;

boolean valuesHaveChanged = false;
 
/////////////////////////////////////////MERDIE/////////////////////////////////////////////////
#define BMP085_ADDRESS 0x77
int photocellPin = A0;
int photocellReading; 
const byte DHT11_PIN = 5;

enum {
  DHT11_SUCCESS, //!< Pas d'erreur
  DHT11_TIMEOUT_ERROR, //!< Temps d'attente dépassé
  DHT11_CHECKSUM_ERROR //!< Données reçues erronées
};

char bmp085ReadChar(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available());

  return Wire.read();
}

int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);

  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

const unsigned char OSS = 0;

int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;

long b5;

short temperature;
long pressure;

void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////FONCTIONS//////////////////////////////////// 
// Lit la température non compensée
unsigned int bmp085ReadUT()
{
  unsigned int ut;

  // Ecrit 0x2E dans le registre 0xF4.
  // Cela demande une lecture de température.
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Attendons au moins 4.5ms
  delay(5);

  // Lit les 2 octets des registres 0xF6 et 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Lit la pression non compensée
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Ecrit 0x34 + (OSS<<6) dans le registre 0xF4 pour
  // demander une lecture de la pression avec le paramètre d'oversampling
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Attend la fin de conversion, le délai dépend de OSS
  delay(2 + (3 << OSS));

  // Lit la réponse dans les registres :
  // 0xF6 (MSB), 0xF7 (LSB), et 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);

  // Attend aue les données soient dispo (3 bytes)
  while(Wire.available() < 3);

  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

// Calcule la température à partir de ut.
// La valeur de sortie est exprimée en dixièmes de degrés.
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);
}

// Calcule la pression à partir de up.
// Les valeurs de calibration doivent être initialisées.
// b5 est aussi requise, donc bmp085GetTemperature(...) doit être appelée en premier.
// La valeur de sortie est exprimée en Pascals.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calcule B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calcule B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  return p;
}

//Retourne l'altitude théorique
//en fonction de la pression atmosphérique
float getAltitude(long pressure)
{
  // Pression au niveau de la mer (Pa)
  const float p0 = 101325;
  float altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));
  return altitude;
}

byte readDHT11(byte pin, byte* humidity) {
 
  /* data[] -> buffer contenant les données du cpateur
   * counter -> compteur permettant de savoir quel bit est reçu (bitwise)
   * index -> compteur permettant de savoir quel octet est reçu (bitwise)
   * timeout -> compteur pour le timeout
   */
  byte data[5] = { 0 }, counter = 7, index = 0;
  unsigned int timeout;
 
  /* Conversion du numéro de broche Arduino en ports/masque binaire "bas niveau" */
  /* Utiliser les registres du microcontrôleur est bien plus rapide que digitalWrite() */
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *ddr = portModeRegister(port); // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port); // Registre IN (lecture)
 
  /* Réveil du capteur */
  *ddr |= bit; // OUTPUT
  *out &= ~bit; // LOW
  delay(18); // Temps d'attente à LOW causant le réveil du capteur
  *out |= bit; // HIGH
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  /* Attente de la réponse du capteur */
  timeout = 0;
  while(!(*in & bit)) /* Attente d'un état LOW */
    if (++timeout == 10000)
      return DHT11_TIMEOUT_ERROR;
  timeout = 0;
  while(*in & bit) /* Attente d'un état HIGH */
    if (++timeout == 10000)
      return DHT11_TIMEOUT_ERROR;
 
  /* Lecture des données du capteur (40 bits) */
  for (byte i = 0; i < 40; ++i) {
 
    /* Attente d'un état LOW */
    timeout = 0;
    while(!(*in & bit))
      if (++timeout == 10000)
        return DHT11_TIMEOUT_ERROR;
 
    /* Mise en mémoire du temps courant */
    unsigned long t = micros();
 
    /* Attente d'un état HIGH */
    timeout = 0;
    while(*in & bit)
      if (++timeout == 10000)
        return DHT11_TIMEOUT_ERROR;
 
    /* Si le delta Temps est supérieur à 40µS c'est un "1", sinon c'est un "0" */
    if ((micros() - t) > 40)
      data[index] |= (1 << counter); // "1"
    // Le tableau data[] est initialisé à "0" par défaut <span class="wp-smiley emoji emoji-wink" title=";)">;)</span>
 
    /* Si le compteur de bits atteint zéro */
    if (counter-- == 0) {
      counter = 7; /* On passe à l'octet suivant */
      ++index;
    }
  }
 
  /* Format des données :
   * [0] = humidité en %
   * [1] = zéro
   * [2] = température en degrés Celsius
   * [3] = zéro
   * [4] = checksum (humidité + température)
   */
  *humidity = data[0]; 
 
  /* Vérifie la checksum */
  if (data[4] != (data[0] + data[2]))
    return DHT11_CHECKSUM_ERROR; /* Erreur de checksum */
  else
    return DHT11_SUCCESS; /* Pas d'erreur */
}
///////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  
  bmp085Calibration();
  Serial.begin(9600);

  Ethernet.begin(mac, ip);
 
  Serial.println(Ethernet.localIP()); 
  Wire.begin();
}
void loop() 
{
  
  ///////////////////////////////////////////////////////////////////////////
    //////////////////////BMP085///////////////////////
  temperature = bmp085GetTemperature(bmp085ReadUT());
  pressure = bmp085GetPressure(bmp085ReadUP());
  float altitude = getAltitude(pressure);

  delay(500);
  
  ////////////////////// lumiere////////////////////////
  
  photocellReading = analogRead(A1);
 
/////////////////////////DHT11////////////////////////////////////

  byte  humidity;
 
  ///////////////////////////////////////////////////////////////////////////
  
  ////////////////////////////////////////////////////////////////////////////
  int humidite;
   switch (readDHT11(DHT11_PIN, &humidity)) {
  case DHT11_SUCCESS: 
     humidite = humidity;
    break;
  }
  ////////////////////////////////////////////////////////////////////////////
  EthernetClient client = server.available();
 
    float datas[6];
    datas[0] = 0;
    datas[1] =(float)temperature/10,1;
    datas[2] = photocellReading;
    datas[3] =pressure/100;
    datas[4] = humidite;
    datas[5] = altitude;
    
    donneetemp = datas[1];
    donneelumi = datas[2];
    donneepress = datas[3];
    donneehumi = datas[4];
    donneealti = datas[5];
    
    valuesHaveChanged = true;

  if (client) 
  {
    Serial.println("Nouveau client");
    boolean currentLineIsBlank = true;
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) 
        {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/xml");
          client.println("Connection: close");  
          client.println();
          client.println("<?xml version=\"1.0\"?>");
          
          client.println("<sensors>");
         
            client.print("<sensor name=\"sns");
            client.print(1);//Id du capteur
            client.println("\">");
            
            client.print("<temperature>");
           if(valuesHaveChanged)
              client.println(donneetemp);
            client.println("</temperature>");

            client.print("<lumiere>");
            if(valuesHaveChanged)
              client.print(donneelumi);
            client.println("</lumiere>");
             
             client.print("<pression>");
           if(valuesHaveChanged)
              client.println(donneepress);
            client.println("</pression>");
            
            client.print("<humi>");
           if(valuesHaveChanged)
              client.println(donneehumi);
            client.println("</humi>");
            
             
            client.print("<alti>");
           if(valuesHaveChanged)
              client.println(donneealti);
            client.println("</alti>");
            
            client.println("</sensor>");   
        
          client.print("</sensors>");
          break;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("client deconnecte");
  }
}
