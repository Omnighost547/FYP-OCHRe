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

        /**
         * Write a string to the serial device.
         * \param s string to write
         * \throws boost::system::system_error on failure
         */
        void writeString(std::string s);

        int readPayload();


//    private:
        boost::asio::io_service io;
        boost::asio::serial_port serial;
        unsigned char lastPayload[256];
        unsigned char packetLength;
        const unsigned char SYNCbyte= 0xAA;
    };
}
#endif //TGCDATAPARSER_THINKGEARCONNECTOR_H
