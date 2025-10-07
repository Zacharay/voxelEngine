#pragma once
#include <Chunk.hpp>
#include <FastNoiseLite.h>
#include <vector>
#include "WorldGenerator.hpp"


class Chunk;
struct Face;


class ChunkColumn {
    unsigned int m_VAO,m_VBO;

    int m_posX;
    int m_posZ;

    ChunkColumn* m_nbrChunkColumnNX ;
    ChunkColumn* m_nbrChunkColumnNZ ;
    ChunkColumn* m_nbrChunkColumnPX;
    ChunkColumn* m_nbrChunkColumnPZ ;


    std::vector<Chunk> m_chunks;
    std::vector<Face> m_mesh;



public:
    bool isMeshDirty = true;

    bool cpuMeshReady = false;
    bool gpuMeshReady = false;

    unsigned int m_meshSize;
    ChunkColumn(FastNoiseLite& m_noise,int x,int z,World *world);
    ~ChunkColumn();

    void generateMesh();
    void uploadToGpu();
    void bindMesh()const;
    void setNeighbouringChunks(ChunkColumn* chunkNx,ChunkColumn* chunkPx,ChunkColumn* chunkNz,ChunkColumn* chunkPz);
    const std::vector<Face>& getMesh()const;
    Chunk *getChunk(int height);
    BlockType getBlockAt(int chunkPosY,int x,int y,int z);

    void setNeighbourNx(ChunkColumn* chunkNX){m_nbrChunkColumnNX = chunkNX;}
    void setNeighbourNz(ChunkColumn* chunkNZ){m_nbrChunkColumnNZ = chunkNZ;};
    void setNeighbourPx(ChunkColumn* chunkPX){m_nbrChunkColumnPX = chunkPX;};
    void setNeighbourPz(ChunkColumn* chunkPZ){m_nbrChunkColumnPZ = chunkPZ;};

    ChunkColumn* getNeighbourNx()  { return m_nbrChunkColumnNX; }
    ChunkColumn* getNeighbourPx()  { return m_nbrChunkColumnPX; }
    ChunkColumn* getNeighbourNz()  { return m_nbrChunkColumnNZ; }
    ChunkColumn* getNeighbourPz()  { return m_nbrChunkColumnPZ; }


    void disconnectNeighbours();
    void destroyGL();
};
