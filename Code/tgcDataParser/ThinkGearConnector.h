//
// Created by tommy on 18/11/22.
//

#ifndef TGCDATAPARSER_THINKGEARCONNECTOR_H
#define TGCDATAPARSER_THINKGEARCONNECTOR_H
#include <boost/asio.hpp>
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
        unsigned char lastPayload[256];
        unsigned char packetLength;
        const unsigned char SYNCbyte= 0xAA;
        /**
         * read next packet form the device, verify it and return result
         * \return 0 if success, 1 if packet too long, 2 if checksum verification failed
         * sets lastPayload and packetLength
         */
        int readPayload();
    };
}
#endif //TGCDATAPARSER_THINKGEARCONNECTOR_H
