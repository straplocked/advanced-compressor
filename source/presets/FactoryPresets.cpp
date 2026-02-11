#include "FactoryPresets.h"

namespace FactoryPresets
{

std::vector<Preset> getVocalPresets()
{
    std::vector<Preset> presets;

    presets.push_back ({ "Vocal Smooth", "Vocal", { -24.0f, 3.0f, 8.0f, 60.0f, 15.0f, 150.0f, 0.0f, 3, 100.0f, 0.0f } });
    presets.push_back ({ "Vocal Aggressive", "Vocal", { -18.0f, 6.0f, 4.0f, 60.0f, 5.0f, 80.0f, 0.0f, 3, 100.0f, 2.0f } });
    presets.push_back ({ "Vocal Parallel", "Vocal", { -30.0f, 8.0f, 6.0f, 60.0f, 8.0f, 120.0f, 0.0f, 0, 50.0f, 0.0f } });
    presets.push_back ({ "Vocal De-Esser", "Vocal", { -28.0f, 4.0f, 3.0f, 30.0f, 0.5f, 40.0f, 0.0f, 0, 100.0f, 0.0f } });

    return presets;
}

std::vector<Preset> getDrumPresets()
{
    std::vector<Preset> presets;

    presets.push_back ({ "Drum Snap", "Drums", { -20.0f, 4.0f, 6.0f, 60.0f, 0.1f, 80.0f, 5.0f, 6, 100.0f, 1.0f } });
    presets.push_back ({ "Drum Smash", "Drums", { -25.0f, 10.0f, 3.0f, 60.0f, 1.0f, 50.0f, 0.0f, 0, 100.0f, 3.0f } });
    presets.push_back ({ "Drum Room Crush", "Drums", { -35.0f, 20.0f, 0.0f, 60.0f, 0.5f, 200.0f, 0.0f, 7, 40.0f, 0.0f } });
    presets.push_back ({ "Kick Tighten", "Drums", { -16.0f, 3.0f, 6.0f, 60.0f, 5.0f, 60.0f, 0.0f, 0, 100.0f, 0.0f } });

    return presets;
}

std::vector<Preset> getBusPresets()
{
    std::vector<Preset> presets;

    presets.push_back ({ "Bus Glue", "Bus", { -22.0f, 2.5f, 10.0f, 60.0f, 20.0f, 200.0f, 0.0f, 5, 100.0f, 0.0f } });
    presets.push_back ({ "Mix Bus Gentle", "Bus", { -18.0f, 2.0f, 12.0f, 60.0f, 30.0f, 300.0f, 0.0f, 5, 100.0f, 0.0f } });
    presets.push_back ({ "Drum Bus Punch", "Bus", { -20.0f, 4.0f, 8.0f, 60.0f, 10.0f, 100.0f, 5.0f, 6, 100.0f, 1.0f } });
    presets.push_back ({ "Guitar Bus", "Bus", { -24.0f, 3.0f, 8.0f, 60.0f, 15.0f, 150.0f, 0.0f, 1, 100.0f, 0.0f } });

    return presets;
}

std::vector<Preset> getMasteringPresets()
{
    std::vector<Preset> presets;

    presets.push_back ({ "Mastering Transparent", "Mastering", { -16.0f, 1.5f, 15.0f, 60.0f, 30.0f, 400.0f, 0.0f, 4, 100.0f, 0.0f } });
    presets.push_back ({ "Mastering Warm", "Mastering", { -18.0f, 2.0f, 12.0f, 60.0f, 25.0f, 300.0f, 0.0f, 1, 100.0f, 0.5f } });
    presets.push_back ({ "Mastering Loud", "Mastering", { -12.0f, 3.0f, 8.0f, 60.0f, 20.0f, 200.0f, 0.0f, 4, 100.0f, 2.0f } });

    return presets;
}

std::vector<Preset> getCreativePresets()
{
    std::vector<Preset> presets;

    presets.push_back ({ "Pumping EDM", "Creative", { -20.0f, 8.0f, 0.0f, 60.0f, 0.5f, 250.0f, 0.0f, 7, 100.0f, 0.0f } });
    presets.push_back ({ "Sidechain Duck", "Creative", { -30.0f, 100.0f, 0.0f, 60.0f, 0.1f, 200.0f, 0.0f, 0, 100.0f, 0.0f } });
    presets.push_back ({ "NY Compression", "Creative", { -35.0f, 10.0f, 6.0f, 60.0f, 5.0f, 80.0f, 0.0f, 0, 40.0f, 0.0f } });
    presets.push_back ({ "Opto Leveler", "Creative", { -22.0f, 3.0f, 10.0f, 60.0f, 20.0f, 500.0f, 0.0f, 2, 100.0f, 0.0f } });

    return presets;
}

std::vector<Preset> getAll()
{
    auto all = getVocalPresets();
    auto drums = getDrumPresets();
    auto bus = getBusPresets();
    auto master = getMasteringPresets();
    auto creative = getCreativePresets();

    all.insert (all.end(), drums.begin(), drums.end());
    all.insert (all.end(), bus.begin(), bus.end());
    all.insert (all.end(), master.begin(), master.end());
    all.insert (all.end(), creative.begin(), creative.end());
    return all;
}

} // namespace FactoryPresets
