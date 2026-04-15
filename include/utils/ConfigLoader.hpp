#pragma once

#include <fstream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include "models/Plot/Plot.hpp"

class ConfigLoader{
public:
    static std::vector<std::pair<int, Plot>> loadProperty(std::string path);

    static std::map<int, int> loadRailroad(std::string path);

    static std::map<int, int> loadUtility(std::string path);

    static std::tuple<int, int, int> loadTax(std::string path);

    static std::tuple<int, int> loadSpecial(std::string path);

    static std::tuple<int, int> loadMisc(std::string path);
};