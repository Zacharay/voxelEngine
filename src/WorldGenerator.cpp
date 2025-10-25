#include "WorldGenerator.hpp"

#include <World.hpp>
#include <iostream>
#include "Config.hpp"

void WorldGenerator::buildTree(ChunkColumn *chunkColumn,int x_local,int y_start,int z_local){

    const int TREE__TRUNK_HEIGHT = 5;

    //generate trunk
    for(int i=0;i<TREE__TRUNK_HEIGHT;i++) {
        int chunkY = (y_start+i)/Config::chunkSize;
        int y_local = (y_start+i)%Config::chunkSize;
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LOG,x_local,y_local ,z_local);
    }

    const int TREE_LEAVES_START_POSITION = 3;

    for(int y=0;y<2;y++) {
        int chunkY = (y_start+TREE_LEAVES_START_POSITION+y)/Config::chunkSize;
        int y_local = (y_start+TREE_LEAVES_START_POSITION+y)%Config::chunkSize;
        for(int x=x_local - 2 ; x<=x_local + 2 ; x++) {
            for(int z=z_local - 2 ; z<=z_local + 2 ; z++) {
                if(z==z_local && x==x_local)continue;

                if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize)
                chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x,y_local,z);
            }
        }
    }

    int chunkY = (y_start+TREE_LEAVES_START_POSITION+2)/Config::chunkSize;
    int y_local = (y_start+TREE_LEAVES_START_POSITION+2)%Config::chunkSize;
    for(int x=x_local - 1 ; x<=x_local + 1 ; x++) {
        for(int z=z_local - 1 ; z<=z_local + 1 ; z++) {
            if(x>=0&&z>=0&&x<Config::chunkSize &&z<Config::chunkSize)
            chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x,y_local,z);
        }
    }

    int directions[ ]= {-1,0,1};
    chunkY = (y_start+TREE_LEAVES_START_POSITION+3)/Config::chunkSize;
    y_local = (y_start+TREE_LEAVES_START_POSITION+3)%Config::chunkSize;
    for (int d :directions) {
        if(x_local+d>=0&&z_local>=0&&x_local+d<Config::chunkSize &&z_local<Config::chunkSize)
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x_local+d,y_local,z_local);
        if(x_local>=0&&z_local+d>=0&&x_local<Config::chunkSize &&z_local+d<Config::chunkSize)
        chunkColumn->getChunk(chunkY)->setBlock(BlockType::OAK_LEAVES,x_local,y_local,z_local+d);
    }
}
