//
//  main.cpp
//  lz4test
//
//  Created by LARRYHOU on 2021/5/27.
//

#include "lz4test.hpp"
#include <string>

int main(int argc, const char * argv[])
{
    if (argc < 2 + 2) { return 1; }
    
    auto command = argv[1];
    if (!strcmp(command, "compress"))
    {
        if (!lz4t_compress(argv[2], argv[3])) { return 10; }
    }
    else if (!strcmp(command, "decompress"))
    {
        if (!lz4t_decompress(argv[2], argv[3])) { return 20; }
    }
    
    return 0;
}


