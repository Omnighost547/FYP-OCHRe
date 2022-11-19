//
// Created by tommy on 18/11/22.
//
#include "ThinkGearConnector.h"
#include "ThinkGearPacket.h"

using namespace boost::asio;
using namespace std;

namespace tgc {
    ThinkGearConnector::ThinkGearConnector(std::string port, unsigned int baud_rate) : io(), serial(io, port) {
        serial.set_option(serial_port_base::baud_rate(baud_rate));
        bytesParsed = 0;

    }

    int ThinkGearConnector::readPayload() {
        byte charBuffer;
        int localPacketLength;
        int checksum;
        vector<byte> payloadBuffer;

        // synchronise on 2 in a row SYNC packets
        while (true) {
            read(serial, buffer(&charBuffer, 1));
            if (charBuffer != SYNC) continue;
            read(serial, buffer(&charBuffer , 1));
            if (charBuffer != SYNC) continue; else break;
        }

        // parse PLENGTH
        read(serial, buffer(&charBuffer,1));
        if (to_integer<unsigned char>(charBuffer) > 169) return 1;
        localPacketLength = to_integer<int>(charBuffer);

        // read the number of bytes equal to PLENGTH
        payloadBuffer.clear();
        payloadBuffer.resize(localPacketLength);
        read(serial, buffer(payloadBuffer));

        //calculate and verify checksum
        checksum = 0;
        for(auto i=0; i < localPacketLength; i++ ) checksum += to_integer<char>(payloadBuffer[i]);
        checksum &= 0xFF;
        checksum = ~checksum & 0xFF;

        read(serial, buffer(&charBuffer, 1));
        if (to_integer<int>(charBuffer) != checksum) return 2;


        lastPayload = payloadBuffer;
        return 0;
    }

    int ThinkGearConnector::getLatestPacket(ThinkGearPacket* packet) {
        byte code;
        
        if (packet == nullptr) return -1;
        // all data points in the payload were parsed, fetch more data
        if (bytesParsed >= lastPayload.size()){
            int retval = readPayload();
            if (retval > 0) return retval;
            bytesParsed = 0;
        }

        code = lastPayload[bytesParsed++];
        switch (code) {
            case POOR_SIGNAL_BYTE:
                packet->type = POOR_SIGNAL;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
            case HEART_RATE_BYTE:
                packet->type = HEART_RATE;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
            case ATTENTION_BYTE:
                packet->type = ATTENTION;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
            case MEDITATION_BYTE:
                packet->type = MEDITATION;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);

            default:
                printf("Unknown data code 0x%02hhx\n", code);
                // discard the next value since we don't know what to do with it anyway
                bytesParsed++;
        }
        return 0;
    }
}
