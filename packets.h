#ifndef __PACKETS_H__
#define __PACKETS_H__

// #include <PacketSerial.h>
#include <string>
#include <vector>

class Sender {
    public:
        virtual void send(const uint8_t* buffer, size_t count);
};

class PacketHandler {
    public:
        virtual bool handles(uint8_t) = 0;
        virtual void handleMessage(uint8_t cmd, const uint8_t* data, size_t size) = 0;
};


class Packets
{
private:
    Sender* sender;
    static std::string panicMsg;
    static bool isPanic;
    static std::vector<PacketHandler*> handlers;

public:
    enum Command {
        Debug = 0,
        Status,
    };

    Packets(Sender *serial);

    void sendMessage(const char *msg);
    virtual void sendStatus() = 0;

    void maybePanic();

    static void receivePacket(const uint8_t *buffer, size_t size, Packets* packets);

    static void unpanic();
    static void panic(const char* msg);
private:
};
#endif // __PACKETS_H__