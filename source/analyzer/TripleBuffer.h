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

// Advanced Compressor
#pragma once

#include <array>
#include <atomic>
#include <vector>

template <typename T>
class TripleBuffer
{
public:
    TripleBuffer() = default;

    void resize (size_t size)
    {
        for (auto& buf : buffers)
            buf.assign (size, T{});
    }

    size_t getSize() const { return buffers[0].size(); }

    T* getWritePointer()
    {
        return buffers[(size_t) writeIdx.load (std::memory_order_relaxed)].data();
    }

    void finishWrite()
    {
        int w = writeIdx.load (std::memory_order_relaxed);
        int m = midIdx.load (std::memory_order_relaxed);
        writeIdx.store (m, std::memory_order_relaxed);
        midIdx.store (w, std::memory_order_release);
        newData.store (true, std::memory_order_release);
    }

    bool newDataAvailable() const
    {
        return newData.load (std::memory_order_acquire);
    }

    bool swapRead()
    {
        if (! newData.load (std::memory_order_acquire))
            return false;

        int r = readIdx.load (std::memory_order_relaxed);
        int m = midIdx.load (std::memory_order_relaxed);
        readIdx.store (m, std::memory_order_relaxed);
        midIdx.store (r, std::memory_order_release);
        newData.store (false, std::memory_order_release);
        return true;
    }

    const T* getReadPointer() const
    {
        return buffers[(size_t) readIdx.load (std::memory_order_relaxed)].data();
    }

private:
    std::array<std::vector<T>, 3> buffers;
    std::atomic<int> writeIdx { 0 };
    std::atomic<int> midIdx   { 1 };
    std::atomic<int> readIdx  { 2 };
    std::atomic<bool> newData { false };
};
