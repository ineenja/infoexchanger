#include <gtest/gtest.h>
#include <msg/msg.h>



TEST(MessageTests, MessageHeaderCheckVector) {

    std::vector<uint8_t> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 1);
    EXPECT_EQ(testMsg1.getPayloadSize(), 24);
}

TEST(MessageTests, MessageHeaderCheck2MessagesVector) {

    std::vector<uint32_t> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 2);
    EXPECT_EQ(testMsg1.getPayloadSize(), 24);

    std::vector<uint32_t> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2, 1);

    EXPECT_EQ(testMsg2.getMessageType(), 1);
    EXPECT_EQ(testMsg2.getMessageID(), 3);
    EXPECT_EQ(testMsg2.getPayloadSize(), 24);
}

TEST(MessageTests, MessageHeaderCheckDouble) {

    double testData1 = 228;
    Message testMsg1(testData1, 2);

    EXPECT_EQ(testMsg1.getMessageType(), 2);
    EXPECT_EQ(testMsg1.getMessageID(), 4);
    EXPECT_EQ(testMsg1.getPayloadSize(), 8);
}

TEST(MessageTests, MessageHeaderCheckString) {

    std::string testData1 = "2jbljkgyugh28";
    Message testMsg1(testData1, 4);

    EXPECT_EQ(testMsg1.getMessageType(), 4);
    EXPECT_EQ(testMsg1.getMessageID(), 5);
    EXPECT_EQ(testMsg1.getPayloadSize(), 32);
}

///// TODO:
/// тесты функции хеширования
///
/// тесты декодирования сообщения - т.е. корректность передаваемых данных


