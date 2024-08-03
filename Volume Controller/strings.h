#pragma once
#include <string>

std::wstring stringToWString(const std::string& str);

void splitData(std::string data, int& potId, float& fVolume);