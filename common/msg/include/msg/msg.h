#ifndef MSG
#define MSG

#include "processing/processing.h"

class Message
{
public:
    static uint32_t idCounter;

    explicit Message(const std::vector<uint8_t>& data, const uint8_t type){
        messageID = ++idCounter;
        messageType = type;
        payloadSize = data.size();

        payload = divideDataIntoBytes();
        payloadHash = getDataHash(payload);
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

private:
#pragma pack(push, 1)
    uint32_t messageID;
    uint32_t messageType;
    uint32_t payloadSize;
    uint32_t payloadHash;
    std::vector<uint8_t> payload;
#pragma pack(pop)
};

#endif
