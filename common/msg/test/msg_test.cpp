#include <gtest/gtest.h>
#include <msg/msg.h>

TEST(MessageTests, MessageHeaderCheckVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 1);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size() * sizeof(double));
}

TEST(MessageTests, MessageHeaderCheck2MessagesVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1);

    EXPECT_EQ(testMsg1.getMessageType(), 1);
    EXPECT_EQ(testMsg1.getMessageID(), 2);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size()  * sizeof(double));

    std::vector<int> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2);

    EXPECT_EQ(testMsg2.getMessageType(), 2);
    EXPECT_EQ(testMsg2.getMessageID(), 3);
    EXPECT_EQ(testMsg2.getPayloadSize(), testData2.size()  * sizeof(int));
}

TEST(MessageTests, MessageHeaderCheckDouble) {

    double testData1 = 228;
    Message testMsg1(testData1);

    EXPECT_EQ(testMsg1.getMessageType(), 3);
    EXPECT_EQ(testMsg1.getMessageID(), 4);
    EXPECT_EQ(testMsg1.getPayloadSize(), sizeof(double));
}

TEST(MessageTests, MessageHeaderCheckString) {

    std::string testData1 = "2jbljkgyugh28";
    Message testMsg1(testData1);

    EXPECT_EQ(testMsg1.getMessageType(), 5);
    EXPECT_EQ(testMsg1.getMessageID(), 5);
    EXPECT_EQ(testMsg1.getPayloadSize(), testData1.size());
}

TEST(MessageTests, MessagePayloadCheckVector) {

    std::vector<double> testData1 = {1,2,3,4,5};
    Message testMsg1(testData1);

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }

    std::vector<int> testData2 = {6,7,8,9,10};
    Message testMsg2(testData2);

    auto checkData2 = any_cast<std::vector<int>>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));

    for (int i = 0; i < testData2.size(); i++) {
        EXPECT_EQ(testData2[i], checkData2[i]);
    }
}

TEST(MessageTests, MessagePayloadCheckVariables) {

    double testData1 = 228.1337;
    Message testMsg1(testData1);

    auto checkData1 = any_cast<double>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));
    EXPECT_EQ(testData1, checkData1);

    int testData2 = 2281337;
    Message testMsg2(testData2);

    auto checkData2 = any_cast<int>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));
    EXPECT_EQ(testData2, checkData2);
}

TEST(MessageTests, MessagePayloadCheckString) {

    std::string testData1 = "228.1337";
    Message testMsg1(testData1);

    auto checkData1 = any_cast<std::string>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));
    EXPECT_EQ(testData1, checkData1);

    std::string testData2 = "kto chitaet tot krasavchik";
    Message testMsg2(testData2);

    auto checkData2 = any_cast<std::string>(getDataFromBytes(testMsg2.getPayload(), testMsg2.getMessageType()));
    EXPECT_EQ(testData2, checkData2);
}

TEST(MessageTests, CreatingAVectorOfSameMessages) {

    std::vector<double> testData1 = {1,2,3,4,5};
    std::vector<Message> Messages;

    Message testMsg1(testData1);
    Message testMsg2(testData1);

    Messages.push_back(testMsg1);
    Messages.push_back(testMsg2);

    EXPECT_EQ(Messages.size(), 2);

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(Messages[1].getPayload(), Messages[1].getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }
}

TEST(MessageTests, PassingAPointerToMessageConstructor) {

    double testValue = 1337;
    double* testPtr = &testValue;
    Message testMsg(testValue);
    Message testMsgPtr(testPtr);

    auto checkValue = any_cast<double>(getDataFromBytes(testMsgPtr.getPayload(), testMsgPtr.getMessageType()));

    EXPECT_EQ(testValue, checkValue);
}

TEST(MessageTests, PassingACastedVoidPointerToMessageConstructor) {

    std::vector<double> testData1 = {1,2,3,4,5};
    void* ptr = &testData1;
    Message testMsg1(*static_cast<std::vector<double>*>(ptr));

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(testMsg1.getPayload(), testMsg1.getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }
}

TEST(MessageTests, BytesIntoMessageCheck) {

    uint8_t serializedData[] = {
        0x01, 0x00, 0x00, 0x00, // messageID = 1
        0x02, 0x00, 0x00, 0x00,     // messageType = 2
        0x03, 0x00, 0x00, 0x00, // payloadSize = 3
        0xAA, 0xBB, 0xCC, 0xDD, // payloadHash = 0xAABBCCDD
        0x11, 0x22, 0x33        // payload = {0x11, 0x22, 0x33}
    };


    Message checkMsg = turnBytesIntoMessage(serializedData);

    EXPECT_EQ(checkMsg.getMessageID(), 1);
    EXPECT_EQ(checkMsg.getMessageType(), 2);
    EXPECT_EQ(checkMsg.getPayloadSize(), 3);
}

TEST(MessageTests, MessageToBytesAndBytesIntoMessageCheck) {

    std::vector<double> testData1 = {1,2,3,4,5};
    void* ptr = &testData1;
    Message testMsg1(*static_cast<std::vector<double>*>(ptr));

    std::vector<uint8_t> serializedData = divideMessageIntoBytes(testMsg1);
    uint8_t* dataPtr = serializedData.data();

    Message checkMsg = turnBytesIntoMessage(dataPtr);

    EXPECT_EQ(checkMsg.getMessageID(), 17);
    EXPECT_EQ(checkMsg.getMessageType(), 1);
    EXPECT_EQ(checkMsg.getPayloadSize(), 40);
}

///// TODO:
/// тесты функции хеширования
///



