#pragma once

#include <vector>
#include <mutex>
#include <atomic>
#include "Config.hpp"
#include <array>
#include "BlockTypes.hpp"
#include "Chunk.hpp"

class World;
struct Face;



class ChunkColumn {

public:

    ChunkColumn(int x, int z, World& world);
    ~ChunkColumn();


    std::mutex m_meshDataMutex; // Protects m_solidMesh and m_transparentMesh

    std::atomic<bool> m_isMeshDirty{true};       // Set to true when blocks change
    std::atomic<bool> m_isGeneratingMesh{false}; // True if in thread pool
    std::atomic<bool> m_meshReadyForUpload{false};

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
    [[nodiscard]] unsigned int getSolidMeshSize() const { return m_solidMeshSize; }
    [[nodiscard]] unsigned int getTransparentMeshSize() const { return m_transparentMeshSize; }


    void setMeshDirty(bool dirty) {m_isMeshDirty = dirty;}

    int m_posX;
    int m_posZ;
private:

    World& m_world;

    std::array<Chunk,Config::chunkColumnHeight> m_chunks;

    std::vector<Face> m_solidMesh;
    std::vector<Face> m_transparentMesh;

    ChunkColumn* m_nbrChunkColumnNX = nullptr;
    ChunkColumn* m_nbrChunkColumnNZ = nullptr;
    ChunkColumn* m_nbrChunkColumnPX = nullptr;
    ChunkColumn* m_nbrChunkColumnPZ = nullptr;


    // GPU DATA
    unsigned int m_solidVAO = 0;
    unsigned int m_solidVBO = 0;
    unsigned int m_transparentVAO = 0;
    unsigned int m_transparentVBO = 0;
    unsigned int m_solidMeshSize = 0;
    unsigned int m_transparentMeshSize = 0;

    void generateTerrain();
};