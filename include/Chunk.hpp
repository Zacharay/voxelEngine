#pragma once
#include <vector>
#include <array>
#include "Config.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "WorldGenerator.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec2 textureCoordinates;
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
public:
    const int m_chunkPositionX;
    const int m_chunkPositionY;
    const int m_chunkPositionZ;
    Chunk(int x,int y,int z);

    void setBlock(BlockType block,int x,int y,int z);
    void generateMesh(std::vector<Face>&mesh, Chunk* chunkNx,Chunk* chunkPx,Chunk* chunkNy, Chunk* chunkPy,Chunk* chunkNz,Chunk* chunkPz);

    inline glm::vec3 convertToWorldCoordinates(const glm::vec3 &coordinates);

};
