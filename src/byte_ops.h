#ifndef _BYTE_OPS
#define _BYTE_OPS

uint32_t c_ntohl( uint8_t* pInt )
{
    return ( pInt[0] << 24 ) | ( pInt[1] << 16 ) | ( pInt[2] << 8 ) | pInt[3];
}

void swapByteOrder( uint32_t& val )
{
    val = c_ntohl( reinterpret_cast< uint8_t* >( &val ) );
}

#endif