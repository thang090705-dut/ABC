#include "ITF_airway.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
#include <limits>

using namespace std;

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    Login log;
    if (!log.performLogin()) {
        return 0;
    }

    string cmd;
    SystemManager sys;

    while (true) {
        cout << "Nhập 'Open' để chạy chương trình hoặc 'Close' để đóng chương trình: ";
        cin >> cmd;
        if (cmd == "Open" || cmd == "open") {
            sys.openFlight();
            sys.bookingProcess();
            sys.closeFlight();
        } else if (cmd == "Close" || cmd == "close") {
            break;
        }
    }
    return 0;
}