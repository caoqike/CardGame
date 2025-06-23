#include "JsonService.h"
#include "cocos2d.h"

bool JsonService::parseJson(const std::string& jsonData, rapidjson::Document& doc) {
    doc.Parse(jsonData.c_str());
    return !doc.HasParseError();
}

bool JsonService::parseJsonFromFile(const std::string& filePath, rapidjson::Document& doc) {
    std::string jsonData = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
    if (jsonData.empty()) {
        cocos2d::log("Failed to read file: %s", filePath.c_str());
        return false;
    }

    return parseJson(jsonData, doc);
}