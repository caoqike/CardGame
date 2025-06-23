#pragma once
#include "json/document.h"
#include <string>

class JsonService {
public:
    static bool parseJson(const std::string& jsonData, rapidjson::Document& doc);
    static bool parseJsonFromFile(const std::string& filePath, rapidjson::Document& doc);
};