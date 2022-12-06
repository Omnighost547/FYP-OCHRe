//
// Created by tommy on 18/11/22.
//

#define BOOST_ASIO_HAS_FILE  // enable boost file support
#define BOOST_ASIO_HAS_IO_URING // io_uring on linux for async file access


#ifndef TGCDATAPARSER_THINKGEARCONNECTOR_H
#define TGCDATAPARSER_THINKGEARCONNECTOR_H
#include <boost/asio.hpp>
#include <fstream>
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
         * service device
         */
         // todo fix the serial constructor
//        ThinkGearConnector(std::string port, unsigned int baud_rate);
        explicit ThinkGearConnector(const std::string& filePath);

//    private:
        boost::asio::io_context context;

//        boost::asio::serial_port service;
        // todo find a type that can store both stream_file and serial_port
        boost::asio::stream_file service;
        // todo investigate replacing with std::deque
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
        int nextDataPoint(ThinkGearPacket* packet);
        int readPayload();
    };
}
#endif //TGCDATAPARSER_THINKGEARCONNECTOR_H
