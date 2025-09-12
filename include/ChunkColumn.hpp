#pragma once
#include <Chunk.hpp>
#include <vector>


class ChunkColumn {
    unsigned int m_VAO,m_VBO;

    int m_posX;
    int m_posZ;

    ChunkColumn* m_nbrChunkColumnNX = nullptr;
    ChunkColumn* m_nbrChunkColumnNZ = nullptr;
    ChunkColumn* m_nbrChunkColumnPX = nullptr;
    ChunkColumn* m_nbrChunkColumnPZ = nullptr;


    std::vector<Chunk> m_chunks;
    std::vector<Face> m_mesh;

    public:
    ChunkColumn(const std::vector<std::vector<unsigned int>> &noiseData,int x,int z);

    void generateMesh();
    void bindMesh()const;
    void setNeighbouringChunks(ChunkColumn* chunkNx,ChunkColumn* chunkPx,ChunkColumn* chunkNz,ChunkColumn* chunkPz);
    const std::vector<Face>& getMesh()const;
    Chunk *getChunk(int height);
};
