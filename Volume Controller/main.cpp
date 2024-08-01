#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Volume.h"
#include "Comport.h"

int main()
{
    Comport comport;
    HANDLE hComm = INVALID_HANDLE_VALUE;

    Volume volume;

    while (hComm == INVALID_HANDLE_VALUE) {
        hComm = comport.initComport("COM11");
        if (hComm == INVALID_HANDLE_VALUE) {
            CloseHandle(hComm);
            std::cerr << "Failed to initialize COM port. Retrying in 5 seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));  // Wait before retrying
        }
    }

    if (comport.isInitialized()) {
        comport.configureComport();
    }
    else {
        std::cerr << "Failed to initialize COM port. Exiting." << std::endl;
        return 1;
    }

    char buffer[256];
    DWORD bytesRead;

    while (true) {
        if (ReadFile(hComm, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0'; // Null-terminate the string

                volume.handleVolumeChange(buffer);
            }
        }
        else {
            DWORD dwError = GetLastError();
            std::cerr << "Error reading from COM port: " << dwError << std::endl;

            // Try to reinitialize the COM port
            CloseHandle(hComm);
            hComm = INVALID_HANDLE_VALUE;

            while (hComm == INVALID_HANDLE_VALUE) {
                std::cerr << "Attempting to reinitialize COM port..." << std::endl;
                hComm = comport.initComport("COM11");
                if (hComm == INVALID_HANDLE_VALUE) {
                    std::cerr << "Reinitialization failed. Retrying in 5 seconds..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(5));
                }
                else {
                    comport.configureComport();
                }
            }
        }
    }

    CloseHandle(hComm);  // Always close the handle when done
    return 0;  // Return 0 for successful execution
}
