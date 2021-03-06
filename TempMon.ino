//Code taken from the Arduino Examples library
/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
*/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

byte mac[] = {
  // MAC printed on ethernet shield
  0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xEF };
IPAddress ip(192,168,0,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

//SD Library Preamble
//0101010101010101010
//The SD Card must be selected, which is done by setting the chipselect pin mode to OUTPUT
//Even if the pin is unused, it must be set to OUTPUT or the SD library will fail

//This is the default chipselect pin for arduino boards and needs to be initialized
int defaultCSPin = 10;
//The ethernet shield chip select pin is 4
int ethernetCSPin = 4;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
 // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  
  //SD Library Setup
  //Initialize the default chipselect pin
  pinMode(defaultCSPin,OUTPUT);
  
  //Init the SD Card
  //Since we are using the ethernet shield, we will init will the ethernet CS pin
  //Using the defaultCSpin will result in failure
  if(SD.begin(ethernetCSPin)){
    Serial.println("SD Card Initialized Successfully");
  }
  else{
    Serial.println("Something went wrong when initializing the SD Card");
  }
}
void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
