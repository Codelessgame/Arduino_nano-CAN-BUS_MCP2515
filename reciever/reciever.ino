#include <mcp_can.h>
#include <SPI.h>
#include <Servo.h>

#define CAN_CS 12       // CS pin for MCP2515
#define CAN_INT 2       // INTerrupt pin 

MCP_CAN CAN(CAN_CS);    // Create CAN object on CS pin
Servo servos[12];       // Array of 12 Servo objects
const int servoPins[12] = {3, 4, 5, 6, 7, 8, 9, A0, A1, A2, A3, A4};


unsigned long myCanId = 0x01;  // Unique ID for this Arduino (you can change this per unit)
unsigned char Flag_Recv = 0;
unsigned char len = 0;
unsigned char buf[8];
unsigned long recvId;

bool servoState[12] = {false};

void MCP2515_ISR()
{
     Flag_Recv = 1;
}


void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 12; i++) {
    servos[i].attach(servoPins[i]);
  }

  // Initialize CAN bus at 500 kbps
  START_INIT:
    if(CAN_OK == CAN.begin(MCP_ANY, CAN_500KBPS,MCP_8MHZ))     
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        delay(100);
        goto START_INIT;
    }
    pinMode(CAN_INT, INPUT);
    attachInterrupt(digitalPinToInterrupt(CAN_INT), MCP2515_ISR, FALLING); 
}


void loop()
{
   if(Flag_Recv)
    {
      Flag_Recv = 0;
      CAN.readMsgBuf(&recvId, &len, buf);
      
      // Only process message if intended for this node
      if (recvId == myCanId) {
        int servoIndex = buf[0];
        if (servoIndex < 12) {
          servoState[servoIndex] = !servoState[servoIndex];
          int angle = servoState[servoIndex] ? 180 : 0;
          servos[servoIndex].write(angle);
        }
    }
  }
}
