#include <windows.h>
#include "strings.h"
#include <iostream>
#include <cstdlib>

std::wstring stringToWString(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void splitData(std::string data, int& potId, float& fVolume) {
    std::string strBuffer(data);
    size_t pos = strBuffer.find('_');

    if (pos != std::string::npos) {
        potId = stoi(strBuffer.substr(0, pos));

        size_t volumeStart = pos + 1;
        if (volumeStart < strBuffer.length()) {
            std::string sVolume = strBuffer.substr(volumeStart);
            fVolume = std::stof(sVolume) / 100.0f;
        }
    }
}
