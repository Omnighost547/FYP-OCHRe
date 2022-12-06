#include <iostream>
#include "ThinkGearConnector.h"


using namespace std;
using namespace boost;
using namespace tgc;

int main(int argc, char *argv[]) {
    try {
        ThinkGearConnector serial("C:\\tools\\thedata");

        for (int i = 0; i < 10; ++i) {
            ThinkGearPacket p{};
            if (int retval = serial.nextDataPoint(&p); retval != 0 ){
                cout << "packet parse failed with " << retval << endl;
                continue;
            }
            cout << "packetType: " << p.getPacketType() << " value: " << p.getValue() << endl;
        }
    } catch (boost::system::system_error &e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

