#pragma once


enum class BlockType :unsigned char{
    Air                 = 0,
    Water               = 1,
    Grass               = 2,
    Sand                = 3,
    Stone               = 4,
    Dirt                = 5,
    OAK_LOG             = 6,
    OAK_LEAVES          = 7,
    OAK_PLANKS          = 8,
    Spruce_Log          = 9,
    Spruce_Leaves       = 10,
    Cactus              = 11,
    Snow                = 12,
    BlockTypeCount      = 13
};

namespace BlockUtils {
    [[nodiscard]] constexpr bool isTransparent(BlockType type) {
        return type == BlockType::Air || type == BlockType::Water;
    }
}
