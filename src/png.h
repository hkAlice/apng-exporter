#ifndef _PNG
#define _PNG

#include <memory>
#include "byte_ops.h"
#include "cpp_crc32.h"

static uint8_t PNG_SIG[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

enum PNG_ITYPE
{
    IHDR = 1380206665,
    IDAT = 1413563465,
    IEND = 1145980233,
    fdAT = 1413571686,
    fcTL = 1280598886,
    acTL = 1280598881
};

struct _png_HEADER
{
    char signature[8];
};

union _png_CHUNK_TYPE
{
    uint32_t itype;
    char ctype[4];
};

struct _png_CHUNK_INFO
{
    uint32_t len;
    _png_CHUNK_TYPE type;
};

struct _png_CHUNK_IHDR
{
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t color_space;
    uint8_t compression;
    uint8_t filter;
    uint8_t interlace;
};

struct _png_CHUNK
{
    _png_CHUNK_INFO info;
    std::vector< char > data;
    uint32_t crc32;
};

class PNG
{

public:
    PNG()
    {

    };
    ~PNG() = default;

    PNG( const PNG& png ) = delete;
    PNG& operator=( const PNG& png ) = delete;

    void setIHDR( _png_CHUNK_IHDR* ihdr )
    {
        m_ihdr = ihdr;
    }

    void setIHDRCRC32( const uint32_t crc32 )
    {
        m_ihdrCRC32 = crc32;
    }

    const uint32_t getIHDRCRC32() const
    {
        return m_ihdrCRC32;
    }

    _png_CHUNK_IHDR* getIHDR() const
    {
        return m_ihdr;
    }

    void addChunk( const _png_CHUNK& chunk )
    {
        m_chunks.push_back( chunk );
    }

    void insertChunk( const _png_CHUNK& chunk, int idx ) {
        m_chunks.insert( m_chunks.begin() + idx, chunk );
    }
    
    void removeChunkAt( int idx ) {
        m_chunks.erase( m_chunks.begin() + idx );
    }
    
    const std::vector< _png_CHUNK >& getChunks() const
    {
        return m_chunks;
    }

    static uint32_t calculateChunkCRC32( const _png_CHUNK& chk ) {
        std::vector< unsigned char > crcBufVec( sizeof( _png_CHUNK_TYPE ) );
        std::memcpy( &crcBufVec.data()[ 0 ], &chk.info.type, sizeof( _png_CHUNK_TYPE ) );
        crcBufVec.insert( crcBufVec.end(), chk.data.begin(), chk.data.end() );

        uint32_t crc32Val = crc( crcBufVec.begin(), crcBufVec.end() );
        swapByteOrder( crc32Val );

        return crc32Val;
    }

    std::vector< char > getBinaryData() {
        std::vector< char > rawData;
        std::copy( PNG_SIG, PNG_SIG + 8, std::back_inserter( rawData ) );

        for( const auto& chk : m_chunks ) {
            std::vector< unsigned char > bufVec( sizeof( _png_CHUNK_INFO ) );
            std::memcpy( &bufVec.data()[ 0 ], &chk.info, sizeof( _png_CHUNK_INFO ) );
            bufVec.insert( bufVec.end(), chk.data.begin(), chk.data.end() );

            auto crcVal = calculateChunkCRC32( chk );
            bufVec.insert( bufVec.end(), ( char* )&crcVal, ( char* )&crcVal + sizeof( crcVal ) );

            rawData.insert( rawData.end(), bufVec.begin(), bufVec.end() );
        }

        return rawData;
    }

private:

    _png_CHUNK_IHDR* m_ihdr;
    uint32_t m_ihdrCRC32;
    std::vector< _png_CHUNK > m_chunks;

};

using PNGPtr = std::shared_ptr< PNG >;

#endif