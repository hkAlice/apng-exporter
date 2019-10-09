#ifndef _PNG
#define _PNG

#include <memory>

static uint8_t PNG_SIG[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

struct _png_HEADER
{
    char signature[8];
};

struct _png_CHUNK_INFO
{
    uint32_t len;
    char type[4];
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
    uint32_t crc;
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

    void setIHDR( const _png_CHUNK_IHDR& ihdr )
    {
        m_ihdr = ihdr;
    }

    void addChunk( const _png_CHUNK& chunk )
    {
        m_chunks.push_back( chunk );
    }

private:

    _png_CHUNK_IHDR m_ihdr;
    std::vector< _png_CHUNK > m_chunks;


};

using PNGPtr = std::shared_ptr< PNG >;

#endif