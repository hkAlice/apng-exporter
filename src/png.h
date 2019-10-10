#ifndef _PNG
#define _PNG

#include <memory>

static uint8_t PNG_SIG[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

enum PNG_ITYPE
{
    IHDR = 1380206665,
    IDAT = 1413563465,
    IEND = 1145980233
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
    std::vector< unsigned char > data;
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

    const std::vector< _png_CHUNK >& getChunks() const
    {
        return m_chunks;
    }


private:

    _png_CHUNK_IHDR* m_ihdr;
    uint32_t m_ihdrCRC32;
    std::vector< _png_CHUNK > m_chunks;

};

using PNGPtr = std::shared_ptr< PNG >;

#endif