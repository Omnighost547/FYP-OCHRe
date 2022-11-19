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
        // https://developer.neurosky.com/docs/doku.php?id=thinkgear_communications_protocol#code_definitions_table
        switch (code) {
            case POOR_SIGNAL_BYTE:
                packet->type = POOR_SIGNAL;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            case HEART_RATE_BYTE:
                packet->type = HEART_RATE;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            case ATTENTION_BYTE:
                packet->type = ATTENTION;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            case MEDITATION_BYTE:
                packet->type = MEDITATION;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            case RAW_8BIT_WAVE_BYTE:
                packet->type = RAW_8BIT_WAVE;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            case RAM_MARKER_BYTE:
                packet->type = RAW_MARKER;
                packet->value = to_integer<int>(lastPayload[bytesParsed++]);
                break;
            // multi byte stuff
            case RAW_WAVE_BYTE:
                packet->type = RAW_WAVE;
                // TODO IMPLEMENT
                bytesParsed+=2;
                break;
            case EEG_POWER_BYTE:
                packet->type = EEG_POWER;
                // TODO IMPLEMENT
                bytesParsed+=32;
                break;
            case ASIC_EEG_POWER_BYTE:
                packet->type = ASIC_EEG_POWER;
                bytesParsed+=24;
                // TODO IMPLEMENT
                break;
            case RRINTERVAL_BYTE:
                packet->type = RRINTERVAL;
                // TODO IMPLEMENT
                bytesParsed+=2;
                break;

            default:
                printf("Unknown data code 0x%02hhx\n", code);
                // discard the next value since we don't know what to do with it anyway
                bytesParsed++;
        }
        return 0;
    }
}
