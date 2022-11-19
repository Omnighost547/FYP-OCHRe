//
// Created by tommy on 19/11/22.
//

#ifndef TGCDATAPARSER_THINKGEARPACKET_H
#define TGCDATAPARSER_THINKGEARPACKET_H


namespace tgc {
    enum packetType {TEST, POOR_SIGNAL, HEART_RATE, ATTENTION, MEDITATION,};

    class ThinkGearPacket {
    public:
        friend class ThinkGearConnector;
        packetType getPacketType();

    private:
        packetType type;
    };

} // tgc

#endif //TGCDATAPARSER_THINKGEARPACKET_H
