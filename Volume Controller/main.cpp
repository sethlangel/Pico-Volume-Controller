#include <iostream>
#include <windows.h>
#include <thread>
#include <chrono>
#include "Volume.h"
#include "Comport.h"
#include <fstream>
#include "json.hpp"
#include "strings.h"

using json = nlohmann::json;

int main()
{
    Comport comport;
    Volume vVolume;

    int potId;
    float fVolume = 0.0f;

    std::ifstream f("settings.json");
    json settings = json::parse(f);

    comport.initComport(settings["comport"]);
        
    if (comport.isInitialized()) {
        comport.configureComport();
    }

    while (comport.isInitialized()) {
       std::string comData = comport.readFile();

       splitData(comData, potId, fVolume);

       auto applications = settings["applications"];

       for (auto& application : applications) {
           if (application["id"] == potId) {
               for (auto& appName : application["name"]) {
                   vVolume.handleVolumeChange(appName, fVolume);
               }
           }
       }
      
    }

    comport.closeHComm();
    return 0;
}
