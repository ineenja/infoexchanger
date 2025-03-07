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
    void newMsg(const T& data) { // функция генерации сообщений по одному в интервал времени
        auto StartTime = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastCreationTime_ >= interval_) { // проверяет, прошел ли интервал c момента последнего создания сообщения
            lastCreationTime_ = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего создания сообщения
            Message msg(data);
            messagesQueuePointer->push_back(msg); // создаем сообщение и пушим его в очередь

            auto EndTime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime);
            std::cout << diff.count() << " ns" << std::endl;
            timeToCreateAMessage.push_back(diff);
        }
    }

    template <typename T>
    void newMsg(const T& data, uint8_t dataType) { // перегрузка функции создания нового сообщения с возможностью передавать тип сообщения
        auto StartTime = std::chrono::steady_clock::now();
        if (std::chrono::steady_clock::now() - lastCreationTime_ >= interval_) { // проверяет, прошел ли интервал c момента последнего создания сообщения
            lastCreationTime_ = std::chrono::steady_clock::now(); // обновляем переменную хранящую время последнего создания сообщения
            Message msg(data, dataType);
            messagesQueuePointer->push_back(msg); // создаем сообщение и пушим его в очередь

            auto EndTime = std::chrono::steady_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime);
            std::cout << diff.count() << " ns" << std::endl;
            timeToCreateAMessage.push_back(diff);
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

    std::vector<Message>* messagesQueuePointer; // указатель на очередь
    std::vector<std::chrono::nanoseconds> timeToCreateAMessage;
};

#endif
