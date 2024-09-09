#include "ChunkColumn.hpp"

ChunkColumn::ChunkColumn(int height,int x,int z) {

    m_posX = x;
    m_posZ = z;
    for(int i=0;i<16;i++) {
        BlockType block;

        if(i > height) {
            block = BlockType::Air;
        }
        else {
            if(i < 4 ) {
                block = BlockType::Stone;
            }
            else {
                block = BlockType::Grass;
            }
        }


        m_chunks.emplace_back(m_posX,i,m_posZ,block);
    }


    for(Chunk& chunk : m_chunks) {
        chunk.generateMesh(m_mesh);
    }

}
const std::vector<Face>& ChunkColumn::getMesh()const {
    return m_mesh;
}
