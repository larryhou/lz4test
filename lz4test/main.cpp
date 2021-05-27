//
//  main.cpp
//  lz4test
//
//  Created by LARRYHOU on 2021/5/27.
//

#include "lz4test.hpp"
#include <fstream>

int main(int argc, const char * argv[])
{
    if (argc < 2 + 2) { return 1; }
    
    auto command = argv[1];
    
    std::fstream input;
    input.open(argv[2], std::ios::in);
    if (!input.good()) { return 2; }
    
    std::fstream output;
    output.open(argv[3], std::ios::out);
    if (!output.good()) { return 4; }
    
    if (!strcmp(command, "compress"))
    {
        if (!lz4test::compress(input, output)) { return 10; }
    }
    else if (!strcmp(command, "decompress"))
    {
        if (!lz4test::decompress(input, output)) { return 20; }
    }
    
    return 0;
}


