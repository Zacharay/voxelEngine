#pragma once
#include <glm/vec2.hpp>
#include "Chunk.hpp"


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
            default:
                break;
        }
    }
    void getTextureCoordinates(Vertex vertices[6],BlockType blockT,FaceDirection faceDir) {
        const float tileW = 1.0f / 32.0f;
        const float tileH = 1.0f / 32.0f;

        int tx,ty;
        getTexturePosition(tx,ty,blockT,faceDir);

        float verticalMin = ty * tileH;
        float verticalMax = verticalMin + tileH ;
        float horizontalMin = tx * tileW;
        float horizontalMax = horizontalMin + tileW;

        //top left
        vertices[0].textureCoordinates = glm::vec2(horizontalMin, verticalMax);
        //top-right
        vertices[1].textureCoordinates = glm::vec2(horizontalMax, verticalMax);
        //bottom-right
        vertices[2].textureCoordinates = glm::vec2(horizontalMax, verticalMin);
        //bottom - right
        vertices[3].textureCoordinates = glm::vec2(horizontalMax, verticalMin);
        //bottom-left
        vertices[4].textureCoordinates = glm::vec2(horizontalMin, verticalMin);
        //top-left
        vertices[5].textureCoordinates = glm::vec2(horizontalMin, verticalMax);


    }

}
