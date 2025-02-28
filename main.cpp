#include "common/msg/include/msg/msg.h"
#include "common/mylist/include/mylist/mylist.h"


int main()
{
    std::vector<double> testData1 = {1,2,3,4,5,6,7,8,9};
    Message testMsg1(testData1, 1);
    auto dick = getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType());
    std::vector<double> check = std::any_cast<std::vector<double>>(dick);


    double testpenis = 123;
    Message testMsg2(testpenis, 2);
    auto cock = getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType());

    int testdick = 2;
    Message testMsg3(testdick, 3);
    auto penis = getDataFromBytes(testMsg3.getPayload(), testMsg3.getMessageType());

    std::string testcunt = "coochie";
    Message testMsg4(testcunt, 4);
    auto cunt = getDataFromBytes(testMsg4.getPayload(), testMsg4.getMessageType());



    return 0;
}

