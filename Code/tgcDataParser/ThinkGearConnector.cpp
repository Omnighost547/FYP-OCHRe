//
// Created by tommy on 18/11/22.
//
#include "ThinkGearConnector.h"

using namespace boost::asio;

namespace tgc {
    ThinkGearConnector::ThinkGearConnector(std::string port, unsigned int baud_rate) : io(), serial(io, port) {
        serial.set_option(serial_port_base::baud_rate(baud_rate));
    }

    int ThinkGearConnector::readPayload() {
        unsigned char charBuffer;
        unsigned char tmpPacketLen;
        unsigned char checksum;
        unsigned char payloadBuffer[256];

        // synchronise on 2 in a row SYNC packets
        while (true) {
            read(serial, buffer(&charBuffer, 1));
            if (charBuffer != SYNCbyte) continue;
            read(serial, buffer(&charBuffer , 1));
            if (charBuffer != SYNCbyte) continue; else break;
        }

        // parse PLENGTH
        read(serial, buffer(&charBuffer,1));
        if (charBuffer > 169) return 1;
        tmpPacketLen = charBuffer;

        // read the number of bytes equal to PLENGTH
        read(serial, buffer(payloadBuffer, charBuffer));

        //calculate and verify checksum
        checksum = 0;
        for(auto i=0; i<tmpPacketLen; i++ ) checksum += payloadBuffer[i];
        checksum &= 0xFF;
        checksum = ~checksum & 0xFF;

        read(serial, buffer(&charBuffer, 1));
        if (charBuffer != checksum) return 2;


        // all is good, set payload, packetLength class variables and return
        packetLength = tmpPacketLen;
        std::copy(payloadBuffer, payloadBuffer+packetLength ,lastPayload);
        return 0;
    }

}
