//
//  lz4test.cpp
//  lz4test
//
//  Created by LARRYHOU on 2021/5/27.
//

#include "lz4test.hpp"

#include <lz4/lz4hc.h>
#include <lz4/lz4.h>

#include <fstream>
#include <string>
#include <vector>

bool lz4test::compress(std::istream &input, std::ostream &output)
{
    input.seekg(0, std::ios::end);
    auto remain = input.tellg();
    input.seekg(0);
    
    char dat[FILE_BLOCK_SIZE];
    char buf[FILE_BLOCK_SIZE];
    
    std::vector<FileBlock> metadata;
    
    int offset = 0;
    output.write((const char*)&offset, sizeof(int));
    output.write((const char*)&offset, sizeof(int));
    
    while (remain)
    {
        auto size = std::min<int>((int)remain, FILE_BLOCK_SIZE);
        input.read(dat, size);
        
        FileBlock page{size, 0};
        if (!compress(dat, size, buf, page.csize)) { return false; }
        output.write(buf, page.csize);
        metadata.push_back(page);
        remain -= size;
    }
    
    offset = (int)output.tellp();
    
    int usize = (int)(sizeof(FileBlock) * metadata.size());
    int csize = 0;
    
    output.write((const char*)&usize, sizeof(int));
    output.write((const char*)&csize, sizeof(int));
    
    {
        char buf[usize];
        if (!compress((const char*)metadata.data(), usize, buf, csize)) { return false; }
        output.write(buf, csize);
    }
    
    auto filesize = (int)output.tellp();
    
    output.seekp(0);
    output.write((const char*)&filesize, sizeof(int));
    output.write((const char*)&offset, sizeof(int));
    
    output.seekp(offset);
    output.write((const char*)&usize, sizeof(int));
    output.write((const char*)&csize, sizeof(int));
    
    return true;
}

bool lz4test::decompress(std::istream &input, std::ostream &output)
{
    int filesize = 0, offset = 0;
    input.read((char *)&filesize, sizeof(int));
    input.read((char *)&offset, sizeof(int));
    
    std::vector<FileBlock> metadata;
    
    {
        input.seekg(offset);
        int usize = 0, csize = 0;
        input.read((char *)&usize, sizeof(int));
        input.read((char *)&csize, sizeof(int));
        
        char dat[csize];
        char buf[usize];
        input.read(dat, csize);
        
        if (!decompress(dat, csize, buf, usize)) { return false; }
        
        metadata.resize(usize / sizeof(FileBlock));
        memcpy(metadata.data(), buf, usize);
    }
    
    input.seekg(sizeof(int) << 1);
    
    char dat[FILE_BLOCK_SIZE];
    char buf[FILE_BLOCK_SIZE];
    for (auto iter = metadata.begin(); iter != metadata.end(); iter++)
    {
        input.read(dat, iter->csize);
        if (!decompress(dat, iter->csize, buf, iter->usize)) { return false; }
        output.write(buf, iter->usize);
    }
    
    return true;
}

bool lz4test::compress(const char* data, int usize, char* buf, int& csize)
{
    csize = LZ4_compressHC2_limitedOutput(data, buf, usize, usize, 16);
    if (csize <= 0 || csize >= usize)
    {
        memcpy(buf, data, usize);
        csize = usize;
    }
    
    return true;
}

bool lz4test::decompress(const char* data, int csize, char* buf, int usize)
{
    if (csize == usize)
    {
        memcpy(buf, data, csize);
        return csize == usize;
    }
    
    return LZ4_decompress_safe(data, buf, csize, usize) == usize;
}

