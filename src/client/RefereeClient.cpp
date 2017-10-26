#include <chrono>

#define SSL_BROADCAST_PORT "10003"
#define SSL_BROADCAST_ADDR "224.5.23.1"

#include <iostream>
#include "RefereeClient.h"
#include "client_config.h"

using namespace Utils::Timing;


namespace RhobanSSL
{

    RefereeClient::RefereeClient() :
    MulticastClient(SSL_REFEREE_ADDRESS, SSL_REFEREE_PORT)
    {
    }

    SSL_Referee RefereeClient::getData()
    {
        SSL_Referee tmp;

        mutex.lock();
        tmp = data;
        mutex.unlock();

        return tmp;
    }

    bool RefereeClient::process(char *buffer, size_t len)
    {
        SSL_Referee packet;

        if (packet.ParseFromArray(buffer, len)) {
            data = packet;
            return true;
        }

        return false;
    }
}