#include <iostream>
#include "ThinkGearConnector.h"


using namespace std;
using namespace boost;
using namespace tgc;

int main(int argc, char *argv[]) {
    try {

        ThinkGearConnector serial("/dev/rfcomm0", 115200);

        for (int i = 0; i < 10; ++i) {

            cout << "read payload returned: " << serial.readPayload() << endl;
            cout << "len" << (int) serial.lastPayload.size() << endl;

            for (auto &byts : serial.lastPayload) {
                printf("%02hhx ", byts);
            }
            cout << endl;
            cout << "==========================" << endl;
        }
    } catch (boost::system::system_error &e) {
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}

