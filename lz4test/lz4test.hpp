//
//  lz4test.hpp
//  lz4test
//
//  Created by LARRYHOU on 2021/5/27.
//

#ifndef lz4test_hpp
#define lz4test_hpp

#define FILE_BLOCK_SIZE (32<<10)

#include <iostream>

namespace lz4test {

struct FileBlock
{
    int usize;
    int csize;
};

bool compress(const char* data, int usize, char* buf, int& csize);
bool compress(std::istream &input, std::ostream &output);

bool decompress(const char* data, int csize, char* buf, int usize);
bool decompress(std::istream &input, std::ostream &output);

}

#endif /* lz4test_hpp */
