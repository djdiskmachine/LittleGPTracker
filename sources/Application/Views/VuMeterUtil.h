#ifndef _VU_METER_UTIL_H_
#define _VU_METER_UTIL_H_

#include "Application/Mixer/MixerService.h"
#include "BaseClasses/View.h"

/**
 * VuMeterUtil: Common utility functions for VU meter rendering.
 * Shared between MixerView and SongView for consistent slew rate decay.
 */

// Decay factor: 0.85 = keeps 85% of old value, adds 15% of new.
// Creates smooth falloff behavior while allowing fast attack.
const float VU_DECAY_FACTOR = 0.85f;

/**
 * UpdateVuBarHeights:
 *   Apply slew rate decay to VU bar heights.
 *   Implements fast attack (instant rise) and slow decay (smooth fall).
 */
inline void UpdateVuBarHeights(int *vuBarHeights, int *displayHeights,
                               float *peakLevels, int numChannels) {
    for (int i = 0; i < numChannels; i++) {
        int newBarHeight = (int)(peakLevels[i] * VU_METER_HEIGHT);
        if (newBarHeight > VU_METER_HEIGHT) newBarHeight = VU_METER_HEIGHT;

        if (newBarHeight > vuBarHeights[i]) {
            vuBarHeights[i] = newBarHeight;  // Instant rise to peak
        } else {
            // Decay: blend 85% old + 15% new
            vuBarHeights[i] = (int)(vuBarHeights[i] * VU_DECAY_FACTOR +
                                    newBarHeight * (1.0f - VU_DECAY_FACTOR));
        }

        displayHeights[i] = vuBarHeights[i];
    }
}

/**
 * ReadMixBusPeakLevels:
 *   Read stereo L/R peak levels from all 8 mix buses.
 *   Returns 0.0 for all channels when isPlaying is false.
 */
inline void ReadMixBusPeakLevels(bool isPlaying, float *peakLevelsL,
                                  float *peakLevelsR) {
    if (!isPlaying) {
        for (int i = 0; i < 8; i++) {
            peakLevelsL[i] = peakLevelsR[i] = 0.0f;
        }
        return;
    }
    MixerService *ms = MixerService::GetInstance();
    for (int i = 0; i < 8; i++) {
        MixBus *bus = ms->GetMixBus(i);
        if (bus) {
            uint32_t level = bus->GetPeakLevel();
            peakLevelsL[i] = (float)((level >> 16) & 0xFFFF) / 32767.0f;
            peakLevelsR[i] = (float)(level & 0xFFFF) / 32767.0f;
        } else {
            peakLevelsL[i] = peakLevelsR[i] = 0.0f;
        }
    }
}

/**
 * GetVuPeakLevelsStereo:
 *   Apply slew rate decay to stereo (L/R) VU bar heights.
 */
inline void GetVuPeakLevelsStereo(int *vuBarHeightsL, int *vuBarHeightsR,
                                   int *displayHeightsL, int *displayHeightsR,
                                   float *peakLevelsL, float *peakLevelsR) {
    UpdateVuBarHeights(vuBarHeightsL, displayHeightsL, peakLevelsL, 8);
    UpdateVuBarHeights(vuBarHeightsR, displayHeightsR, peakLevelsR, 8);
}

/**
 * GetVuBarColor:
 *   Return the color for a given VU bar row (0 = bottom, top = clip).
 */
inline ColorDefinition GetVuBarColor(int row) {
    if (row >= VU_METER_CLIP_LEVEL) {
        return CD_MAJORBEAT;  // Clip level = red
    } else if (row >= VU_METER_WARN_LEVEL) {
        return CD_HILITE2;    // Warning level = orange/yellow
    } else {
        return CD_CONSOLE;    // Normal = cyan
    }
}

/**
 * DrawVuBarRow:
 *   Draw a single row of a VU meter bar.
 *   Uses '=' for filled cells and '-' for empty cells.
 *   Restores rowColor after drawing an empty cell so the caller's color
 *   state is unaffected.
 */
inline void DrawVuBarRow(View *view, GUIPoint pos, int row, int barHeight,
                         GUITextProperties vuProps, ColorDefinition rowColor) {
    if (row < barHeight) {
        view->DrawString(pos._x, pos._y, "=", vuProps);
    } else {
        vuProps.invert_ = false;
        view->SetColor(CD_NORMAL);
        view->DrawString(pos._x, pos._y, "-", vuProps);
        view->SetColor(rowColor);
    }
}

#endif

