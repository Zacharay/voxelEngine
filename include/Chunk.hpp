#pragma once
#include <vector>
#include <array>
#include "Config.hpp"
#include "glm/vec3.hpp"
#include "WorldGenerator.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
struct Face {
    Vertex vertices[6];
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
