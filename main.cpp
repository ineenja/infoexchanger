#include "common/msg/include/msg/msg.h"
#include "common/mylist/include/mylist/mylist.h"


int main()
{
    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);
    auto dick = getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType());
    std::cout << (std::any_cast<std::vector<double>>(dick)).size() << std::endl;


    double testpenis = 123;
    Message testMsg2(testpenis, 2);
    auto cock = getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType());
    std::cout << std::any_cast<double>(cock) << std::endl;

    int testdick = 2;
    Message testMsg3(testdick, 3);
    auto penis = getDataFromBytes(testMsg3.getPayload(), testMsg3.getMessageType());
    std::cout << std::any_cast<int>(penis) << std::endl;

    return 0;
}

