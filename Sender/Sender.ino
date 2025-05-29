#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS 12       // CS pin for MCP2515
#define CAN_INT 2       // INTerrupt pin 

MCP_CAN CAN(CAN_CS);    // Create CAN object on CS pin

unsigned char buf[8];
const uint32_t nodeIds[] = {0x01, 0x02, 0x03, 0x04, 0x05}; // static list of IDS
const int numNodes = sizeof(nodeIds) / sizeof(nodeIds[0]);


void setup()
{
  Serial.begin(9600);
  START_INIT:

    if(CAN_OK == CAN.begin(MCP_ANY, CAN_500KBPS,MCP_8MHZ))     
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}

bool isValidNode(uint32_t id) {
  for (int i = 0; i < numNodes; i++) {
    if (nodeIds[i] == id) return true;
  }
  return false;
}

void loop()
{
   while (true) {
      Serial.println("Enter target CAN ID (e.g., 1 for 0x01):");
      int targetId = Serial.parseInt();
      if (isValidNode((uint32_t)targetId)) break;
      Serial.println("Invalid ID, try again.");
      delay(500);

    Serial.println("Enter servo index (0-11):");
    int servoIndex = Serial.parseInt();

    if (servoIndex >= 0 && servoIndex < 12) {
      buf[0] = (byte)servoIndex;
      CAN.sendMsgBuf( targetId, 0, 1, buf);
      Serial.print("Command sent to toggle servo ");
      Serial.println(servoIndex);
      }
    }
  }