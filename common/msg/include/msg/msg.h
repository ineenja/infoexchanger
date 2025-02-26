#ifndef MSG
#define MSG

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    uint32_t sizeBytes;


}   WavHeader;
#pragma pack(pop)

class Msg
{
    Msg(){
        WavHeader MyWavHeader;

    }


};

#endif
