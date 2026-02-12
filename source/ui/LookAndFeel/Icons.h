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

#include <juce_graphics/juce_graphics.h>

/**
 * @brief Header-only icon paths for toolbar buttons.
 *
 * All paths are normalized to a 0-1 bounding box and drawn as stroked outlines.
 * Callers scale via path.applyTransform(AffineTransform::scale(size)).
 */
class Icons
{
public:
    /** Counter-clockwise curved arrow (undo). */
    static juce::Path getUndo()
    {
        juce::Path p;
        // Arrowhead tip pointing left (open V, not filled)
        p.startNewSubPath (0.32f, 0.18f);
        p.lineTo (0.15f, 0.35f);
        p.lineTo (0.35f, 0.45f);
        // Arc: curved tail from arrowhead sweeping clockwise around
        p.startNewSubPath (0.15f, 0.35f);
        p.cubicTo (0.25f, 0.15f, 0.65f, 0.1f, 0.78f, 0.3f);
        p.cubicTo (0.88f, 0.48f, 0.82f, 0.72f, 0.62f, 0.8f);
        p.cubicTo (0.48f, 0.86f, 0.3f, 0.78f, 0.25f, 0.65f);
        return p;
    }

    /** Clockwise curved arrow (redo). */
    static juce::Path getRedo()
    {
        auto p = getUndo();
        p.applyTransform (juce::AffineTransform::scale (-1.0f, 1.0f)
                              .translated (1.0f, 0.0f));
        return p;
    }

    /** Headphones icon (delta / difference listen). */
    static juce::Path getDelta()
    {
        juce::Path p;
        // Headband arc
        p.addArc (0.15f, 0.08f, 0.7f, 0.7f,
                  juce::MathConstants<float>::pi,
                  juce::MathConstants<float>::twoPi, true);
        // Left ear cup
        p.startNewSubPath (0.15f, 0.45f);
        p.lineTo (0.15f, 0.78f);
        p.lineTo (0.28f, 0.78f);
        p.lineTo (0.28f, 0.45f);
        p.lineTo (0.15f, 0.45f);
        // Right ear cup
        p.startNewSubPath (0.72f, 0.45f);
        p.lineTo (0.72f, 0.78f);
        p.lineTo (0.85f, 0.78f);
        p.lineTo (0.85f, 0.45f);
        p.lineTo (0.72f, 0.45f);
        return p;
    }

    /** Question mark (help). */
    static juce::Path getHelp()
    {
        juce::Path p;
        // Circle outline
        p.addEllipse (0.1f, 0.1f, 0.8f, 0.8f);
        // Question mark stem
        p.startNewSubPath (0.38f, 0.3f);
        p.cubicTo (0.38f, 0.18f, 0.62f, 0.18f, 0.62f, 0.33f);
        p.cubicTo (0.62f, 0.43f, 0.53f, 0.46f, 0.5f, 0.56f);
        // Dot below
        p.addEllipse (0.45f, 0.65f, 0.1f, 0.1f);
        return p;
    }

    /** Left-pointing chevron (<). */
    static juce::Path getChevronLeft()
    {
        juce::Path p;
        p.startNewSubPath (0.65f, 0.18f);
        p.lineTo (0.35f, 0.5f);
        p.lineTo (0.65f, 0.82f);
        return p;
    }

    /** Right-pointing chevron (>). */
    static juce::Path getChevronRight()
    {
        juce::Path p;
        p.startNewSubPath (0.35f, 0.18f);
        p.lineTo (0.65f, 0.5f);
        p.lineTo (0.35f, 0.82f);
        return p;
    }

    /** Floppy disk outline (save). */
    static juce::Path getSave()
    {
        juce::Path p;
        // Outer body with clipped corner
        p.startNewSubPath (0.15f, 0.1f);
        p.lineTo (0.68f, 0.1f);
        p.lineTo (0.85f, 0.27f);
        p.lineTo (0.85f, 0.9f);
        p.lineTo (0.15f, 0.9f);
        p.closeSubPath();
        // Label slot (bottom rectangle)
        p.startNewSubPath (0.3f, 0.55f);
        p.lineTo (0.7f, 0.55f);
        p.lineTo (0.7f, 0.82f);
        p.lineTo (0.3f, 0.82f);
        p.closeSubPath();
        // Disk slot (top)
        p.startNewSubPath (0.3f, 0.16f);
        p.lineTo (0.6f, 0.16f);
        p.lineTo (0.6f, 0.32f);
        p.lineTo (0.3f, 0.32f);
        p.closeSubPath();
        return p;
    }

    /** Double chevron up (sidechain toggle). */
    static juce::Path getSidechain()
    {
        juce::Path p;
        p.startNewSubPath (0.2f, 0.48f);
        p.lineTo (0.5f, 0.18f);
        p.lineTo (0.8f, 0.48f);
        p.startNewSubPath (0.2f, 0.78f);
        p.lineTo (0.5f, 0.48f);
        p.lineTo (0.8f, 0.78f);
        return p;
    }
};
