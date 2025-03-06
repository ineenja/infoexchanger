#ifndef DATASOURCE
#define DATASOURCE

#include "msg.h"

class DataSource {
    public:
    DataSource();

    Message newMsg(const void* data, uint32_t msgType) {
        Message msg(data, msgType);
        return msg;
    }

    private:

};

#endif
