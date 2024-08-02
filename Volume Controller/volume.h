#pragma once
#include <string>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audiopolicy.h>
#include "strings.h"
#include <fstream>
#include "json.hpp"

class Volume {
private:
    void CleanUpMasterVolume(IMMDeviceEnumerator* deviceEnumerator, IMMDevice* defaultDevice, IAudioEndpointVolume* endpointVolume);
    void CleanUpAppVolume(IMMDeviceEnumerator* deviceEnumerator, IMMDevice* defaultDevice, IAudioSessionManager2* sessionManager);
    void SetMasterVolume(float volumeLevel);
    void SetApplicationVolume(const std::wstring& appName, float volumeLevel);
public:
    void handleVolumeChange(char* input, nlohmann::json settings);
};
