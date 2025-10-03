#pragma once
#include <vector>
#include <array>
#include <sys/types.h>

#include "Config.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "WorldGenerator.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec2 textureCoordinates;
    uint8_t ao;
};
struct Face {
    Vertex vertices[6];
};

enum FaceDirection {
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom
};





class Chunk {
    unsigned char blocks[Config::chunkSize][Config::chunkSize][Config::chunkSize];

    bool isBlockSolid(int x,int y,int z) {

        if(x<0 || x >= Config::chunkSize
            || y<0 || y >=
            Config::chunkSize ||
            z<0 || z >= Config::chunkSize) {
            return false;
        }

        return blocks[z][y][x] != BlockType::Air;
    }
    u_int8_t computeCornerAo(FaceDirection faceDir,int corner,int x,int y,int z);
    inline uint8_t calcAO(bool side1, bool side2, bool corner);

public:
    const int m_chunkPositionX;
    const int m_chunkPositionY;
    const int m_chunkPositionZ;
    Chunk(int x,int y,int z);

    void setBlock(BlockType block,int x,int y,int z);
    void generateMesh(std::vector<Face>&mesh, Chunk* chunkNx,Chunk* chunkPx,Chunk* chunkNy, Chunk* chunkPy,Chunk* chunkNz,Chunk* chunkPz);

    inline glm::vec3 convertToWorldCoordinates(const glm::vec3 &coordinates);

};
