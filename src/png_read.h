#ifndef _PNG_READ
#define _PNG_READ

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"
#include "byte_ops.h"

_png_CHUNK readNextChunk( std::ifstream& ifs )
{
    _png_CHUNK pngChk;
    _png_CHUNK_INFO pngChkInf;

    ifs.read( ( char* )&pngChkInf.len, sizeof( pngChkInf.len ) );
    ifs.read( ( char* )&pngChkInf.type, sizeof( pngChkInf.type ) );
    swapByteOrder( pngChkInf.len );

    pngChk.info = pngChkInf;

    std::vector< unsigned char > chkData( pngChkInf.len );
    ifs.read( ( char* )&chkData.data()[0], pngChkInf.len );

    pngChk.data = chkData;

    ifs.read( ( char* )&pngChk.crc, sizeof( pngChk.crc ) );

    return pngChk;
}

PNGPtr readPNG( const std::string& filePath )
{
	PNGPtr pPng = std::make_shared< PNG >();

	// Open binary file and verify if OK
    std::ifstream ifs( filePath.c_str(), std::ios::binary );

    bool isOpen = ifs.is_open() && !ifs.fail();

    if( !isOpen )
    {
        // Can't read file
        return nullptr;
    }

    // Verify PNG signature
    _png_HEADER pngHeader;
    ifs.read( (char*)&pngHeader, sizeof( _png_HEADER ) );

    if( memcmp( &pngHeader, PNG_SIG, sizeof( PNG_SIG ) ) != 0 )
    {
        // Not a valid PNG file
        return nullptr;
    }

	// We read the first chunk, which should be the header

    _png_CHUNK pngChk = readNextChunk( ifs );
    std::cout << "LEN: " << pngChk.info.len << std::endl;
    std::cout << "TYPE: " << pngChk.info.type[0]
              << pngChk.info.type[1]
              << pngChk.info.type[2]
              << pngChk.info.type[3]
              << std::endl;
/*
    if( pngChk.info.type != "IHDR" )
    {
        // First chunk is not IHDR
        return nullptr;
    }*/

    _png_CHUNK_IHDR* pngIHDR = reinterpret_cast< _png_CHUNK_IHDR* >( pngChk.data.data() );

    uint32_t pngWidth = c_ntohl( reinterpret_cast< uint8_t* >( &pngIHDR->width ) );
    std::cout << "TPNG: " << std::to_string( pngWidth ) << std::endl;

    // TODO: at the cost of possible collisions, compare base PNG IHDR with other frame IHDR only comparing CRC32

    return pPng;
}

#endif