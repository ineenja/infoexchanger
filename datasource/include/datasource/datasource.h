#ifndef DATASOURCE
#define DATASOURCE

#include "msg/msg.h"

class DataSource {
    public:
    DataSource();

    Message newMsg(const void* data, uint32_t msgType) {
        Message msg;
        msg.payload.resize(msgType);

        return msg;
    }

    private:

};

#endif
