#include <gtest/gtest.h>
#include <datasource/datasource.h>


TEST(DataSourceTests, MessageCreationCheckWithTimer) {

    std::vector<double> testData1 = {1,2,3,4,5};
    std::vector<Message> Messages;
    Messages.reserve(8);

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &Messages);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(1500);

    while (true) {
        bool msgWasCreated;
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        msgWasCreated = TestMsgCreator.newMsg(testData1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(Messages.size(), 2);

    EXPECT_EQ((Messages[0]).getMessageType(), 1);
    //EXPECT_EQ((Messages[0]).getMessageID(), 1);
    EXPECT_EQ((Messages[0]).getPayloadSize(), testData1.size() * sizeof(double));

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(Messages[1].getPayload(), Messages[1].getMessageType()));

     for (int i = 0; i < testData1.size(); i++) {
         EXPECT_EQ(testData1[i], checkData1[i]);
     }

    std::cout << "average time to create a message is " << TestMsgCreator.getMeanTimeToCreateAMsg().count() << " ns" << std::endl;
}

TEST(DataSourceTests, DifferentMessagesCreationCheckWithTimer) {

    std::vector<double> testData1 = {1,2,3,4,5};
    std::vector<int> testData2 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    double testData3 = 4124;
    int testData4 = 41421;
    std::string testData5 = "dollbyopp";

    void* arr[5];
    arr[0] = &testData1;
    arr[1] = &testData2;
    arr[2] = &testData3;
    arr[3] = &testData4;
    arr[4] = &testData5;

    std::vector<Message> MessagesQueue;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &MessagesQueue);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(3000);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        bool msgWasCreated;
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        msgWasCreated = TestMsgCreator.newMsg(arr[MessagesQueue.size()], MessagesQueue.size()+1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(MessagesQueue.size(), 5);

    EXPECT_EQ((MessagesQueue[0]).getMessageType(), 1);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((MessagesQueue[0]).getPayloadSize(), testData1.size() * sizeof(double));

    EXPECT_EQ((MessagesQueue[1]).getMessageType(), 2);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((MessagesQueue[1]).getPayloadSize(), testData2.size() * sizeof(int));

    EXPECT_EQ((MessagesQueue[2]).getMessageType(), 3);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((MessagesQueue[2]).getPayloadSize(), sizeof(double));

    EXPECT_EQ((MessagesQueue[3]).getMessageType(), 4);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((MessagesQueue[3]).getPayloadSize(), sizeof(int));

    EXPECT_EQ((MessagesQueue[4]).getMessageType(), 5);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((MessagesQueue[4]).getPayloadSize(), testData5.size() * sizeof(char));

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(MessagesQueue[0].getPayload(), MessagesQueue[0].getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }

    std::cout << "average time to create a message is " << TestMsgCreator.getMeanTimeToCreateAMsg().count() << " ns" << std::endl;
}

