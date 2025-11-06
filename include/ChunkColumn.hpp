#pragma once

#include <vector>
#include "Chunk.hpp"


class World;
struct Face;

class ChunkColumn {

public:

    ChunkColumn(int x, int z, World& world);
    ~ChunkColumn();


    void generateMesh();
    void disconnectNeighbours();
    void setNeighbouringChunks(ChunkColumn* chunkNx, ChunkColumn* chunkPx, ChunkColumn* chunkNz, ChunkColumn* chunkPz);


    void uploadToGpu();
    void destroyGL();
    void bindSolidMesh() const;
    void bindTransparentMesh() const;


    [[nodiscard]] BlockType getBlockAt(int localX,int worldY,int localZ) const;
    [[nodiscard]] const std::vector<Face>& getMesh() const;
    [[nodiscard]] Chunk* getChunk(int height);
    [[nodiscard]] const Chunk* getChunk(int height) const;

    void setBlockAt(int localX,int worldY,int localZ,BlockType block);

    void setNeighbourNx(ChunkColumn* chunkNX) { m_nbrChunkColumnNX = chunkNX; }
    void setNeighbourNz(ChunkColumn* chunkNZ) { m_nbrChunkColumnNZ = chunkNZ; }
    void setNeighbourPx(ChunkColumn* chunkPX) { m_nbrChunkColumnPX = chunkPX; }
    void setNeighbourPz(ChunkColumn* chunkPZ) { m_nbrChunkColumnPZ = chunkPZ; }

    [[nodiscard]] ChunkColumn* getNeighbourNx() { return m_nbrChunkColumnNX; }
    [[nodiscard]] ChunkColumn* getNeighbourPx() { return m_nbrChunkColumnPX; }
    [[nodiscard]] ChunkColumn* getNeighbourNz() { return m_nbrChunkColumnNZ; }
    [[nodiscard]] ChunkColumn* getNeighbourPz() { return m_nbrChunkColumnPZ; }


    [[nodiscard]] const ChunkColumn* getNeighbourNx() const { return m_nbrChunkColumnNX; }
    [[nodiscard]] const ChunkColumn* getNeighbourPx() const { return m_nbrChunkColumnPX; }
    [[nodiscard]] const ChunkColumn* getNeighbourNz() const { return m_nbrChunkColumnNZ; }
    [[nodiscard]] const ChunkColumn* getNeighbourPz() const { return m_nbrChunkColumnPZ; }


    [[nodiscard]] bool isMeshDirty() const { return m_isMeshDirty; }
    [[nodiscard]] bool isCpuMeshReady() const { return m_cpuMeshReady; }
    [[nodiscard]] bool isGpuMeshReady() const { return m_gpuMeshReady; }
    [[nodiscard]] unsigned int getSolidMeshSize() const { return m_solidMeshSize; }
    [[nodiscard]] unsigned int getTransparentMeshSize() const { return m_transparentMeshSize; }


    void setMeshDirty(bool dirty) { m_isMeshDirty = dirty; }
    void setGpuMeshReady(bool ready) { m_gpuMeshReady = ready; }
    void setCpuMeshReady(bool ready) { m_cpuMeshReady = ready; }


private:

    void resetMeshesContainers();

    int m_posX;
    int m_posZ;
    std::array<Chunk,Config::chunkColumnHeight> m_chunks;
    std::vector<Face> m_solidMesh;
    std::vector<Face> m_transparentMesh;


    ChunkColumn* m_nbrChunkColumnNX = nullptr;
    ChunkColumn* m_nbrChunkColumnNZ = nullptr;
    ChunkColumn* m_nbrChunkColumnPX = nullptr;
    ChunkColumn* m_nbrChunkColumnPZ = nullptr;

    // FLAGS
    bool m_isMeshDirty = true;
    bool m_cpuMeshReady = false;
    bool m_gpuMeshReady = false;

    // GPU DATA
    unsigned int m_solidVAO = 0;
    unsigned int m_solidVBO = 0;
    unsigned int m_transparentVAO = 0;
    unsigned int m_transparentVBO = 0;
    unsigned int m_solidMeshSize = 0;
    unsigned int m_transparentMeshSize = 0;
};