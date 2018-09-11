#include <IRrecv.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>

uint16_t RECV_PIN = D10;

IRrecv irrecv(RECV_PIN);

decode_results results;

const char* ssid = "Your WiFi SSID";
const char* password = "Your WiFi Password";
char auth[] = "your blink auth token";
 
int LED1 = D0; 
int LED2 = D1; 
int LED3 = D2; 
int LED4 = D3; 
int LED5 = D4; 
int LED6 = D5; 
int LED7 = D6; 
int LED8 = D7; 
  int swval[] = {0,0,0,0,0,0,0,0,0};
  int errpin = D9;

WiFiServer server(80);

BLYNK_WRITE(V1)
{
  int pd1 = param.asInt(); 
  appchng(LED1,pd1,1);
}
BLYNK_WRITE(V2)
{
  int pd2 = param.asInt(); 
  appchng(LED2,pd2,2);
}
BLYNK_WRITE(V3)
{
  int pd3 = param.asInt(); 
  appchng(LED3,pd3,3);
}
BLYNK_WRITE(V4)
{
  int pd4 = param.asInt(); 
  appchng(LED4,pd4,4);
}
BLYNK_WRITE(V5)
{
  int pd5 = param.asInt(); 
  appchng(LED5,pd5,5);
}
BLYNK_WRITE(V6)
{
  int pd6 = param.asInt(); 
  appchng(LED6,pd6,6);
}
BLYNK_WRITE(V7)
{
  int pd7 = param.asInt(); 
  appchng(LED7,pd7,7);
}
BLYNK_WRITE(V8)
{
  int pd8 = param.asInt(); 
  appchng(LED8,pd8,8);
}

void appchng(int k, int q, int m){
  digitalWrite(k, q);
  swval[m]=q;
  EEPROM.write(m, q);
  EEPROM.commit();
}

void setup() {
  //Serial.begin(9600);
  irrecv.enableIRIn();  // Start the receiver
  delay(10);
  
  //eeprom
  EEPROM.begin(512);

  swval[1]=EEPROM.read(1);
  swval[2]=EEPROM.read(2);
  swval[3]=EEPROM.read(3);
  swval[4]=EEPROM.read(4);
  swval[5]=EEPROM.read(5);
  swval[6]=EEPROM.read(6);
  swval[7]=EEPROM.read(7);
  swval[8]=EEPROM.read(8);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
   pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(LED7, OUTPUT);
  pinMode(LED8, OUTPUT);
  pinMode(errpin, OUTPUT);
  digitalWrite(LED1, swval[1]);
  digitalWrite(LED2, swval[2]);
  digitalWrite(LED3, swval[3]);
  digitalWrite(LED4, swval[4]);
  digitalWrite(LED5, swval[5]);
  digitalWrite(LED6, swval[6]);
  digitalWrite(LED7, swval[7]);
  digitalWrite(LED8, swval[8]);
  digitalWrite(errpin, HIGH);

  // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 delay(5000);
  server.begin();

  Blynk.config(auth); 
  Blynk.connect(); 
}

BLYNK_CONNECTED() {
  Blynk.virtualWrite(V1, swval[1]);
  delay(100);
  Blynk.virtualWrite(V2, swval[2]);
  delay(100);
  Blynk.virtualWrite(V3, swval[3]);
  delay(100);
  Blynk.virtualWrite(V4, swval[4]);
  delay(100);
  Blynk.virtualWrite(V5, swval[5]);
  delay(100);
  Blynk.virtualWrite(V6, swval[6]);
  delay(100);
  Blynk.virtualWrite(V7, swval[7]);
  delay(100);
  Blynk.virtualWrite(V8, swval[8]);
  delay(100);
}
void relayswitch(int i,int j, int v){
  if(swval[j]==1){
    digitalWrite(i, LOW);
    swval[j]=0;
    EEPROM.write(j, 0);
  }else{
    digitalWrite(i, HIGH);
    swval[j]=1;
    EEPROM.write(j, 1);
  }
  EEPROM.commit();
  Blynk.virtualWrite(v, swval[j]);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    digitalWrite(errpin,HIGH);
  }else{
    digitalWrite(errpin,LOW);
    Blynk.run();
  }


  //ir receiveing code
  
  //CHANGE VALUE ACCORDINGLY
  
  if (irrecv.decode(&results)) {
   unsigned int ircode = results.value;
        //Serial.println(ircode);
        if(ircode==1086290565){
          relayswitch(LED1,1,V1);
        }else if(ircode==1086306885){
          relayswitch(LED2,2,V2);
        }else if(ircode==1086290055){
          relayswitch(LED3,3,V3);
        }else if(ircode==1086280365){
          relayswitch(LED4,4,V4);
        }else if(ircode==1086296685){
          relayswitch(LED5,5,V5);
        }else if(ircode==1086279855){
          relayswitch(LED6,6,V6);
        }else if(ircode==1086282405){
          relayswitch(LED7,7,V7);
        }else if(ircode==1086298725){
          relayswitch(LED8,8,V8);
        }
    irrecv.resume();  // Receive the next value
  }
  delay(100);

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
  return;
  }
 
  // Wait until the client sends some data
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  client.flush();
          
