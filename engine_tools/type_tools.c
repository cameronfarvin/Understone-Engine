#include <engine_tools/type_tools.h>

void uDetermineSystemEndianness()
{
    uEndiannessUnion.dword = 1;
    if (uEndiannessUnion.word[0])
    {
        uSystemEndianness = uLITTLE_ENDIAN;
        return;
    }

    uSystemEndianness = uBIG_ENDIAN;
}
