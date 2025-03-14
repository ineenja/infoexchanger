#ifndef BUFFERMANAGER
#define BUFFERMANAGER

#include "sharedmemmemorysupplier/sharedmemmemorysupplier.h"

namespace Bip = boost::interprocess;

class BufferManager {
public:
    BufferManager(const std::string& shmName, size_t shmSize, size_t offset)
        : memorySupply(shmName, shmSize), mutex(Bip::open_or_create, "mtx"), offset(offset) {}

    void writePocket(const std::vector<uint8_t>& data, size_t position) {
        if (offset * position + data.size() > memorySupply.getSize()) {
            throw std::out_of_range("Write operation exceeds shared memory size");
        }

        Bip::scoped_lock<Bip::named_mutex> lock(mutex);
        void* mem = memorySupply.getMemory();
        std::memcpy(static_cast<uint8_t*>(mem) + offset * position, data.data(), data.size());
    }

    std::vector<uint8_t> readPocket(size_t size, size_t position) {
        if (offset * position + size > memorySupply.getSize()) {
            throw std::out_of_range("Read operation exceeds shared memory size");
        }

        Bip::scoped_lock<Bip::named_mutex> lock(mutex);
        void* mem = memorySupply.getMemory();
        std::vector<uint8_t> buffer(size);
        std::memcpy(buffer.data(), static_cast<uint8_t*>(mem) + offset * position, size);
        return buffer;
    }

private:
    SharedMemMemorySupplier memorySupply;
    Bip::named_mutex mutex;
    size_t offset;
};



#endif