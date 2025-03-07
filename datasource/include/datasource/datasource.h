#ifndef DATASOURCE
#define DATASOURCE

#include "msg/msg.h"
#include <chrono>
#include <thread>

class MessageGenerator {
    public:
    MessageGenerator(std::chrono::milliseconds interval, std::vector<Message>* messageQueuePointer)
        : interval_(interval), lastCreationTime_(std::chrono::steady_clock::now()), messagesQueuePointer(messageQueuePointer) {};

    template <typename T>
    void newMsg(const T& data, uint8_t msgType) {
        if (std::chrono::steady_clock::now() - lastCreationTime_ >= interval_) {
            std::cout << "msg created" << std::endl;
            lastCreationTime_ = std::chrono::steady_clock::now();
            Message msg(data, msgType);
            messagesQueuePointer->push_back(msg);
        }
    }

    private:
    std::chrono::milliseconds interval_;
    std::chrono::steady_clock::time_point lastCreationTime_;

    std::vector<Message>* messagesQueuePointer;

};

#endif
