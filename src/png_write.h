#ifndef _PNG_WRITE
#define _PNG_WRITE

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "png.h"
#include "byte_ops.h"

bool writePNG( const std::string& filename, const PNGPtr pPng ) {
    std::ofstream of( filename, std::ios::out | std::ios::binary );
    if( !of ) {
        return false;
    }

    auto rawData = pPng->getBinaryData();

    of.write( reinterpret_cast< const char* >( &rawData[ 0 ] ), rawData.size() * sizeof( char ) );
        
    of.close();
}
#endif