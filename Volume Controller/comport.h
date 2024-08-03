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
    void initComport(const std::string& comportName);
    bool isInitialized() const;
    void configureComport();
    void setCommTimeouts();
    std::string readFile();
    void closeHComm();
};