// Set LED according to the request
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
  client.println("<title>Amits Room</title>");
  client.println("<br><br>");
 
 // Return the response LED1
  client.print("Relay 1 is now: ");
  // Control LED 1
  if (request.indexOf("/LED1=ON") != -1)  {
    digitalWrite(LED1, LOW);
    swval[1] = 0;
    EEPROM.write(1, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED1=OFF") != -1)  {
    digitalWrite(LED1, HIGH);
    swval[1] = 1;
    EEPROM.write(1, 1);
    EEPROM.commit();
     }
     if(swval[1]==0){client.print("ON");}
     if(swval[1]==1){client.print("OFF");}
    
  client.println("<br><br>");
  client.println("<a href=\"/LED1=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED1=OFF\"\"><button>TURN OFF </button></a><br />");  
  

 // Return the response LED2
 client.println("<br><br>");
 client.print("Relay 2 is now: ");
  // Control LED 2
  if (request.indexOf("/LED2=ON") != -1)  {
    digitalWrite(LED2, LOW);
    swval[2] = 0;
    EEPROM.write(2, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED2=OFF") != -1)  {
    digitalWrite(LED2, HIGH);
    swval[2] = 1;
    EEPROM.write(2,1);
    EEPROM.commit();
    }
    if(swval[2]==0){client.print("ON");}
     if(swval[2]==1){client.print("OFF");}
   
  client.println("<br><br>");
  client.println("<a href=\"/LED2=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED2=OFF\"\"><button>TURN OFF </button></a><br />");  
 
  // Return the response LED3
  client.println("<br><br>");
  client.print("Relay 3 is now: ");
  // Control LED 3
  if (request.indexOf("/LED3=ON") != -1)  {
    digitalWrite(LED3, LOW);
    swval[3] = 0;
    EEPROM.write(3, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED3=OFF") != -1)  {
    digitalWrite(LED3, HIGH);
    swval[3] = 1;
    EEPROM.write(3, 1);
    EEPROM.commit();
    }
    if(swval[3]==0){client.print("ON");}
     if(swval[3]==1){client.print("OFF");}
    
  client.println("<br><br>");
  client.println("<a href=\"/LED3=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED3=OFF\"\"><button>TURN OFF </button></a><br />");  
 
  // Return the response LED4
  client.println("<br><br>");
  client.print("Relay 4 is now: ");
  // Control LED 4
  if (request.indexOf("/LED4=ON") != -1)  {
    digitalWrite(LED4, LOW);
    swval[4] = 0;
    EEPROM.write(4, 0);
    EEPROM.commit();
    } 
  if (request.indexOf("/LED4=OFF") != -1)  {
    digitalWrite(LED4, HIGH);
    swval[4] = 1;
    EEPROM.write(4, 1);
    EEPROM.commit();
    }
    if(swval[4]==0){client.print("ON");}
     if(swval[4]==1){client.print("OFF");}
     
  client.println("<br><br>");
  client.println("<a href=\"/LED4=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED4=OFF\"\"><button>TURN OFF </button></a><br />");  
  
  // Return the response LED5
  client.println("<br><br>");
  client.print("Relay 5 is now: ");
  // Control LED 5
  if (request.indexOf("/LED5=ON") != -1)  {
    digitalWrite(LED5, LOW);
    swval[5] = 0;
    EEPROM.write(5, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED5=OFF") != -1)  {
    digitalWrite(LED5, HIGH);
    swval[5] = 1;
    EEPROM.write(5, 1);
    EEPROM.commit();
     }
     if(swval[5]==0){client.print("ON");}
     if(swval[5]==1){client.print("OFF");}
    
  client.println("<br><br>");
  client.println("<a href=\"/LED5=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED5=OFF\"\"><button>TURN OFF </button></a><br />");  
  

 // Return the response LED6
 client.println("<br><br>");
 client.print("Relay 6 is now: ");
  // Control LED 6
  if (request.indexOf("/LED6=ON") != -1)  {
    digitalWrite(LED6, LOW);
    swval[6] = 0;
    EEPROM.write(6, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED6=OFF") != -1)  {
    digitalWrite(LED6, HIGH);
    swval[6] = 1;
    EEPROM.write(6, 1);
    EEPROM.commit();
    }
    if(swval[6]==0){client.print("ON");}
     if(swval[6]==1){client.print("OFF");}
   
  client.println("<br><br>");
  client.println("<a href=\"/LED6=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED6=OFF\"\"><button>TURN OFF </button></a><br />");  
 
  // Return the response LED7
  client.println("<br><br>");
  client.print("Relay 7 is now: ");
  // Control LED 7
  if (request.indexOf("/LED7=ON") != -1)  {
    digitalWrite(LED7, LOW);
    swval[7] = 0;
    EEPROM.write(7, 0);
    EEPROM.commit();
    }
  if (request.indexOf("/LED7=OFF") != -1)  {
    digitalWrite(LED7, HIGH);
    swval[7] = 1;
    EEPROM.write(7, 1);
    EEPROM.commit();
    }
    if(swval[7]==0){client.print("ON");}
     if(swval[7]==1){client.print("OFF");}
    
  client.println("<br><br>");
  client.println("<a href=\"/LED7=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED7=OFF\"\"><button>TURN OFF </button></a><br />");  
 
  // Return the response LED8
  client.println("<br><br>");
  client.print("Relay 8 is now: ");
  // Control LED 8
  if (request.indexOf("/LED8=ON") != -1)  {
    digitalWrite(LED8, LOW);
    swval[8] = 0;
    EEPROM.write(8, 0);
    EEPROM.commit();
    } 
  if (request.indexOf("/LED8=OFF") != -1)  {
    digitalWrite(LED8, HIGH);
    swval[8] = 1;
    EEPROM.write(8, 1);
    EEPROM.commit(); 
    }
    if(swval[8]==0){client.print("ON");}
     if(swval[8]==1){client.print("OFF");}
     
  client.println("<br><br>");
  client.println("<a href=\"/LED8=ON\"\"><button>TURN ON </button></a>");
  client.println("<a href=\"/LED8=OFF\"\"><button>TURN OFF </button></a><br />");  
  client.println("</html>");

  delay(1);
}
