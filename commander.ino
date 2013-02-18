#include <CmdMessenger.h>
#include <Base64.h>
#include <Streaming.h>

char field_separator = ',';
char command_separator = ';';

CmdMessenger cmdMessenger = CmdMessenger(Serial, field_separator, command_separator);

// Commands we send from the Arduino to be received on the PC
enum
{
  kCOMM_ERROR    = 000, // Lets Arduino report serial port comm error back to the PC (only works for some comm errors)
  kACK           = 001, // Arduino acknowledges cmd was received
  kARDUINO_READY = 002, // After opening the comm port, send this cmd 02 from PC to check arduino is ready
  kERR           = 003, // Arduino reports badly formatted cmd, or cmd not recognised
  kRES           = 004, // Arduino acknowledges cmd was receive  
  kSEND_CMDS_END, // Mustnt delete this line
};

// Commands we send from the PC and want to recieve on the Arduino.
// We must define a callback function in our Arduino program for each entry in the list below vv.
// They start at the address kSEND_CMDS_END defined ^^ above as 004
messengerCallbackFunction messengerCallbacks[] = 
{
  pin_mode,
  d_write,
  a_write,
  d_read,
  a_read,
  NULL
};
// Its also possible (above ^^) to implement some symetric commands, when both the Arduino and
// PC / host are using each other's same command numbers. However we recommend only to do this if you
// really have the exact same messages going in both directions. Then specify the integers (with '=')


// ------------------ C A L L B A C K  M E T H O D S -------------------------

void readParams(int &pin, int &value)
{
  pin   = cmdMessenger.readInt();
  value = cmdMessenger.readInt();
}

void d_write()
{
  int pin   = cmdMessenger.readInt();
  int value = cmdMessenger.readInt();

  digitalWrite(pin, value);
}

void a_write()
{
  int pin   = cmdMessenger.readInt();
  int value = cmdMessenger.readInt();
  
  analogWrite(pin, value);
}

void d_read()
{
  int  pin  = cmdMessenger.readInt();
  Serial.println(digitalRead(pin));
}

void a_read()
{
  int pin   = cmdMessenger.readInt();
  Serial.println(analogRead(pin));
}

void pin_mode()
{
  int pin   = cmdMessenger.readInt();
  int value = cmdMessenger.readInt();
  
  pinMode(pin, value);
}


// ------------------ D E F A U L T  C A L L B A C K S -----------------------

void ready()
{
  // In response to ping. We just send a throw-away Acknowledgement to say "im alive"
  cmdMessenger.sendCmd(kACK,"ready...");
}

void unknownCmd()
{
  // Default response for unknown commands and corrupt messages
  cmdMessenger.sendCmd(kERR,"Unknown command");
}

// ------------------ E N D  C A L L B A C K  M E T H O D S ------------------



// ------------------ S E T U P ----------------------------------------------

void attach_callbacks(messengerCallbackFunction* callbacks)
{
  // Attach default / generic callback methods
  cmdMessenger.attach(unknownCmd);
  
  int i = 0;
  int offset = kSEND_CMDS_END;
  while(callbacks[i])
  {
    cmdMessenger.attach(offset+i, callbacks[i]);
    i++;
  }
}

void setup() 
{
  Serial.begin(57600);  // Arduino Duemilanove, FTDI Serial
//Serial.begin(115200); // Arduino Uno, Mega, with AT8u2 USB

  // cmdMessenger.discard_LF_CR(); // Useful if your terminal appends CR/LF, and you wish to remove them
  cmdMessenger.print_LF_CR();   // Make output more readable whilst debugging in Arduino Serial Monitor
  attach_callbacks(messengerCallbacks);

  ready();
  
}


// ------------------ M A I N ( ) --------------------------------------------

// Timeout handling
long timeoutInterval = 2000; // 2 seconds
long previousMillis = 0;
int counter = 0;

void loop() 
{
  // Process incoming serial data, if any
  cmdMessenger.feedinSerialData();
}


