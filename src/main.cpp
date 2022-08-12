#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <fmt/format.h>
#include <fmt/printf.h>

#include "png.h"
#include "png_read.h"
#include "png_write.h"
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

        uint64_t idx = 0;

        auto pBasePng = readPNG( fmt::sprintf( inputPath, idx ) );
        idx++;

        // todo: while pPng read

        if( pBasePng )
        {
            std::vector< _png_CHUNK > datChunks;

            for( const auto& chk : pBasePng->getChunks() )
            {
                std::cout << chk.info.type.ctype[0]
                            << chk.info.type.ctype[1]
                            << chk.info.type.ctype[2]
                            << chk.info.type.ctype[3]
                            << std::endl
                            << std::to_string( chk.info.type.itype )
                            << std::endl;

                std::cout << "size: " << chk.data.size() << std::endl;

                std::cout << "real crc32: " << std::to_string( chk.crc32 ) << std::endl;

                std::vector< unsigned char > crcBufVec( sizeof( _png_CHUNK_TYPE ) ) ;
                std::memcpy( &crcBufVec.data()[0], &chk.info.type, sizeof( _png_CHUNK_TYPE ) );
                crcBufVec.insert( crcBufVec.end(), chk.data.begin(), chk.data.end() );

                uint32_t crc32Val = crc( crcBufVec.begin(), crcBufVec.end() );
                swapByteOrder( crc32Val );

                std::cout << "calc crc32: " << crc32Val << std::endl;
                if( crc32Val != chk.crc32 )
                    std::cout << "CRC32 mismatch!" << std::endl;

                if( chk.info.type.itype == PNG_ITYPE::IDAT ) {
                    datChunks.emplace_back( chk );
                }
                if( chk.info.type.itype == PNG_ITYPE::fdAT ) {
                    _png_CHUNK transformChk;

                    auto newChkLen = chk.info.len;
                    swapByteOrder( newChkLen );
                    newChkLen -= 0x4;
                    swapByteOrder( newChkLen );

                    transformChk.info.len = newChkLen;
                    transformChk.info.type.itype = PNG_ITYPE::IDAT;
                    transformChk.data.insert( transformChk.data.begin(), chk.data.begin() + 4, chk.data.end() );
                    datChunks.push_back( transformChk );
                }
            }

            for( const auto& frame : datChunks ) {
                auto pInsertPNG = readPNG( fmt::sprintf( inputPath, idx ) );

                pInsertPNG->removeChunkAt( 1 );
                pInsertPNG->removeChunkAt( 1 );
                pInsertPNG->removeChunkAt( 1 );

                auto chunkCount = pInsertPNG->getChunks().size();

                // remove all chunks except for IEND
                for( int i = 0; i < chunkCount - 2; i++ ) {
                    pInsertPNG->removeChunkAt( 1 );
                }

                pInsertPNG->insertChunk( frame, 1 );

                writePNG( "testPNG" + std::to_string( idx ) + ".png", pInsertPNG );

                idx++;
            }
        }
    }

    return errc;
}