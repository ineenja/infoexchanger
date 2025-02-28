#ifndef MSG
#define MSG

#include <processing/processing.h>

class Message
{
public:

    template <typename T>
    explicit Message(const T& data, const uint8_t type){
        messageID = ++idCounter; // инкремент счетчика сообщений
        messageType = type; // определяем тип сообщения
        payloadSize = sizeof(data); // определяем размер тела сообщения в байтах

        payload = divideDataIntoBytes(data, type); // переводим сообщение в последовательность байт
        payloadHash = getDataHash(payload); // хешируем отправляемые байты
    }

    uint32_t getMessageID() const {
        return(messageID);
    }

    uint32_t getMessageType() const {
        return(messageType);
    }

    uint32_t getPayloadSize() const {
        return(payloadSize);
    }

    uint32_t getPayloadHash() const {
        return(payloadHash);
    }

    [[nodiscard]] std::vector<uint8_t> getPayload() const {
        return(payload);
    }

private:
    static uint32_t idCounter;

#pragma pack(push, 1)
    uint32_t messageID;
    uint32_t messageType;
    uint32_t payloadSize;
    uint32_t payloadHash;
    std::vector<uint8_t> payload;
#pragma pack(pop)
};

#endif
