#ifndef BUFFERMANAGER
#define BUFFERMANAGER

#include "sharedmemmemorysupplier/sharedmemmemorysupplier.h"
#include <cmath>


namespace Bip = boost::interprocess;

class BufferManager {
public:
    BufferManager(const std::string& shmName, size_t shmSize, size_t pocketDataSize)
        : memorySupply(shmName, shmSize), mutex(Bip::open_or_create, "mtx"), pocketDataSize(pocketDataSize) {
        idPocket = 0;
        pocketHeaderSize = 5;
        pocketSize = pocketDataSize + pocketHeaderSize;
    }

    void writePocket(const std::vector<uint8_t>& data, size_t position) {
        if (pocketDataSize * position + data.size() > memorySupply.getSize()) {
            throw std::out_of_range("Write operation exceeds shared memory size");
        }
        //Bip::scoped_lock<Bip::named_mutex> lock(mutex);

        std::vector<std::vector<uint8_t>> pocketsBuffer = createPockets(data);

        void* mem = memorySupply.getMemory();
        for (std::vector<uint8_t> pocket: pocketsBuffer) {
            std::memcpy(static_cast<uint8_t*>(mem) + pocketSize * position, pocket.data(), (pocketSize)*sizeof(uint8_t));
            position++;
        }
    }

    std::vector<uint8_t> readPocket(size_t position) {

        //Bip::scoped_lock<Bip::named_mutex> lock(mutex);
        std::vector<uint8_t> dataFromPockets;

        void* mem = memorySupply.getMemory();

        std::vector<uint8_t> buffer(pocketDataSize + pocketHeaderSize);
        std::memcpy(buffer.data(), static_cast<uint8_t*>(mem) + pocketSize * position, (pocketSize)*sizeof(uint8_t));

        // if (buffer[0] == 228) {
        //     for (size_t i = 0; i < pocketDataSize; i++) {
        //         dataFromPockets.push_back(buffer[i + pocketHeaderSize]);
        //     }
        // } else {
        //     throw std::out_of_range("Invalid read operation - no message on this position");
        // }
        bool keepReadingAMessage = true;
        uint8_t currentMessageID = buffer[1];
        uint8_t currentMessagePocketsNumber = buffer[3];

        while (keepReadingAMessage) {
            std::cout << "starting reading a message" << std::endl;
            if (buffer[0] == 228 or buffer[2] == 1) { // проверка, является ли выгруженное пакетом, и первый ли это пакет из сообщения
                std::cout << "looks like this pocket is OK" << std::endl;
                if (buffer[2] <= currentMessagePocketsNumber and buffer[1] == currentMessageID) { // проверка, не перескочили ли мы на следующее сообщение в памяти
                    std::cout << "starting to read pocket #" << static_cast<int>(buffer[2]) << " from " << static_cast<int>(buffer[3]) << " of msg " << static_cast<int>(buffer[1]) << std::endl;
                    for (size_t i = 0; i < buffer[4]; ++i) { // копируем байты информации из текущего пакета в формируемое сообщение на выход
                        std::cout << "currently reading byte #" << i+1 << " from " << static_cast<int>(buffer[4]) << " which holds value = "<< static_cast<int>(buffer[i + pocketHeaderSize]) << std::endl;
                        dataFromPockets.push_back(buffer[i + pocketHeaderSize]);
                    }
                    std::cout << "done reading pocket #" << static_cast<int>(buffer[2]) << " from " << static_cast<int>(buffer[3]) << " of msg " << static_cast<int>(buffer[1]) << std::endl;
                    position++; // переходим на следующий пакет
                    if (pocketSize * position < memorySupply.getSize()) { // проверка, не выйдем ли мы за пределы памяти на след. пакете
                        std::memcpy(buffer.data(), static_cast<uint8_t*>(mem) + pocketSize * position, pocketSize*sizeof(uint8_t));
                        std::cout << "copied next pocket" << std::endl;
                    } else {
                        std::cout << "reached shared memo end here!" << pocketSize * position << " > " << memorySupply.getSize() << std::endl;
                        keepReadingAMessage = false;
                    }
                } else {
                    std::cout << "looks like current message is over" << std::endl;
                    keepReadingAMessage = false;
                }
            } else {
                std::cout << "wrong ass position" << std::endl;
                keepReadingAMessage = false; // если это не пакет или это не первый пакет из сообщения то читать не продолжаем
            }
            std::cout << std::endl;
        }

        return dataFromPockets;
    }

    std::vector<std::vector<uint8_t>> createPockets(const std::vector<uint8_t>& data) {
        std::vector<std::vector<uint8_t>> pockets;
        ++idPocket;

        uint8_t pocketsNumber = (uint8_t)ceil((double)data.size() / (double)pocketDataSize);

        uint8_t numberOfPocket = 0;
        for (size_t i = 0; i < data.size(); i++) {
            numberOfPocket++;
            std::vector<uint8_t> newPocket(pocketDataSize+pocketHeaderSize, 0); // размер вектора байт - пакета составляет pocketSize в байтах плюс 2 байта метаинформации
            newPocket[0] = 228; // кодовое число чтобы было понятно, что это - пакет
            newPocket[1] = idPocket; // pocket ID
            newPocket[2] = numberOfPocket;; // number of pockets for particular pocket ID
            newPocket[3] = pocketsNumber;; // pocketNo for particular pocket ID
            newPocket[4] = 0;
            for (size_t j = 0; j < pocketDataSize; j++) {
                if (i < data.size()) {
                    newPocket[j+pocketHeaderSize] = data[i++];
                    newPocket[4] = newPocket[4] + 1;
                } else {
                    newPocket[j+pocketHeaderSize] = 0;
                }
            }
            i--;
            pockets.push_back(newPocket);
        }
        return pockets;
    }

private:
    uint8_t idPocket;

    SharedMemMemorySupplier memorySupply;
    Bip::named_mutex mutex;
    size_t pocketDataSize;
    size_t pocketHeaderSize;
    size_t pocketSize;
};



#endif