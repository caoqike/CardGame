#include "CardConfigLoader.h"
#include "json/document.h"
#include "../../services/JsonService.h"
#include "../../models/CardModel.h"

bool CardConfigLoader::loadFromFile(const std::string& filePath, GameModel& model) {
    rapidjson::Document doc;
    if (!JsonService::parseJsonFromFile(filePath, doc)) {
        cocos2d::log("Failed to parse JSON from file: %s", filePath.c_str());
        return false;
    }

    // Ω‚ŒˆPlayfieldø®≈∆
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const auto& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++) {
            cocos2d::log("%d\n",i);
            const auto& cardObj = playfieldArray[i];
            if (cardObj.HasMember("CardFace") && cardObj.HasMember("CardSuit")) {
                int face = cardObj["CardFace"].GetInt();
                int suit = cardObj["CardSuit"].GetInt();
                model.playfieldCards.emplace_back(
                    static_cast<CardSuitType>(suit),
                    static_cast<CardFaceType>(face)
                );
            }
        }
    }

    // Ω‚ŒˆStackø®≈∆
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const auto& stackArray = doc["Stack"];

        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++) {
            cocos2d::log("%d\n", i);
            const auto& cardObj = stackArray[i];
            if (cardObj.HasMember("CardFace") && cardObj.HasMember("CardSuit")) {
                int face = cardObj["CardFace"].GetInt();
                int suit = cardObj["CardSuit"].GetInt();
                model.stackCards.emplace_back(
                    static_cast<CardSuitType>(suit),
                    static_cast<CardFaceType>(face)
                );
            }
        }
    }

    return true;
}