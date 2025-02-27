#include <gtest/gtest.h>
#include <msg/msg.h>



TEST(MessageTests, MessageHeaderCheck) {

    std::vector<uint8_t> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 1);
    EXPECT_EQ(testMsg1.getPayloadHash(), 15);
    EXPECT_EQ(testMsg1.getPayloadSize(), 5);
}

TEST(MessageTests, MessageHeaderCheck2Messages) {

    std::vector<uint8_t> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1, 1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 1);
    EXPECT_EQ(testMsg1.getPayloadHash(), 15);
    EXPECT_EQ(testMsg1.getPayloadSize(), 5);

    std::vector<uint8_t> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2, 1);

    EXPECT_EQ(testMsg2.getMessageType(), 1);
    EXPECT_EQ(testMsg2.getMessageID(), 2);
    EXPECT_EQ(testMsg2.getPayloadHash(), 40);
    EXPECT_EQ(testMsg2.getPayloadSize(), 5);
}


