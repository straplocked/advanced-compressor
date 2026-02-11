#pragma once

#include "PresetData.h"
#include <vector>

namespace FactoryPresets
{
    std::vector<Preset> getVocalPresets();
    std::vector<Preset> getDrumPresets();
    std::vector<Preset> getBusPresets();
    std::vector<Preset> getMasteringPresets();
    std::vector<Preset> getCreativePresets();
    std::vector<Preset> getAll();
}
