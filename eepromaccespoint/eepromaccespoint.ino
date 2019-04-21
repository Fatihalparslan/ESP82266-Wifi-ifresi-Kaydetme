
#include <ESP8266WiFi.h>
#include <EEPROM.h>
WiFiServer wifiServer(80);//wifi server 80 porttan çalışacak
int status;//wifinin bağlı olma durumunu gösterecek değişken
int i=0;//stamod fonksiyonundaki for döngüsü için kullanılacak
int j=0;//wifi bağlantısı kontrolü için oluşturulan for döngüsünde kullanılmıştır.
bool apmoddurum,stamoddurum,conn=false,degis=false;//

const char *APssid = "esp_erisim";
const char *APpassword = "espsifre";

char ssid[30] = "";
char password[30]= "";

void stamod(){
  for(i=0;i<100;i++){
  delay(100);
  }
WiFi.disconnect();
WiFi.mode(WIFI_STA);
stamoddurum=WiFi.begin(ssid,password);
if(stamoddurum!=0){
  apmoddurum=0;
}
  }

void setup() {
Serial.begin(115200);
delay(1000);
EEPROM.begin(512);
read_string(30,0).toCharArray(ssid,30); 
read_string(30,100).toCharArray(password,30); 
for(j=0;j<10;j++){
status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(100);
}
if (status==true){
  stamod();
  }else{
apmoddurum=WiFi.softAP(APssid, APpassword);
stamoddurum=false;
  }
Serial.println(apmoddurum);
Serial.println(ssid);
Serial.println(password);
//stamod();
 wifiServer.begin();
}

void loop() {
 if(apmoddurum){
 
  char wifiadi[30]="";
  char pass[30]="";
  WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()) {
        char c = client.read();
        Serial.write(c);

        
  if(c=='#'){
    degis=true;
    i=0;
    }else{
  if(degis==false){
    wifiadi[i]=c;
    i++;
    }else{
      pass[i]=c;
      i++;
    }
    }
    }
    }
    degis=false;
    i=0;
    
    client.stop();
  write_to_Memory(String(wifiadi),String(pass));
  
stamod();
  }
  
  }
  
}
void write_to_Memory(String s, String p) {
  s += ";";
  write_EEPROM(s, 0);
  p += ";";
  write_EEPROM(p, 100);
  EEPROM.commit();
}
//write to memory
void write_EEPROM(String x, int pos) {
  for (int n = pos; n < x.length() + pos; n++) {
    EEPROM.write(n, x[n - pos]);
  }
}
//Reads a string out of memory
String read_string(int l, int p){
  String temp;
  for (int n = p; n < l+p; ++n)
    {
     if(char(EEPROM.read(n))!=';'){
        if(isWhitespace(char(EEPROM.read(n)))){
          //do nothing
        }else temp += String(char(EEPROM.read(n)));
      
     }else n=l+p;
     
    }
  return temp;
}

