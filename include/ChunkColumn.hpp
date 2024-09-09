#pragma once
#include <Chunk.hpp>
#include <vector>


class ChunkColumn {

    int m_posX;
    int m_posZ;


    std::vector<Chunk> m_chunks;
    std::vector<Face> m_mesh;

    public:
    ChunkColumn(int height,int x,int z);
    const std::vector<Face>& getMesh()const;
};
