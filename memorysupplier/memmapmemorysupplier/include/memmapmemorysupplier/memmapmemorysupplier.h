#ifndef MEMMAPMEMORYSUPPLIER
#define MEMMAPMEMORYSUPPLIER



class MemMapMemorySupplier {

public:
    virtual ~IMemorySupplier();

private:
    uint32_t size;
    uint8_t* start;
    uint8_t* end;
    
};



#endif