/*
 * Advanced Compressor - Open-source dynamic range compressor plugin
 * Copyright (C) 2024-2026 Advanced Compressor contributors
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

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
