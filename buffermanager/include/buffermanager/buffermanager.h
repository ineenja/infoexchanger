#ifndef BUFFERMANAGER
#define BUFFERMANAGER

#include "sharedmemmemorysupplier/sharedmemmemorysupplier.h"

namespace Bip = boost::interprocess;

class BufferManager {
public:
    BufferManager(const std::string& shmName, size_t shmSize, size_t offset)
        : memorySupply(shmName, shmSize), mutex(Bip::open_or_create, "mtx"), offset(offset) {
        idPocket = 0;
    }

    void writePocket(const std::vector<uint8_t>& data, size_t position, size_t pocketSize) {
        if (offset * position + data.size() > memorySupply.getSize()) {
            throw std::out_of_range("Write operation exceeds shared memory size");
        }

        Bip::scoped_lock<Bip::named_mutex> lock(mutex);

        void* mem = memorySupply.getMemory();
        std::memcpy(static_cast<uint8_t*>(mem) + offset * position, data.data(), data.size());
    }

    std::vector<uint8_t> readPocket(size_t size, size_t position, size_t pocketSize) {
        if (offset * position + size > memorySupply.getSize()) {
            throw std::out_of_range("Read operation exceeds shared memory size");
        }

        Bip::scoped_lock<Bip::named_mutex> lock(mutex);

        void* mem = memorySupply.getMemory();
        std::vector<uint8_t> buffer(size);
        std::memcpy(buffer.data(), static_cast<uint8_t*>(mem) + offset * position, size);
        return buffer;
    }

    std::vector<std::vector<uint8_t>> createPockets(const std::vector<uint8_t>& data, size_t pocketSize) {
        std::vector<std::vector<uint8_t>> pockets;
        ++idPocket;

        uint8_t numberOfPocket = 0;
        for (size_t i = 0; i < data.size(); i++) {
            std::vector<uint8_t> newPocket(pocketSize+2, 0); // размер вектора байт - пакета составляет pocketSize в байтах плюс 2 байта метаинформации
            for (size_t j = 0; j < pocketSize; j++) {
                newPocket[0] = idPocket; // pocket ID
                newPocket[1] = numberOfPocket;; // pocketNo for particular pocket ID
                newPocket[j+2] = data[i++];
            }
            i--;
            numberOfPocket++;
            pockets.push_back(newPocket);
        }
        return pockets;
    }

private:
    uint8_t idPocket;

    SharedMemMemorySupplier memorySupply;
    Bip::named_mutex mutex;
    size_t offset;
};



#endif