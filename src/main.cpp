#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/printf.h>

#include "png.h"

uint32_t c_ntohl( uint8_t* pInt )
{
    return ( pInt[0] << 24 ) | ( pInt[1] << 16 ) | ( pInt[2] << 8 ) | pInt[3];
}

void swapOrder( uint32_t& val )
{
    val = c_ntohl( reinterpret_cast< uint8_t* >( &val ) );
}

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
        while( hasNext )
        {
            std::string filePath = fmt::sprintf( inputPath, idx );
            std::ifstream ifs( filePath.c_str(), std::ios::binary );

            hasNext = ifs.is_open();
            if( hasNext )
            {
                std::cout << "Loading " << filePath << std::endl;

                _png_HEADER pngHeader;
                ifs.read( (char*)&pngHeader, sizeof( _png_HEADER ) );

                //std::vector< char > pngBuffer( std::istreambuf_iterator< char >( ifs ), {} );

                if( memcmp( &pngHeader, PNG_SIG, 8 ) )
                {
                    errc = 1;
                    break;
                }
                    // PNG signature detected, we can copy vals
                uint32_t offset = 0;

                _png_CHUNK_INFO pngChkInf;
                ifs.read( ( char* )&pngChkInf, sizeof( _png_CHUNK_INFO ) );
                pngChkInf.len = c_ntohl( reinterpret_cast< uint8_t* >( &pngChkInf.len ) );
                //swapOrder( pngChkInf.len );

                std::cout << "LEN: " << pngChkInf.len << std::endl;
                std::cout << "TYPE: " << pngChkInf.type[0]
                    << pngChkInf.type[1]
                    << pngChkInf.type[2]
                    << pngChkInf.type[3]
                    << std::endl;

                if( pngChkInf.type == "IHDR" )
                {
                    std::cerr << "PNG " << filePath << " failed: first chunk not IHDR" << std::endl;
                    errc = 1;
                    break;
                }
                
                _png_CHUNK_IHDR pngChkIHDR;
                ifs.read( (char*)&pngChkIHDR, sizeof( _png_CHUNK_IHDR ) );

                swapOrder( pngChkIHDR.width );
                swapOrder( pngChkIHDR.height );

                std::cout << "W: " << std::to_string( pngChkIHDR.width )<< std::endl;
                std::cout << "H: " << std::to_string( pngChkIHDR.height )<< std::endl;
                std::cout << "bpp: " << std::to_string( pngChkIHDR.bpp ) << std::endl;
                std::cout << "color_space: " << std::to_string( pngChkIHDR.color_space ) << std::endl;
                std::cout << "compression: " << std::to_string( pngChkIHDR.compression ) << std::endl;
                std::cout << "filter: " << std::to_string( pngChkIHDR.filter ) << std::endl;
                std::cout << "interlace: " << std::to_string( pngChkIHDR.interlace ) << std::endl;



            }

            idx++;
        }
        
    }

    return errc;
}