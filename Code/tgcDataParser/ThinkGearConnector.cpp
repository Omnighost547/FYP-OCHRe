//
// Created by tommy on 18/11/22.
//
#include "ThinkGearConnector.h"
#include "ThinkGearPacket.h"

using namespace boost::asio;

namespace tgc {
    ThinkGearConnector::ThinkGearConnector(std::string port, unsigned int baud_rate) : io(), serial(io, port) {
        serial.set_option(serial_port_base::baud_rate(baud_rate));
        packetLength = 0;
        bytesParsed = 0;
    }

    int ThinkGearConnector::readPayload() {
        unsigned char charBuffer;
        unsigned char localPacketLength;
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
        localPacketLength = charBuffer;

        // read the number of bytes equal to PLENGTH
        read(serial, buffer(payloadBuffer, charBuffer));

        //calculate and verify checksum
        checksum = 0;
        for(auto i=0; i < localPacketLength; i++ ) checksum += payloadBuffer[i];
        checksum &= 0xFF;
        checksum = ~checksum & 0xFF;

        read(serial, buffer(&charBuffer, 1));
        if (charBuffer != checksum) return 2;


        // all is good, set payload, localPacketLength class variables and return
        this->packetLength = localPacketLength;
        std::copy(payloadBuffer, payloadBuffer + localPacketLength , this->lastPayload);
        return 0;
    }

    int ThinkGearConnector::getLatestPacket(ThinkGearPacket* packet) {
        // all data points in the payload were parsed, fetch more data
        if (bytesParsed >= packetLength){
            int retval = readPayload();
            if (retval > 0) return retval;
            bytesParsed = 0;
        }

        switch () {
            
        }

    }
}
