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

#include <vector>
#include <algorithm>

class GRHistoryBuffer
{
public:
    GRHistoryBuffer() = default;

    void setCapacity (int numSamples)
    {
        buffer.assign ((size_t) numSamples, 0.0f);
        writePos = 0;
    }

    void push (float grDB)
    {
        if (buffer.empty()) return;
        buffer[(size_t) writePos] = grDB;
        writePos = (writePos + 1) % (int) buffer.size();
    }

    // Returns GR value at position [0..size-1], where size-1 is the most recent
    float getSample (int index) const
    {
        int capacity = (int) buffer.size();
        if (capacity == 0) return 0.0f;
        int readIdx = (writePos + index) % capacity;
        return buffer[(size_t) readIdx];
    }

    int getCapacity() const { return (int) buffer.size(); }

    void clear()
    {
        std::fill (buffer.begin(), buffer.end(), 0.0f);
    }

private:
    std::vector<float> buffer;
    int writePos = 0;
};
