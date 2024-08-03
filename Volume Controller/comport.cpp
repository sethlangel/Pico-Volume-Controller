#include "Comport.h"
#include <iostream>
#include "Strings.h"
#include "json.hpp"
#include "volume.h"

Comport::Comport() : hComm(INVALID_HANDLE_VALUE) {}

void Comport::initComport(const std::string& comportName) {
    std::wstring wComPort = stringToWString("\\\\.\\") + stringToWString(comportName);

    hComm = CreateFile(
        wComPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening COM port: "<< comportName << std::endl;
    }
    else {
        std::cout << "COM port opened successfully" << std::endl;
    }
}

bool Comport::isInitialized() const {
    return hComm != INVALID_HANDLE_VALUE;
}

void Comport::configureComport() {
    if (hComm == INVALID_HANDLE_VALUE) {
        std::cerr << "COM port is not initialized" << std::endl;
        return;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        std::cerr << "Error getting device state: " << GetLastError() << std::endl;
        return;
    }

    // Set baud rate, byte size, stop bits, and parity
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    // Set the new state
    if (!SetCommState(hComm, &dcbSerialParams)) {
        std::cerr << "Error setting device state: " << GetLastError() << std::endl;
        return;
    }

    // Set timeouts
    setCommTimeouts();
}

void Comport::setCommTimeouts() {
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = 50; // Max time between characters
    timeouts.ReadTotalTimeoutConstant = 1000; // Total timeout for read
    timeouts.ReadTotalTimeoutMultiplier = 10; // Multiplier for read timeout
    timeouts.WriteTotalTimeoutConstant = 1000; // Total timeout for write
    timeouts.WriteTotalTimeoutMultiplier = 10; // Multiplier for write timeout

    if (!SetCommTimeouts(hComm, &timeouts)) {
        std::cerr << "Error setting timeouts: " << GetLastError() << std::endl;
    }
}

std::string Comport::readFile() {
    char buffer[256];
    DWORD bytesRead;

    if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';

            return std::string(buffer);
        }
    }
    else {
        DWORD dwError = GetLastError();
        std::cerr << "Error reading from COM port: " << dwError << std::endl;
    }
}

void Comport::closeHComm() {
    CloseHandle(hComm);
}
