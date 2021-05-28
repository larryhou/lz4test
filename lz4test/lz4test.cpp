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

double lz4t_progress = 0.0f;

bool lz4t_decompress(const char* data, int csize, char* buf, int usize);
bool lz4t_compress(const char* data, int usize, char* buf, int& csize);

namespace {
struct LZ4TFileBlock
{
    int usize;
    int csize;
};
}

bool lz4t_compress(const char* filename, const char *savename)
{
    std::fstream input;
    input.open(filename, std::ios::in);
    if (!input.good()) { return false; }
    
    std::fstream output;
    output.open(savename, std::ios::out);
    if (!output.good()) { return false; }
    
    input.seekg(0, std::ios::end);
    auto remain = (int)input.tellg();
    input.seekg(0);
    
    char dat[FILE_BLOCK_SIZE];
    char buf[FILE_BLOCK_SIZE];
    
    std::vector<LZ4TFileBlock> metadata;
    
    int offset = 0;
    output.write((const char*)&offset, sizeof(int));
    output.write((const char*)&offset, sizeof(int));
    
    int progress = 0;
    
    auto block_count = (remain + FILE_BLOCK_SIZE - 1) / FILE_BLOCK_SIZE;
    auto total_bytes = block_count * sizeof(LZ4TFileBlock) + remain;
    
    while (remain)
    {
        auto size = std::min<int>((int)remain, FILE_BLOCK_SIZE);
        input.read(dat, size);
        
        LZ4TFileBlock page{size, 0};
        if (!lz4t_compress(dat, size, buf, page.csize)) { return false; }
        output.write(buf, page.csize);
        metadata.push_back(page);
        remain -= size;
        
        progress += size;
        lz4t_progress = (double)progress / (double)total_bytes;
    }
    
    offset = (int)output.tellp();
    
    int usize = (int)(sizeof(LZ4TFileBlock) * metadata.size());
    int csize = 0;
    
    output.write((const char*)&usize, sizeof(int));
    output.write((const char*)&csize, sizeof(int));
    
    {
        char buf[usize];
        if (!lz4t_compress((const char*)metadata.data(), usize, buf, csize)) { return false; }
        output.write(buf, csize);
        
        progress += usize;
        lz4t_progress = (double)progress / (double)total_bytes;
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

bool lz4t_decompress(const char* filename, const char *savename)
{
    std::fstream input;
    input.open(filename, std::ios::in);
    if (!input.good()) { return false; }
    
    std::fstream output;
    output.open(savename, std::ios::out);
    if (!output.good()) { return false; }
    
    int filesize = 0, offset = 0;
    input.read((char *)&filesize, sizeof(int));
    input.read((char *)&offset, sizeof(int));
    
    std::vector<LZ4TFileBlock> metadata;
    
    int progress = sizeof(int) * 4;
    lz4t_progress = (double)progress / (double)filesize;
    
    {
        input.seekg(offset);
        int usize = 0, csize = 0;
        input.read((char *)&usize, sizeof(int));
        input.read((char *)&csize, sizeof(int));
        
        char dat[csize];
        char buf[usize];
        input.read(dat, csize);
        
        if (!lz4t_decompress(dat, csize, buf, usize)) { return false; }
        
        metadata.resize(usize / sizeof(LZ4TFileBlock));
        memcpy(metadata.data(), buf, usize);
        
        progress += csize;
        lz4t_progress = (double)progress / (double)filesize;
    }
    
    input.seekg(sizeof(int) << 1);
    
    char dat[FILE_BLOCK_SIZE];
    char buf[FILE_BLOCK_SIZE];
    for (auto iter = metadata.begin(); iter != metadata.end(); iter++)
    {
        input.read(dat, iter->csize);
        if (!lz4t_decompress(dat, iter->csize, buf, iter->usize)) { return false; }
        output.write(buf, iter->usize);
        
        progress += iter->csize;
        lz4t_progress = (double)progress / (double)filesize;
    }
    
    return true;
}

bool lz4t_compress(const char* data, int usize, char* buf, int& csize)
{
    csize = LZ4_compressHC2_limitedOutput(data, buf, usize, usize, 16);
    if (csize <= 0 || csize >= usize)
    {
        memcpy(buf, data, usize);
        csize = usize;
    }
    
    return true;
}

bool lz4t_decompress(const char* data, int csize, char* buf, int usize)
{
    if (csize == usize)
    {
        memcpy(buf, data, csize);
        return csize == usize;
    }
    
    return LZ4_decompress_safe(data, buf, csize, usize) == usize;
}

