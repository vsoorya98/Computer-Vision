//
//Created by SHIRISH & SOORYA on 4/23/23.
//This program file sends commands to the arduino pins i.e) to turn on/off relay that is connected to the arduino
//

//Setting the digital pin number 12 as the output pin
const int ledPin = 12;

void setup()
{
    //Setting the pin mode to output
  pinMode(ledPin, OUTPUT);
  //Setting the baud rate to 9600
  Serial.begin(9600);
}

void loop()
{
    //Setting the pin to high
  digitalWrite(ledPin, HIGH);
  if (Serial.available() > 0) {
    char command = Serial.read();
    //If the command is H then the pin is set to low and the delay is set to 2 seconds
    if (command == 'H') 
    {
        digitalWrite(ledPin, LOW);
        delay(2000);
    }
    //If the command is L then the pin is set to high
    else if (command == 'L') 
    {
        digitalWrite(ledPin, HIGH);
    }
 }
}


