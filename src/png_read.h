#ifndef _PNG_READ
#define _PNG_READ

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"
#include "byte_ops.h"

bool readNextChunk( std::ifstream& ifs, _png_CHUNK& pngChk )
{
    _png_CHUNK_INFO pngChkInf;

    std::vector< char > chkLen;
    chkLen.resize( sizeof( pngChkInf.len ) );

    ifs.read( ( char* )&chkLen[ 0 ], sizeof( pngChkInf.len ) );
    memcpy( ( char* )&pngChkInf.len, ( char* )&chkLen[ 0 ], sizeof( pngChkInf.len ) );
    // TODO: I can't read
    if( ifs.eof() )
        return false;

    auto chkLength = pngChkInf.len;
    swapByteOrder( chkLength );

    std::vector< char > chkType;
    chkType.resize( sizeof( pngChkInf.type ) );

    ifs.read( ( char* )&chkType[ 0 ], sizeof( pngChkInf.type ) );
    memcpy( ( char* )&pngChkInf.type, ( char* )&chkType[ 0 ], sizeof( pngChkInf.type ) );

    pngChk.info = pngChkInf;

    std::vector< char > chkData( chkLength );
    ifs.read( ( char* )&chkData.data()[0], chkLength );

    pngChk.data = chkData;

    std::vector< char > chkCrc;
    chkCrc.resize( sizeof( pngChkInf.len ) );
    ifs.read( ( char* )&chkCrc[ 0 ], sizeof( pngChkInf.len ) );
    memcpy( ( char* )&pngChk.crc32, ( char* )&chkCrc[ 0 ], sizeof( char ) * chkCrc.size() );

    std::cout << pngChk.info.type.ctype[0]
                              << pngChk.info.type.ctype[1]
                              << pngChk.info.type.ctype[2]
                              << pngChk.info.type.ctype[3]
                              << std::endl;

    return true;
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

    if( std::memcmp( &pngHeader, PNG_SIG, sizeof( PNG_SIG ) ) != 0 )
    {
        // Not a valid PNG file
        return nullptr;
    }

	// We read the first chunk, which should be the header

    _png_CHUNK pngChkIHDR;
    bool ihdrReadOk = readNextChunk( ifs, pngChkIHDR );

    if( !ihdrReadOk )
        return nullptr; 

    std::cout << "LEN: " << pngChkIHDR.info.len << std::endl;
    std::cout << "TYPE: " << pngChkIHDR.info.type.ctype[0]
              << pngChkIHDR.info.type.ctype[1]
              << pngChkIHDR.info.type.ctype[2]
              << pngChkIHDR.info.type.ctype[3]
              << std::endl;

    if( pngChkIHDR.info.type.itype != PNG_ITYPE::IHDR ) // todo: move int literal ("IHDR") to static
    {
        // First chunk is not IHDR
        return nullptr;
    }

    pPng->addChunk( pngChkIHDR );

    _png_CHUNK_IHDR* pngIHDR = reinterpret_cast< _png_CHUNK_IHDR* >( pngChkIHDR.data.data() );
    pPng->setIHDR( pngIHDR );
    pPng->setIHDRCRC32( pngChkIHDR.crc32 );

    bool pngEof = !ifs;

    while( !pngEof )
    {
        _png_CHUNK nextPngChk;
        bool chunkReadOk = readNextChunk( ifs, nextPngChk );

        if( chunkReadOk )
            pPng->addChunk( nextPngChk );

        pngEof = !ifs || nextPngChk.info.type.itype == PNG_ITYPE::IEND;
    }

    // TODO: at the cost of possible collisions, compare base PNG IHDR with other frame IHDR only comparing CRC32

    return pPng;
}

#endif