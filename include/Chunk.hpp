#pragma once
#include <vector>

#include "Config.hpp"
#include "MeshRenderer.hpp"




class Chunk {
    unsigned char blocks[Config::chunkSize][Config::chunkSize][Config::chunkSize];


    std::vector<Face> mesh;

    const int m_chunkPositionX;
    const int m_chunkPositionY;
    const int m_chunkPositionZ;
public:
    Chunk(int x,int y,int z);
    void generateMesh();
    const std::vector<Face> &getMesh()const;
};
