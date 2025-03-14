#ifndef SHAREDMEMMEMORYSUPPLIER
#define SHAREDMEMMEMORYSUPPLIER

#include "imemorysupplier/imemorysupplier.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

namespace Bip = boost::interprocess;

class SharedMemMemorySupplier : public IMemorySupplier {

    public:

    SharedMemMemorySupplier(const std::string& name, size_t size)
    : memName(name), memSize(size) {
        locateMemory();
    }

    ~SharedMemMemorySupplier() override {
        Bip::shared_memory_object::remove(memName.c_str());
    }

    [[nodiscard]] void* getMemory() const {
        return region.get_address();
    }

    [[nodiscard]] size_t getSize() const {
        return memSize;
    }

    void locateMemory() override {
        // Удаляем shared memory, если она уже существует
        Bip::shared_memory_object::remove(memName.c_str());

        // Создаем shared memory
        shm = Bip::shared_memory_object(Bip::create_only, memName.c_str(), Bip::read_write);

        // Устанавливаем размер shared memory
        shm.truncate(memSize);

        // Отображаем shared memory в адресное пространство процесса
        region = Bip::mapped_region(shm, Bip::read_write);

    }

private:

    std::string memName;
    size_t memSize;
    Bip::shared_memory_object shm;
    Bip::mapped_region region;

};



#endif