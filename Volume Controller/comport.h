#pragma once

#include <string>
#include <windows.h>
#include "volume.h"

class Comport {
private:
    std::string comportName;
    HANDLE hComm;

public:
    Comport();
    HANDLE initComport(const std::string& comportName);
    bool isInitialized() const;
    void configureComport();
    void setCommTimeouts();
    void readFile(HANDLE hComm, Volume& volume, nlohmann::json& settings, Comport& comport);
};
