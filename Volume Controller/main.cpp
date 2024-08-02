#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Volume.h"
#include "Comport.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main()
{
    std::ifstream f("settings.json");
    json settings = json::parse(f);
    Comport comport;
    Volume volume;

    HANDLE hComm = INVALID_HANDLE_VALUE;

    hComm = comport.initComport(settings["comport"]);
        

    if (comport.isInitialized()) {
        comport.configureComport();
    }

    while (true) {
        comport.readFile(hComm, volume, settings, comport);
    }

    CloseHandle(hComm);
    return 0;
}
