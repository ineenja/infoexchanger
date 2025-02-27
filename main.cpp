#include "common/msg/include/msg/msg.h"
#include "common/mylist/include/mylist/mylist.h"


int main()
{
    std::vector<uint8_t> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    return 0;
}

