#pragma once
#define  w_char wchar_t

char * g_buffer = NULL;
int g_size = 0;

static inline int
wstrlen(const w_char *str)
{
    const w_char *eos = str;
    while( *eos++ ) ;
    return( (int)(eos - str - 1) );
}

static const char *
convert_back_internal(const w_char *utf16, int len)
{
    unsigned char *ptr;
    g_buffer=(char*)realloc (g_buffer,1);
    g_buffer=(char*)realloc (g_buffer,(len+1)*3);

    ptr=(unsigned char*)g_buffer;
    g_size=0;

    while (*utf16 && len-- > 0) {
        w_char c=*utf16;
        if (c<=0x7F) {
            *ptr=(unsigned char)c;
        }
        else {
            if (c<=0x7ff) {
                *ptr=(unsigned char)((c>>6) | 0xc0);
            }
            else {
                *ptr=(unsigned char)((c>>12) | 0xe0);
                ++ptr;
                ++g_size;
                *ptr=(unsigned char)(((c>>6) & 0x3f) | 0x80);
            }
            ++ptr;
            ++g_size;
            *ptr=(unsigned char)((c&0x3f) | 0x80);
        }
        ++ptr;
        ++utf16;
        ++g_size;
    }
    *ptr=0;
    return g_buffer;
}

const char * 
utf_convert_back(const w_char * utf16)
{
    int len=wstrlen(utf16);
    return convert_back_internal(utf16, len);
}