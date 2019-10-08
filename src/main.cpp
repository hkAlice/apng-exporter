#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/printf.h>

#include "png.h"
#include "png_read.h"

int main( int argc, char *argv[] )
{
    int errc = 0;
    std::string inputPath;
    std::string outputPath;

    std::cout << "apng-exporter" << std::endl;

    for( int i = 1; i < argc; i++ )
    {
        std::string arg = argv[i];

        if( arg == "-i" )
        {
            inputPath = argv[i + 1];
        }
        else
        {
            outputPath = argv[i];
        }
    }

    if( inputPath.empty() )
    {
        std::cout << "usage: apng-exporter -i {infile} {outfile}" << std::endl;
        std::cout << "example: apng-exporter -i test%03d.png exportedImage.png" << std::endl;

        errc = 2;
    }
    else
    {
        std::vector< std::string > testTmp;

        uint64_t idx = 1;
        bool hasNext = true;

        auto pBasePng = readPNG( fmt::sprintf( inputPath, idx ) );

        // todo: while pPng read
        idx++;
        
    }

    return errc;
}