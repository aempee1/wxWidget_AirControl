#include <string>
#include <boost/asio.hpp>
#include <vector>
#include "comport_setting.hpp"
#include "serial_utils.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <dirent.h>
#include <regex>
#endif


void ComportSettingsDialog::InitSerial(const string& port ){
    io_service io;
    serial_port serial = init_serial_port(io, port);
}



std::vector<std::string> ComportSettingsDialog::FetchAvailablePorts() {
    std::vector<std::string> ports;

#ifdef _WIN32
    char portName[10];
    for (int i = 1; i <= 256; ++i) {
        snprintf(portName, sizeof(portName), "COM%d", i);
        HANDLE hPort = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hPort != INVALID_HANDLE_VALUE) {
            ports.push_back(portName);
            CloseHandle(hPort);
        }
    }
#endif

#ifdef __APPLE__
    DIR *dir = opendir("/dev");
    if (dir) {
        struct dirent *entry;
        std::regex serialRegex("^tty\\..*"); // พอร์ต serial บน macOS มักจะขึ้นต้นด้วย "cu."
        while ((entry = readdir(dir)) != NULL) {
            if (std::regex_match(entry->d_name, serialRegex)) {
                ports.push_back("/dev/" + std::string(entry->d_name));
            }
        }
        closedir(dir);
    }
#endif

    return ports;
}