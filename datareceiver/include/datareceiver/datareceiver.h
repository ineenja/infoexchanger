#ifndef DATASOURCE
#define DATASOURCE

#include "msg/msg.h"
#include "mylist/mylist.h"
#include <chrono>
#include <thread>
#include "buffermanager/buffermanager.h"

class DataReceiver {
    public:
        DataReceiver(SharedMemMemorySupplier* sharedMemoPtr, std::chrono::milliseconds interval, std::vector<Message>* messagesQueuePtr, size_t pocketDataSize) :
    messagesQueuePtr(messagesQueuePtr), pullingInterval(interval), lastPullingTime(std::chrono::steady_clock::now()),
    pocketDataSize(pocketDataSize), bufferManager(sharedMemoPtr, pocketDataSize), sharedMemoPtr(sharedMemoPtr) {
            currentPosition = 0;
            pocketHeaderSize = 5;
        };

        bool tryPullMsg() {
            std::vector<uint8_t> tempSequencedMessage;
            if (std::chrono::steady_clock::now() - lastPullingTime >= pullingInterval) { // проверяет, прошел ли интервал c момента последнего вытаскивания сообщения
                lastPullingTime = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего вытаскивания сообщения

                tempSequencedMessage = bufferManager.readPocket(currentPosition);

                if (tempSequencedMessage.empty()) {
                    return false;
                } else {
                    std::cout << "msg pulled from pos " << currentPosition << std::endl;
                    currentPosition = (uint8_t)ceil((double)tempSequencedMessage.size() / (double)pocketDataSize) + currentPosition;
                    messagesQueuePtr->push_back(turnBytesIntoMessage(tempSequencedMessage.data()));
                    return true;
                }
            } else {
                return false;
            }
        }

    private:
        std::vector<Message>* messagesQueuePtr;
        SharedMemMemorySupplier* sharedMemoPtr;

        size_t currentPosition;
        size_t pocketDataSize;
        size_t pocketHeaderSize;
        BufferManager bufferManager;

        std::chrono::milliseconds pullingInterval;
        std::chrono::steady_clock::time_point lastPullingTime;

};

#endif