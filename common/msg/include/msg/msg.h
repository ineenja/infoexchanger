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
            messageType = getDataType(*data); // определяем тип сообщения
            payload = divideDataIntoBytes(*data); // переводим сообщение в последовательность байт
        } else {
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
            if (std::is_same_v<T, void*>) {
                payload = divideDataIntoBytes(
                        *data); // переводим сообщение в последовательность байт, с разыменованием указателя на данные
            }
        } else {
            payload = divideDataIntoBytes(data); // переводим сообщение в последовательность байт
        }
        messageID = ++idCounter; // инкремент счетчика сообщений
        messageType = dataType; // определяем тип сообщения
        payloadHash = getDataHash(payload); // хешируем отправляемые байты
        payloadSize = payload.size();
    }

    explicit Message(void* data, uint8_t dataType){ // частный случай передачи данных в виде указателей на void
        messageID = ++idCounter; // инкремент счетчика сообщений
        messageType = dataType;
        if (dataType == 1){
            payload = divideDataIntoBytes(*static_cast<std::vector<double> *>(data));
        }
        if (dataType == 2){
            payload = divideDataIntoBytes(*static_cast<std::vector<int> *>(data));
        }
        if (dataType == 3){
            payload = divideDataIntoBytes(*static_cast<double *>(data));
        }
        if (dataType == 4){
            payload = divideDataIntoBytes(*static_cast<int *>(data));
        }
        if (dataType == 5){
            payload = divideDataIntoBytes(*static_cast<std::string *>(data));
        }

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
