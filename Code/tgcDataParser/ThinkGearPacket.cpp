//
// Created by tommy on 19/11/22.
//

#include "ThinkGearPacket.h"

namespace tgc {
    packetType ThinkGearPacket::getPacketType() {
        return type;
    }

    int ThinkGearPacket::getValue() const {
        return value;
    }
} // tgc
