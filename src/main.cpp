#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/printf.h>

#include "png.h"
#include "png_read.h"
#include "crc32.h"
#include "cpp_crc32.h"

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

        if( !pBasePng )
            errc = 1;
        else
        {
            auto pPng = readPNG( fmt::sprintf( inputPath, idx ) );
            while( pPng )
            {
                for( const auto& chk : pPng->getChunks() )
                {
                    std::cout << "CHUNKY" << std::endl;
                    std::cout << chk.info.type.ctype[0]
                              << chk.info.type.ctype[1]
                              << chk.info.type.ctype[2]
                              << chk.info.type.ctype[3]
                              << std::endl;

                    std::cout << "size: " << chk.data.size() << std::endl;

                    std::cout << "real crc32: " << std::to_string( chk.crc32 ) << std::endl;

                    std::vector< unsigned char > crcBufVec;
                    crcBufVec.push_back( chk.info.type.ctype[0] );
                    crcBufVec.push_back( chk.info.type.ctype[1] );
                    crcBufVec.push_back( chk.info.type.ctype[2] );
                    crcBufVec.push_back( chk.info.type.ctype[3] );
                    crcBufVec.insert( crcBufVec.end(), chk.data.begin(), chk.data.end() );

                    uint32_t crc32Val = crc( crcBufVec.begin(), crcBufVec.end() );
                    swapByteOrder( crc32Val );

                    std::cout << "calc crc32: " << crc32Val << std::endl;
                    if( crc32Val != chk.crc32 )
                        std::cout << "CRC32 mismatch!" << std::endl;
                }

                pPng = readPNG( fmt::sprintf( inputPath, idx ) );

                idx++;
            }
        }

        
        
    }

    return errc;
}