#pragma once
#include <fstream>
#include <string>
#include <memory>
#include "core/GameState.hpp"
#include "core/GameException.hpp"
#include "models/Player/PlayerStatus.hpp"
#include "models/Plot/PropertyPlot/PropertyStatus.hpp"
#include "models/Plot/PlotType.hpp"
 
class SkillCard;
class Player;

class ConfigSaver {
private:
    static std::ofstream open(const std::string& path);

    static std::string playerStatusToString(PlayerStatus status);
    static std::string propertyStatusToString(PropertyStatus status);
    static std::string plotTypeToString(PlotType type);
    
    static std::string cardValueString(const std::shared_ptr<SkillCard>& card,
                                       const Player& player);

    static void savePlayerState(std::ofstream& file, const GameState& state);
    static void savePropertyState(std::ofstream& file, const GameState& state);
    static void saveDeckState(std::ofstream& file, const GameState& state);
    static void saveLogState(std::ofstream& file, const GameState& state);

public:
    static const std::string savePath;
    static const std::string defaultSaveFileName;
    
    static void save(const GameState& state, const std::string& filename);
    
    static bool fileExists(const std::string& filename);
};