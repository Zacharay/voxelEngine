#pragma once
#include <glm/vec2.hpp>
#include "Chunk.hpp"
#include "WorldGenerator.hpp"


namespace TextureManager {
    void getTexturePosition(int &tx,int &ty,BlockType bType,FaceDirection fDir) {
        switch (bType) {
            case BlockType::Grass:
                ty= 0;
                if(fDir == FaceDirection::Top ) {
                    tx = 0;
                }
                else if(fDir == FaceDirection::Bottom ) {
                    tx = 2;
                }
                else {
                    tx = 1;
                }
            break;
            case BlockType::Stone:
                tx = 4;
                ty = 0;
            break;
            case BlockType::Sand:
                ty = 0;
                tx = 9;
            break;
            case BlockType::Water:
                ty = 0;
                tx = 8;
            break;
            case BlockType::OAK_LOG:
                if(fDir == FaceDirection::Top || fDir == FaceDirection::Bottom ) {
                    tx = 5;
                }
                else {
                    tx = 6;
                }
                ty = 0;

                break;
            case BlockType::OAK_LEAVES:
                ty = 0;
                tx = 7;
                break;
            case BlockType::OAK_PLANKS:
                ty =0;
                tx = 17;
            break;
            case BlockType::Snow:
                ty =0;
                tx = 10;
            break;
            case BlockType::Spruce_Log:
                if(fDir == FaceDirection::Top || fDir == FaceDirection::Bottom ) {
                    tx = 11;
                }
                else {
                    tx = 12;
                }
                ty = 0;

                break;
            case BlockType::Spruce_Leaves:
                ty =0;
                tx = 13;
                break;
            case BlockType::Cactus:
                ty= 0;
                if(fDir == FaceDirection::Top ) {
                    tx = 15;

                }
                else if(fDir == FaceDirection::Bottom ) {
                    tx = 14;
                }
                else {
                    tx = 16;
                }
                break;
            default:
                break;
        }
    }

}
