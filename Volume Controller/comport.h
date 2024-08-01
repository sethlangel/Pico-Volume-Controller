#pragma once

#include <string>
#include <windows.h>

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
};
