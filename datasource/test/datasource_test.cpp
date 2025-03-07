#include <gtest/gtest.h>
#include <datasource/datasource.h>


TEST(DataSourceTests, MessageCreationCheckWithTimer) {

    std::vector<double> testData1 = {1,2,3,4,5};
    std::vector<Message> Messages;

    MessageGenerator TestMsgCreator = MessageGenerator(std::chrono::milliseconds(500), &Messages);

    auto StartTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds duration(4000);

    while (true) {
        auto CurrentTime = std::chrono::steady_clock::now();
        if (CurrentTime - StartTime >= duration) {
            std::cout << "finished" << std::endl;
            break;
        }
        TestMsgCreator.newMsg(testData1, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_EQ(Messages.size(), 7);

    EXPECT_EQ((Messages[0]).getMessageType(), 1);
    EXPECT_EQ((Messages[0]).getMessageID(), 1);
    EXPECT_EQ((Messages[0]).getPayloadSize(), testData1.size() * sizeof(double));

    auto checkData1 = any_cast<std::vector<double>>(getDataFromBytes(Messages[1].getPayload(), Messages[1].getMessageType()));

     for (int i = 0; i < testData1.size(); i++) {
         EXPECT_EQ(testData1[i], checkData1[i]);
     }
}
