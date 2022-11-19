//
// Created by tommy on 18/11/22.
//
#include "ThinkGearConnector.h"

using namespace boost::asio;

namespace tgc {
    ThinkGearConnector::ThinkGearConnector(std::string port, unsigned int baud_rate) : io(), serial(io, port) {
        serial.set_option(serial_port_base::baud_rate(baud_rate));
    }

    void ThinkGearConnector::writeString(std::string s) {
        boost::asio::write(serial, buffer(s.c_str(), s.size()));
    }

    int ThinkGearConnector::readPayload() {
        unsigned char charBuffer;
        unsigned char payloadBuffer[256];

        // synchronise on 2 in a row SYNC packets
        while (true) {
            read(serial, buffer(&charBuffer, 1));
            printf("wait for sync1 got %02hhx \n",charBuffer);
            if (charBuffer != SYNCbyte) continue;
            read(serial, buffer(&charBuffer , 1));
            printf("wait for sync2 got %02hhx \n",charBuffer);
            if (charBuffer != SYNCbyte) continue; else break;
        }

        // parse PLENGTH
        read(serial, buffer(&charBuffer,1));
        if (charBuffer > 169) return 1;

        // read the number of bytes equal to PLENGTH
        read(serial, buffer(payloadBuffer, charBuffer));

        packetLength = charBuffer;
        std::copy(payloadBuffer, payloadBuffer+packetLength ,lastPayload);
        return 0;
    }




}
