#pragma once

namespace Config {
    constexpr unsigned int windowWidth = 1920;
    constexpr unsigned int windowHeight = 1080;

    constexpr unsigned int chunkSize = 16; // must be power of 2
    constexpr unsigned int chunkColumnHeight = 16;

    namespace detail {
        constexpr int compileTimeLog2(unsigned int n) {
            int shift = 0;
            while (n > 1) {
                n = n >> 1;
                shift++;
            }
            return shift;
        }


        constexpr bool isPowerOfTwo(unsigned int n) {
            return (n != 0) && ((n & (n - 1)) == 0);
        }
    }


    static_assert(detail::isPowerOfTwo(chunkSize),
                  "Chunk size MUST be a power of two (e.g., 8, 16, 32).");


    constexpr int chunkSizeShift = detail::compileTimeLog2(chunkSize);
    constexpr int chunkSizeMask = chunkSize - 1;

    constexpr int chunkRadius = 24;

    constexpr int SEA_LEVEL = 100;
    constexpr int chunkMaxBlockHeight = chunkSize * chunkColumnHeight;

    inline bool showFaceCount = false;
    inline bool wireframeMode = false;
}