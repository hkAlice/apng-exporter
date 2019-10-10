#ifndef _PNG_READ
#define _PNG_READ

// from: https://stackoverflow.com/a/25195711

constexpr bool is_digit( char c )
{
    return c <= '9' && c >= '0';
}

constexpr int constexpr_stoi( const char* str, int value = 0 )
{
    return *str ?
            is_digit(*str) ?
                stoi_impl(str + 1, (*str - '0') + value * 10)
                : throw "compile-time-error: not a digit"
            : value;
}

#endif