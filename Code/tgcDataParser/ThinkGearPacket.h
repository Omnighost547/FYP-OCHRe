//
// Created by tommy on 19/11/22.
//

#ifndef TGCDATAPARSER_THINKGEARPACKET_H
#define TGCDATAPARSER_THINKGEARPACKET_H


namespace tgc {
    enum packetType {
        POOR_SIGNAL, HEART_RATE, ATTENTION, MEDITATION, RAW_8BIT_WAVE, RAW_MARKER, // single bytes
        RAW_WAVE, EEG_POWER, ASIC_EEG_POWER,RRINTERVAL
    };

    class ThinkGearPacket {
    public:
        friend class ThinkGearConnector;
        packetType getPacketType();
        int getValue() const;
    private:
        packetType type;
        int value;
    };

} // tgc

#endif //TGCDATAPARSER_THINKGEARPACKET_H
