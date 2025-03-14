#ifndef IMEMORYSUPPLIER
#define IMEMORYSUPPLIER

#include <vector>
#include <iostream>
#include <cstring>

class IMemorySupplier {

public:

    virtual ~IMemorySupplier() = default;

    virtual void locateMemory() = 0;

protected:
};



#endif