#ifndef IMEMORYSUPPLIER
#define IMEMORYSUPPLIER


class IMemorySupplier {

public:
    virtual ~IMemorySupplier();

private:
    uint32_t size;
    uint8_t* start;
    uint8_t* end;

};



#endif