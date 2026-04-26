#pragma once
#include <fstream>
#include <string>
#include <memory>
#include <map>
#include "core/GameState.hpp"
#include "core/GameException.hpp"
#include "models/Player/PlayerStatus.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"
#include "models/Plot/PlotType.hpp"
#include "models/Card/SkillCard/MoveCard.hpp"
#include "models/Card/SkillCard/DiscountCard.hpp"
#include "models/Card/SkillCard/TeleportCard.hpp"
#include "models/Card/SkillCard/ShieldCard.hpp"
#include "models/Card/SkillCard/LassoCard.hpp"
#include "models/Card/SkillCard/DemolitionCard.hpp"
#include "models/Plot/PropertyPlot/LandPlot.hpp"
#include "models/Plot/PropertyPlot/PropertyPlot.hpp"

class SaveLoader {
private:
    static std::ifstream open(const std::string& path);
 
    static PlayerStatus stringToPlayerStatus(const std::string& str);
    static int parseJailTurns(const std::string& str);
    static PropertyStatus stringToPropertyStatus(const std::string& str);
 
    static std::shared_ptr<SkillCard> makeSkillCard(
        const std::string& name, int value, int duration);
 
    static std::map<std::string,std::string> loadPlayerState(std::ifstream& file, GameState& state);
    static void loadPropertyState(std::ifstream& file, GameState& state, const std::map<std::string,std::string>& playerPosCodes);
    static void loadDeckState(std::ifstream& file, GameState& state);
    static void loadLogState(std::ifstream& file, GameState& state);
 
public:
    static const std::string savePath;
 
    static void load(GameState& state, const std::string& filename);
    static bool fileExists(const std::string& filename);
};