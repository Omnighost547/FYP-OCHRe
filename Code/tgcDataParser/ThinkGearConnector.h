//
// Created by tommy on 18/11/22.
//

#ifndef TGCDATAPARSER_THINKGEARCONNECTOR_H
#define TGCDATAPARSER_THINKGEARCONNECTOR_H
#include <boost/asio.hpp>
#include "ThinkGearPacket.h"
using namespace std;
namespace tgc {
    class ThinkGearConnector {
    public:
        /**
         * Constructor.
         * \param port device name, example "/dev/ttyUSB0" or "COM4"
         * \param baud_rate communication speed, example 9600 or 115200
         * \throws boost::system::system_error if cannot open the
         * serial device
         */
        ThinkGearConnector(std::string port, unsigned int baud_rate);

//    private:
        boost::asio::io_service io;
        boost::asio::serial_port serial;
        vector<byte> lastPayload;
        unsigned int bytesParsed;
        static const byte SYNC{0xAA};

        // Data Codes
        // Single Bytes
        static const byte POOR_SIGNAL_BYTE{0x02};
        static const byte HEART_RATE_BYTE{0x03};
        static const byte ATTENTION_BYTE{0x04};
        static const byte MEDITATION_BYTE {0x05};
        static const byte RAW_8BIT_WAVE_BYTE{0x06};
        static const byte RAM_MARKER_BYTE{0x07};
        // Multi bytes
        static const byte RAW_WAVE_BYTE{0x80};
        static const byte EEG_POWER_BYTE{0x81};
        static const byte ASIC_EEG_POWER_BYTE{0x83};
        static const byte RRINTERVAL_BYTE{0x86};

        /**
         * read next packet form the device, verify it and return result
         * \param: packet pointer to ThinkGearPacket to write to
         * \returns:
         * 0 - success,
         * -1 - *packet is null
         * 1 - packet too long,
         * 2 - checksum verification failed
         *
         * sets lastPayload and packetLength
         */
        int getLatestPacket(ThinkGearPacket* packet);
        int readPayload();
    };
}
#endif //TGCDATAPARSER_THINKGEARCONNECTOR_H
