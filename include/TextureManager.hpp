#pragma once
#include <glm/vec2.hpp>
#include "Chunk.hpp"
#include "WorldGenerator.hpp"

namespace TextureManager {

    namespace detail {
        struct TextureCoord {
            uint8_t u, v;
        };

        constexpr std::array<TextureCoord, 6> allFaces(uint8_t tx, uint8_t ty) {
            return { TextureCoord{tx, ty}, {tx, ty}, {tx, ty}, {tx, ty}, {tx, ty}, {tx, ty} };
        }

        constexpr std::array<TextureCoord, 6> topBottomSide(uint8_t topX, uint8_t topY,
                                                            uint8_t bottomX, uint8_t bottomY,
                                                            uint8_t sideX, uint8_t sideY) {
            return {
                TextureCoord{sideX, sideY}, TextureCoord{sideX, sideY},
                TextureCoord{sideX, sideY}, TextureCoord{sideX, sideY},
                TextureCoord{topX, topY},   TextureCoord{bottomX, bottomY}
            };
        }


        constexpr size_t TABLE_SIZE = static_cast<size_t>(BlockType::BlockTypeCount);

        inline constexpr auto textureTable = []() {
            std::array<std::array<TextureCoord, 6>, TABLE_SIZE> t{};

            for (auto& block : t) block = allFaces(0, 0);


            auto Set = [&](BlockType type, const std::array<TextureCoord, 6>& faces) {
                t[static_cast<size_t>(type)] = faces;
            };

            Set(BlockType::Water,                allFaces(8, 0));
            Set(BlockType::Stone,                allFaces(4, 0));
            Set(BlockType::Sand,                 allFaces(9, 0));
            Set(BlockType::Grass,           topBottomSide(0, 0, 2, 0, 1, 0));
            Set(BlockType::OAK_LOG,         topBottomSide(5, 0, 5, 0, 6, 0));
            Set(BlockType::Cactus,          topBottomSide(15, 0, 14, 0, 16, 0));
            Set(BlockType::OAK_LEAVES,          allFaces(7, 0));
            Set(BlockType::OAK_PLANKS,          allFaces(17, 0));
            Set(BlockType::Snow,                allFaces(10, 0));
            Set(BlockType::Spruce_Log,      topBottomSide(11, 0, 11, 0, 12, 0));
            Set(BlockType::Spruce_Leaves,       allFaces(13, 0));

            return t;
        }();
    }


    inline void getTexturePosition(int &tx, int &ty, BlockType blockType, FaceDirection faceDir) {
        const auto& coord = detail::textureTable[static_cast<size_t>(blockType)][static_cast<size_t>(faceDir)];

        tx = coord.u;
        ty = coord.v;
    }
}