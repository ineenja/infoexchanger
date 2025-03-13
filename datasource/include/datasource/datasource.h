#ifndef DATASOURCE
#define DATASOURCE

#include "msg/msg.h"
#include "mylist/mylist.h"
#include <chrono>
#include <thread>

class MessageGenerator {
    public:
    MessageGenerator(std::chrono::milliseconds interval, MyList<Message>* messageQueuePointer)
        : interval_(interval), lastCreationTime_(std::chrono::steady_clock::now()), messagesQueuePointer(messageQueuePointer) {};

    template <typename T>
    bool newMsg(const T& data) { // функция генерации сообщений по одному в интервал времени
        auto StartTime = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastCreationTime_ >= interval_) { // проверяет, прошел ли интервал c момента последнего создания сообщения
            lastCreationTime_ = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего создания сообщения
            Message msg(data);
            messagesQueuePointer->pushBack(msg); // создаем сообщение и пушим его в очередь
            if (messagesQueuePointer->getSize() == queueSize){
                messagesQueuePointer->popFront();
            }

            auto EndTime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime);
            std::cout << diff.count() << " ns" << std::endl;
            timeToCreateAMessage.push_back(diff);
            return true;
        } else {
            return false;
        }
    }

    template <typename T>
    bool newMsg(const T& data, uint8_t dataType) { // перегрузка функции создания нового сообщения с возможностью передавать тип сообщения
        auto StartTime = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastCreationTime_ >= interval_) { // проверяет, прошел ли интервал c момента последнего создания сообщения
            lastCreationTime_ = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего создания сообщения
            Message msg(data, dataType);
            messagesQueuePointer->pushBack(msg); // создаем сообщение и пушим его в очередь
            if (messagesQueuePointer->getSize() > queueSize){
                messagesQueuePointer->popFront();
            }

            auto EndTime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime);
            std::cout << "message " << msg.getMessageID() << " took " << diff.count() << " ns" << " to create" << std::endl;
            timeToCreateAMessage.push_back(diff);
            return true;
        } else {
            return false;
        }
    }

    std::chrono::nanoseconds getMeanTimeToCreateAMsg() {
        for (int i = 1; i < timeToCreateAMessage.size(); ++i) {
            timeToCreateAMessage[0] = timeToCreateAMessage[0] + timeToCreateAMessage[i];
        }
        return timeToCreateAMessage[0]/timeToCreateAMessage.size();
    }

    private:
    std::chrono::milliseconds interval_;
    std::chrono::steady_clock::time_point lastCreationTime_;
    uint8_t queueSize = 5;

    MyList<Message>* messagesQueuePointer; // указатель на очередь
    std::vector<std::chrono::nanoseconds> timeToCreateAMessage;
};

class MessageTransferer {
    public:
    MessageTransferer(std::chrono::milliseconds interval, MyList<Message>* messageQueuePointer, uint8_t* sharedMemoryPtr, uint8_t sharedMemorySize, uint8_t sharedMemorySlotSize)
        : interval_(interval), lastPullingTime_(std::chrono::steady_clock::now()), messagesQueuePointer(messageQueuePointer),
    sharedMemoryPtr(sharedMemoryPtr), sharedMemorySize(sharedMemorySize), sharedMemorySlotSize(sharedMemorySlotSize) {};

    bool tryPullMsg() {
        if (std::chrono::steady_clock::now() - lastPullingTime_ >= interval_) { // проверяет, прошел ли интервал c момента последнего вытаскивания сообщения
            lastPullingTime_ = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего вытаскивания сообщения

            std::vector<uint8_t> tempMessageSequenced;
            if (messagesQueuePointer->getSize() > 0) {
                tempMessageSequenced = divideMessageIntoBytes(messagesQueuePointer->front()); // секвентируем первое сообщение в очереди, записывая во временный вектор байт
                std::cout << "size of pulled msg, in bytes: " << tempMessageSequenced.size() << std::endl;
                messagesQueuePointer->popFront(); // удаляем его из очереди

                if (sharedMemoryPtr + sharedMemorySize > sharedMemoryPtr + sharedMemorySlotSize){ // чтобы предотвратить ошибку сегментации (VALGRIND MEMCHECK INVALID WRITE)
                    memcpy(sharedMemoryPtr, tempMessageSequenced.data(), tempMessageSequenced.size()); // загружаем в общую память байты сообщения
                    sharedMemoryPtr += sharedMemorySlotSize;
                } else {
                    std::cout << "memory leak prevented, msg not pulled" << std::endl;
                    return false;
                }
            } else {
                std::cout << "queue is empty rn" << std::endl;
                return false;
            }

            return true;
        } else {
            return false;
        }
    }

    private:
    std::chrono::milliseconds interval_;
    std::chrono::steady_clock::time_point lastPullingTime_;
    MyList<Message>* messagesQueuePointer; // указатель на очередь

    uint8_t* sharedMemoryPtr;
    uint8_t sharedMemorySize;
    uint8_t sharedMemorySlotSize;
};

#endif