#pragma once
#include <Chunk.hpp>
#include <FastNoiseLite.h>
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
    bool isMeshDirty = true;
    unsigned int m_meshSize;
    ChunkColumn(FastNoiseLite& m_noise,int x,int z);
    ~ChunkColumn();

    void generateMesh();
    void bindMesh()const;
    void setNeighbouringChunks(ChunkColumn* chunkNx,ChunkColumn* chunkPx,ChunkColumn* chunkNz,ChunkColumn* chunkPz);
    const std::vector<Face>& getMesh()const;
    Chunk *getChunk(int height);

    void setNeighbourNx(ChunkColumn* chunkNX){m_nbrChunkColumnNX = chunkNX;}
    void setNeighbourNz(ChunkColumn* chunkNZ){m_nbrChunkColumnNZ = chunkNZ;};
    void setNeighbourPx(ChunkColumn* chunkPX){m_nbrChunkColumnPX = chunkPX;};
    void setNeighbourPz(ChunkColumn* chunkPZ){m_nbrChunkColumnPZ = chunkPZ;};
    void disconnectNeighbours();
    void destroyGL();
};
