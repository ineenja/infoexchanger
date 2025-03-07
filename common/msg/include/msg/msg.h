#ifndef MSG
#define MSG

#include <processing/processing.h>

class Message
{
public:

    template <typename T>
    explicit Message(const T& data){
        messageID = ++idCounter; // инкремент счетчика сообщений

        if constexpr (std::is_pointer_v<T>) {
            std::cout << "calling a constructor for pointer" << std::endl;
            messageType = getDataType(*data); // определяем тип сообщения
            payload = divideDataIntoBytes(*data); // переводим сообщение в последовательность байт
        } else {
            std::cout << "calling a constructor for data" << std::endl;
            messageType = getDataType(data); // определяем тип сообщения
            payload = divideDataIntoBytes(data); // переводим сообщение в последовательность байт
        }
        /////////////////
        /// на данный момент есть 5 реализованных вариантов сообщений
        /// 1 - vector элементов типа double
        /// 2 - vector элементов типа int
        /// 3 - переменная типа double
        /// 4 - переменная типа int
        /// 5 - string
        //////////////////

        payloadHash = getDataHash(payload); // хешируем отправляемые байты
        payloadSize = payload.size();
    }

    template <typename T>
    explicit Message(const T& data, uint8_t dataType){ // перегрузка функции с возможностью передать тип сообщения
        if constexpr (std::is_pointer_v<T>) {
            std::cout << "calling a constructor for pointer" << std::endl;
            payload = divideDataIntoBytes(*data); // переводим сообщение в последовательность байт
        } else {
            std::cout << "calling a constructor for data" << std::endl;
            payload = divideDataIntoBytes(data); // переводим сообщение в последовательность байт
        }
        messageID = ++idCounter; // инкремент счетчика сообщений
        messageType = dataType; // определяем тип сообщения
        payloadHash = getDataHash(payload); // хешируем отправляемые байты
        payloadSize = payload.size();
    }

    [[nodiscard]] uint32_t getMessageID() const {
        return messageID;
    }

    [[nodiscard]] uint8_t getMessageType() const {
        return messageType;
    }

    [[nodiscard]] uint32_t getPayloadSize() const {
        return payloadSize;
    }

    [[nodiscard]] uint32_t getPayloadHash() const {
        return payloadHash;
    }

    [[nodiscard]] std::vector<uint8_t> getPayload() const {
        return payload;
    }

private:
    static uint32_t idCounter;

#pragma pack(push, 1)
    uint32_t messageID;
    uint8_t messageType;
    uint32_t payloadSize;
    uint32_t payloadHash;
    std::vector<uint8_t> payload;
#pragma pack(pop)
};

#endif
