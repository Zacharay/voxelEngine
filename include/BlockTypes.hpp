#pragma once
#include <string>
#include <string_view>
#include <array>

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
    constexpr size_t BLOCK_COUNT = static_cast<size_t>(BlockType::BlockTypeCount);
    constexpr std::array<std::string_view, BLOCK_COUNT> BLOCK_NAMES =[](){
            std::array<std::string_view, BLOCK_COUNT> blockNames;

            auto SetBlockName =[&](BlockType type,std::string_view name) {
                blockNames[static_cast<size_t>(type)] =name;
            };

            SetBlockName(BlockType::Air, "None");
            SetBlockName(BlockType::Water, "Water");
            SetBlockName(BlockType::Grass, "Grass");
            SetBlockName(BlockType::Sand, "Sand");
            SetBlockName(BlockType::Stone, "Stone");
            SetBlockName(BlockType::Dirt, "Dirt");
            SetBlockName(BlockType::OAK_LOG, "Oak Log");
            SetBlockName(BlockType::OAK_LEAVES, "Oak Leaves");
            SetBlockName(BlockType::OAK_PLANKS, "Oak Planks");
            SetBlockName(BlockType::Spruce_Log, "Spruce Log");
            SetBlockName(BlockType::Spruce_Leaves, "Spruce Leaves");
            SetBlockName(BlockType::Cactus, "Cactus");
            SetBlockName(BlockType::Snow, "Snow");

            return blockNames;
    }();

    [[nodiscard]] constexpr bool isTransparent(BlockType type) {
        return type == BlockType::Air || type == BlockType::Water;
    }
    [[nodiscard]]constexpr std::string_view getBlockName(BlockType type) {

        if (static_cast<size_t>(type) >= BLOCK_COUNT) return "Unknown";

        return BLOCK_NAMES[static_cast<size_t>(type)];
    }

}
