#pragma once
#include <vector>
#include <array>
#include "Config.hpp"
#include "glm/vec3.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};
struct Face {
    Vertex vertices[6];
};




// Define block types
enum BlockType {
    Air,
    Grass,
    Sand,
    Stone,
    Dirt,
    BlockTypeCount
};


constexpr std::array<glm::vec3, BlockTypeCount> blockColors = {{
    glm::vec3(0.0f, 0.0f, 0.0f),   // Air - black or transparent
    glm::vec3(0.13f, 0.54f, 0.13f), // Grass - green
    glm::vec3(0.93f, 0.79f, 0.52f), // Sand - sandy color
    glm::vec3(0.6f, 0.65f, 0.69f),  // Stone - gray
    glm::vec3(0.58f,0.29f,0.0f)
}};

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
