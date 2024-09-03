#include <string.h>
#include <string>

#include "packets.h"

Packets::Packets(Sender *serial)
    : sender(serial) {
        unpanic();
}

void Packets::maybePanic()
{
  if (this->isPanic)
  {
    this->sendMessage(this->panicMsg.c_str());
  }
}

void Packets::sendMessage(const char *msg)
{
  uint8_t buffer[256];
  size_t len = strlen(msg);
  if (len == 0)
  {
    return;
  }
  if (len > 254)
  {
    len = 255;
  }
  buffer[0] = Debug;
  memcpy(buffer + 1, msg, len + 1);
  this->sender->send(buffer, len + 2);
}

uint8_t *writeLong(uint8_t *addr, long val)
{
  memcpy(addr, &val, sizeof(long));
  return addr + sizeof(long);
}

uint8_t *writeInt(uint8_t *addr, int val)
{
  memcpy(addr, &val, sizeof(int));
  return addr + sizeof(int);
}

uint8_t *writeBool(uint8_t *addr, bool val)
{
  *addr = val ? 1 : 0;
  return addr + 1;
}

uint8_t *writeByte(uint8_t *addr, uint8_t val)
{
  *addr = val;
  return addr + 1;
}

void Packets::receivePacket(const uint8_t *buffer, size_t size, Packets *packets)
{
  if (size == 0)
  {
    packets->panic("Size zero message!");
    return;
  }
  switch (buffer[0])
  {
  case Packets::Status:
    packets->sendStatus();
    break;
  default:
    for (int i = 0; i < handlers.size(); i++) {
        if (handlers[i]->handles(buffer[0])) {
            handlers[i]->handleMessage(buffer[0], buffer, size);
            break;
        }
    }
    packets->panic("Unknown message!");
    break;
  }
}

void Packets::unpanic()
{
    Packets::isPanic = false;
}

void Packets::panic(const char* msg)
{
    Packets::isPanic = true;
    Packets::panicMsg = msg;
}