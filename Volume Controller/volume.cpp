#include <iostream>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audiopolicy.h>
#include <atlbase.h>
#include "volume.h"
#include <psapi.h>
#include "strings.h"

void Volume::CleanUpMasterVolume(IMMDeviceEnumerator* deviceEnumerator, IMMDevice* defaultDevice, IAudioEndpointVolume* endpointVolume) {
    if (endpointVolume) {
        endpointVolume->Release();
    }
    if (defaultDevice) {
        defaultDevice->Release();
    }
    if (deviceEnumerator) {
        deviceEnumerator->Release();
    }
    CoUninitialize();
}

void Volume::CleanUpAppVolume(IMMDeviceEnumerator* deviceEnumerator, IMMDevice* defaultDevice, IAudioSessionManager2* sessionManager) {
    if (sessionManager) {
        sessionManager->Release();
    }
    if (defaultDevice) {
        defaultDevice->Release();
    }
    if (deviceEnumerator) {
        deviceEnumerator->Release();
    }
    CoUninitialize();
}

void Volume::handleVolumeChange(char* input, nlohmann::json settings) {
    std::string strBuffer(input);
    size_t pos = strBuffer.find('_');
    int potId;
    float fVolume = 0.0f;

    if (pos != std::string::npos) {
        potId = stoi(strBuffer.substr(0, pos));

        size_t volumeStart = pos + 1;
        if (volumeStart < strBuffer.length()) {
            std::string volume = strBuffer.substr(volumeStart);
            fVolume = std::stof(volume) / 100.0f;
        }

        auto applications = settings["applications"];

        for (auto& application : applications) {
            if (application["id"] == potId) {
                for (auto& appName : application["name"]) {
                    if (appName == "master") {
                        SetMasterVolume(fVolume);
                    }
                    else {
                        SetApplicationVolume(stringToWString(appName), fVolume);
                    }
                }
            }
        }
    }
}

void Volume::SetMasterVolume(float volumeLevel) {
    if (volumeLevel < 0.0f || volumeLevel > 1.0f) {
        std::cerr << "Volume level must be between 0.0 and 1.0" << std::endl;
        return;
    }

    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM library" << std::endl;
        return;
    }

    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr)) {
        std::cerr << "Failed to create device enumerator" << std::endl;
        CoUninitialize();
        return;
    }

    IMMDevice* defaultDevice = nullptr;
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &defaultDevice);
    if (FAILED(hr)) {
        std::cerr << "Failed to get default audio endpoint" << std::endl;
        CleanUpMasterVolume(deviceEnumerator, nullptr, nullptr);
        return;
    }

    IAudioEndpointVolume* endpointVolume = nullptr;
    hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (void**)&endpointVolume);
    if (FAILED(hr)) {
        std::cerr << "Failed to activate endpoint volume" << std::endl;
        CleanUpMasterVolume(deviceEnumerator, defaultDevice, nullptr);
        return;
    }

    hr = endpointVolume->SetMasterVolumeLevelScalar(volumeLevel, &GUID_NULL); // Corrected here
    if (FAILED(hr)) {
        std::cerr << "Failed to set master volume level" << std::endl;
    }

    CleanUpMasterVolume(deviceEnumerator, defaultDevice, endpointVolume);
}

void Volume::SetApplicationVolume(const std::wstring& appName, float volumeLevel) {
    if (volumeLevel < 0.0f || volumeLevel > 1.0f) {
        std::cerr << "Volume level must be between 0.0 and 1.0" << std::endl;
        return;
    }

    HRESULT hr = CoInitialize(nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize COM library" << std::endl;
        return;
    }

    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr)) {
        std::cerr << "Failed to create device enumerator" << std::endl;
        CoUninitialize();
        return;
    }

    IMMDevice* defaultDevice = nullptr;
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &defaultDevice);
    if (FAILED(hr)) {
        std::cerr << "Failed to get default audio endpoint" << std::endl;
        CleanUpAppVolume(deviceEnumerator, nullptr, nullptr);
        return;
    }

    IAudioSessionManager2* sessionManager = nullptr;
    hr = defaultDevice->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, nullptr, (void**)&sessionManager);
    if (FAILED(hr)) {
        std::cerr << "Failed to activate audio session manager" << std::endl;
        CleanUpAppVolume(deviceEnumerator, defaultDevice, nullptr);
        return;
    }

    IAudioSessionEnumerator* sessionEnumerator = nullptr;
    hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
    if (FAILED(hr)) {
        std::cerr << "Failed to get session enumerator" << std::endl;
        CleanUpAppVolume(deviceEnumerator, defaultDevice, sessionManager);
        return;
    }

    int sessionCount = 0;
    sessionEnumerator->GetCount(&sessionCount);
    for (int i = 0; i < sessionCount; i++) {
        IAudioSessionControl* sessionControl = nullptr;
        hr = sessionEnumerator->GetSession(i, &sessionControl);
        if (FAILED(hr)) {
            continue;
        }

        IAudioSessionControl2* sessionControl2 = nullptr;
        hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
        if (FAILED(hr)) {
            sessionControl->Release();
            continue;
        }

        DWORD processId;
        sessionControl2->GetProcessId(&processId);

        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
        if (processHandle) {
            WCHAR processName[MAX_PATH] = L"";
            if (GetModuleBaseName(processHandle, nullptr, processName, MAX_PATH)) {
                if (_wcsicmp(processName, appName.c_str()) == 0) {
                    ISimpleAudioVolume* audioVolume = nullptr;
                    hr = sessionControl2->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&audioVolume);
                    if (SUCCEEDED(hr)) {
                        audioVolume->SetMasterVolume(volumeLevel, nullptr);
                        audioVolume->Release();
                    }
                    sessionControl2->Release();
                    sessionControl->Release();
                    CloseHandle(processHandle);
                    break;
                }
            }
            CloseHandle(processHandle);
        }

        sessionControl2->Release();
        sessionControl->Release();
    }

    sessionEnumerator->Release();

    CleanUpAppVolume(deviceEnumerator, defaultDevice, sessionManager);
}