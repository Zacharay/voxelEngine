#pragma once

namespace Config {
    constexpr unsigned int windowWidth = 1920;
    constexpr unsigned int windowHeight = 1080;

    constexpr unsigned int chunkSize = 16;
    constexpr unsigned int chunkColumnHeight = 16;

    constexpr int chunkRadius = 16;

    constexpr int noiseWidth = (chunkRadius * 2 ) * chunkSize;


    constexpr int chunkMaxBlockHeight = chunkSize * chunkColumnHeight;


    inline bool wireframeMode = false;
}