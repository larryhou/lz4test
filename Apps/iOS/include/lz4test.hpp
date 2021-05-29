//
//  lz4test.hpp
//  lz4test
//
//  Created by LARRYHOU on 2021/5/27.
//

#ifndef lz4test_hpp
#define lz4test_hpp

#define FILE_BLOCK_SIZE (32<<10)

#include <stdbool.h>

extern double lz4t_progress;

bool lz4t_decompress(const char* filename, const char *savename);
bool lz4t_compress(const char* filename, const char *savename);

#endif /* lz4test_hpp */
