#include <gtest/gtest.h>
#include <msg/msg.h>



TEST(MessageTests, MessageHeaderCheckVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 1);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size() * sizeof(double));
}

TEST(MessageTests, MessageHeaderCheck2MessagesVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 2);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size()  * sizeof(double));

    std::vector<int> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2, 2);

    EXPECT_EQ(testMsg2.getMessageType(), 2);
    EXPECT_EQ(testMsg2.getMessageID(), 3);
    EXPECT_EQ(testMsg2.getPayloadSize(), testData2.size()  * sizeof(int));
}

TEST(MessageTests, MessageHeaderCheckDouble) {

    double testData1 = 228;
    Message testMsg1(testData1, 3);

    EXPECT_EQ(testMsg1.getMessageType(), 3);
    EXPECT_EQ(testMsg1.getMessageID(), 4);
    EXPECT_EQ(testMsg1.getPayloadSize(), sizeof(double));
}

TEST(MessageTests, MessageHeaderCheckString) {

    std::string testData1 = "2jbljkgyugh28";
    Message testMsg1(testData1, 5);

    EXPECT_EQ(testMsg1.getMessageType(), 5);
    EXPECT_EQ(testMsg1.getMessageID(), 5);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size());
}

TEST(MessageTests, MessagePayloadCheckVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }

    std::vector<int> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2, 2);

    auto checkData2 = any_cast<std::vector<int>>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));

    for (int i = 0; i < testData2.size(); i++) {
        EXPECT_EQ(testData2[i], checkData2[i]);
    }
}

TEST(MessageTests, MessagePayloadCheckVariables) {

    double testData1 = 228.1337;
    Message testMsg1(testData1, 3);

    auto checkData1 = any_cast<double>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));
    EXPECT_EQ(testData1, checkData1);

    int testData2 = 2281337;
    Message testMsg2(testData2, 4);

    auto checkData2 = any_cast<int>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));
    EXPECT_EQ(testData2, checkData2);
}

TEST(MessageTests, MessagePayloadCheckString) {

    std::string testData1 = "228.1337";
    Message testMsg1(testData1, 5);

    auto checkData1 = any_cast<std::string>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));
    EXPECT_EQ(testData1, checkData1);

    std::string testData2 = "kto chitaet tot krasavchik";
    Message testMsg2(testData2, 5);

    auto checkData2 = any_cast<std::string>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));
    EXPECT_EQ(testData2, checkData2);
}

///// TODO:
/// тесты функции хеширования
///



