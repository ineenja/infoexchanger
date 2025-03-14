#ifndef IMEMORYSUPPLIER
#define IMEMORYSUPPLIER

#include <cstdint>

class IMemorySupplier {

public:
    virtual ~IMemorySupplier() = default;

    virtual void locateMemory(uint32_t size) = 0;

protected:
    uint32_t size;
    uint8_t* start;
    uint8_t* end;

};



#endif