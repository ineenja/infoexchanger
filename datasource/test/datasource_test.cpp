#include <gtest/gtest.h>
#include <datasource/datasource.h>

///// тест создания одного и того же сообщения раз в интервал
TEST(DataSourceTests, MessageCreationCheckWithTimer) {

    std::vector<double> testData1 = {1,2,3,4,5};
    MyList<Message> Messages;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &Messages);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(1500);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        bool msgWasCreated = TestMsgCreator.newMsg(testData1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(Messages.getSize(), 2);

    EXPECT_EQ((Messages.front()).getMessageType(), 1);
    //EXPECT_EQ((Messages[0]).getMessageID(), 1);
    EXPECT_EQ((Messages.front()).getPayloadSize(), testData1.size() * sizeof(double));

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(Messages.front().getPayload(), Messages.front().getMessageType()));

     for (int i = 0; i < testData1.size(); i++) {
         EXPECT_EQ(testData1[i], checkData1[i]);
     }

    std::cout << "average time to create a message is " << TestMsgCreator.getMeanTimeToCreateAMsg().count() << " ns" << std::endl;
}

///// тест создания разных сообщений раз в интервал
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

    MyList<Message> MessagesQueue;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &MessagesQueue);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(3000);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        bool msgWasCreated;
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        msgWasCreated = TestMsgCreator.newMsg(arr[MessagesQueue.getSize()], MessagesQueue.getSize()+1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(MessagesQueue.getSize(), 5);

    auto iter = MessagesQueue.begin();

    EXPECT_EQ((*iter).getMessageType(), 1);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((*iter).getPayloadSize(), testData1.size() * sizeof(double));
    iter++;

    EXPECT_EQ((*iter).getMessageType(), 2);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((*iter).getPayloadSize(), testData2.size() * sizeof(int));
    iter++;

    EXPECT_EQ((*iter).getMessageType(), 3);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((*iter).getPayloadSize(), sizeof(double));
    iter++;

    EXPECT_EQ((*iter).getMessageType(), 4);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((*iter).getPayloadSize(), sizeof(int));
    iter++;

    EXPECT_EQ((*iter).getMessageType(), 5);
    //EXPECT_EQ((MessagesQueue[0]).getMessageID(), 8);
    EXPECT_EQ((*iter).getPayloadSize(), testData5.size() * sizeof(char));
    iter = MessagesQueue.begin();

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes((*iter).getPayload(), (*iter).getMessageType()));

    for (int i = 0; i < testData1.size(); i++) {
        EXPECT_EQ(testData1[i], checkData1[i]);
    }

    std::cout << "average time to create a message is " << TestMsgCreator.getMeanTimeToCreateAMsg().count() << " ns" << std::endl;
}

///// тест закидывания сообщений в очередь с лимитированным размером
TEST(DataSourceTests, QueueSizeControlCheck) {

    std::vector<double> testData1 = {1,2,3,4,5};

    MyList<Message> MessagesQueue;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &MessagesQueue);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(3500);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        bool msgWasCreated = TestMsgCreator.newMsg(testData1, 1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(MessagesQueue.getSize(), 5);

    EXPECT_EQ((MessagesQueue.front()).getMessageID(), 9);

    std::cout << "average time to create a message is " << TestMsgCreator.getMeanTimeToCreateAMsg().count() << " ns" << std::endl;
}




/////////////////////////////////////////////////////////////////
/////////////////// ТЕСТЫ ПЕРЕДАЧИ СООБЩЕНИЙ ////////////////////
/////////////////////////////////////////////////////////////////

/// тест вытаскивания сообщений из очереди с последующим копированием в общую память, проверяем на одном сообщении
TEST(DataSourceTests, SingleMessagesMovingCheck) {

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

    MyList<Message> MessagesQueue;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &MessagesQueue);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(1000);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        bool msgWasCreated = TestMsgCreator.newMsg(arr[MessagesQueue.getSize()], MessagesQueue.getSize()+1);
        if (msgWasCreated == true){
            std::cout << "msg was created" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << std::endl;

    uint8_t sharedMemorySize = sizeof(uint8_t) * 1000;
    uint8_t sharedMemorySlotSize = sizeof(uint8_t) * 100;
    auto *sharedMemoryPtr = new uint8_t[sharedMemorySize];

    MessageTransferer TestMsgTransferer = MessageTransferer(std::chrono::milliseconds(500), &MessagesQueue, sharedMemoryPtr, sharedMemorySize, sharedMemorySlotSize);
    StartTime = std::chrono::steady_clock::now();

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            break;
        }
        bool msgWasPulled = TestMsgTransferer.tryPullMsg();
        if (msgWasPulled == true){
            std::cout << "msg was pulled" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "pulling finished" << std::endl;

    Message checkData1Msg = turnBytesIntoMessage(sharedMemoryPtr);
    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(checkData1Msg.getPayload(), checkData1Msg.getMessageType()));
    int i = 0;
    for (const double value : testData1) {
        EXPECT_EQ(checkData1[i++], value);
    }

    // sharedMemoryPtr = sharedMemoryPtr + sharedMemorySlotSize;
    // Message checkData2Msg = turnBytesIntoMessage(sharedMemoryPtr);
    // auto checkData2 = any_cast<std::vector<int>>(getDataFromBytes(checkData2Msg.getPayload(), checkData2Msg.getMessageType()));
    // i = 0;
    // for (const double value : testData2) {
    //     EXPECT_EQ(checkData2[i++], value);
    // }


    delete [] sharedMemoryPtr;
}





