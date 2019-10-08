#ifndef _PNG
#define _PNG

static char PNG_SIG[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };

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

struct _png_CHUNK_IDAT
{
    
};

struct _png_CHUNK
{
    _png_CHUNK_INFO info;
    void* chunk;
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


};

#endif