/**
 * @file  Test the application code that counts the valid command sequences
 * @brief This facilitates different test conditions using google test framework
 * @note
 *
 */
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

/* Include application code here */
#include "Application.cpp"
#include "CmdSeqParser.cpp"
#include "BackgroundTask.cpp"
#include "SharedMem.cpp"

/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
class TestApp : public ::testing::Test {
    protected:
        void SetUp() override {
            shmem_ = new SharedMem();
            processor_ = new CmdSeqParser(shmem_);
            task_ = new BackgroundTask(processor_);
            app_ = new Application(task_);
	
	    /* Start the thread here */
            appThread_ = std::thread([this]() { app_->start(); });
        } 

        void TearDown() override {
            /* Stop the application thread */
	    app_->stop();
	    appThread_.join();

            /* delete the app objects */
            delete app_;
            delete task_;
            delete processor_;
            delete shmem_;
        }

	void TestApp_SignalAndTest(bool isFull, bool isEmpty, uint64_t count){
		/* Check the buffer is full */
		EXPECT_EQ(shmem_->IsFull(), isFull);

		/* Signal data is available */
		app_->dataAvailable();

		/* wait for processing to finish */
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		/* check buffer is consumed */
		EXPECT_EQ(shmem_->IsEmpty(), isEmpty);

		/* check expected counter */
		EXPECT_EQ(processor_->getCount(), count);
	}


    /* test variables */
    CmdSeqParser* processor_;
    BackgroundTask* task_;
    Application* app_;
    SharedMem* shmem_;
    std::thread appThread_;
};
/*-----------------------------------------------------------------------*/
/* Prototype                                                             */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Variable                                                              */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Function                                                              */
/*-----------------------------------------------------------------------*/
/*
 * Define test cases
 */
TEST_F(TestApp, NoSequence_1) {
    /* Fill the shared memory buffer */
    int i = 0;
    for(i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }
    
    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, NoSequence_2) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x12);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, NoSequence_3) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0xA5);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    shmem_->PutData(0x0);
    for(int i=1;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x5A);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, NoSequence_4) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x55);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0xAA);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, singleSequence_1) {
    /* Fill the shared memory buffer */
    int i = 0;
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A);
    for(i=2;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);
}

TEST_F(TestApp, singleSequence_2) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE-2;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);
}

TEST_F(TestApp, SingleSequence_4) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0xA5);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x5A);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);
}

TEST_F(TestApp, SingleSequence_shift_1) {
    /* Fill the shared memory buffer */
    for(int i = 0;i<2;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=4;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);

    /* Fill the shared memory buffer */
    for(int i = 0;i<4;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=6;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 2);

    /* Fill the shared memory buffer */
    for(int i = 0;i<6;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=8;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 3);

    /* Fill the shared memory buffer */
    for(int i = 0;i<8;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=10;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 4);

    /* Fill the shared memory buffer */
    for(int i = 0;i<10;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=12;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 5);

    /* Fill the shared memory buffer */
    for(int i = 0;i<12;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=14;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 6);
}


TEST_F(TestApp, MultiSequence_shift_1) {
    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=4;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 2);

    /* Fill the shared memory buffer */
    for(int i = 0;i<4;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=8;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 4);

    /* Fill the shared memory buffer */
    for(int i = 0;i<8;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=12;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 6);

    /* Fill the shared memory buffer */
    for(int i = 0;i<12;i++){
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 8);
}

TEST_F(TestApp, MultiSequence_shift_2) {
    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=10;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0xAA);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 3);
}

TEST_F(TestApp, CheckSharedMemBeforeIrq) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0);
    }

    /* check buffer is not full */
    EXPECT_EQ(shmem_->IsFull(), false);

    /* Fill the buffer */
    shmem_->PutData(0);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, CheckAcrossMultipleIrq) {
    int k = 0;
    int exp = 0;
    /* Test across multiple interrupts */
    for(k=0;k<10;k++){
        shmem_->PutData(0x5A);
        /* Fill the shared memory buffer */
        for(int i=1;i<SHARED_MEM_SIZE-1;i++){ 
            shmem_->PutData(0);
        } 
        shmem_->PutData(0xA5);

	if(k > 0){
	    exp = exp+1;
	}
        /* Signal and test conditions */
        TestApp_SignalAndTest(true, true, exp);

        shmem_->PutData(0x5A);
        /* Fill the shared memory buffer */
        for(int i=1;i<SHARED_MEM_SIZE-1;i++){ 
            shmem_->PutData(0);
        }
        shmem_->PutData(0xA5);
	exp = exp+1;
        /* Signal and test conditions */
        TestApp_SignalAndTest(true, true, exp);
    }
}

TEST_F(TestApp, SequenceSplit_1) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    for(int i=0;i<SHARED_MEM_SIZE-2;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    shmem_->PutData(0x5A);
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);
}

TEST_F(TestApp, SequenceSplit_2) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    shmem_->PutData(0x5A);
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);
}

TEST_F(TestApp, SequenceSplit_3) {
    /* Fill the shared memory buffer */
    for(int i=0;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 0);

    /* Fill the shared memory buffer */
    shmem_->PutData(0x5A);
    shmem_->PutData(0x5A);
    for(int i=1;i<SHARED_MEM_SIZE-2;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);

    /* Fill the shared memory buffer */
    shmem_->PutData(0x5A);
    for(int i=1;i<SHARED_MEM_SIZE-1;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 2);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A);
    for(int i=2;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }
    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 3);
}

TEST_F(TestApp, MultiSequence_3) {
    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=14;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 4);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=14;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }
    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 8);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0x5A);
    for(int i=7;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }
    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 10);
}

TEST_F(TestApp, MultiSequence_4) {
    /* Fill the shared memory buffer */
    for(int i=0;i<(SHARED_MEM_SIZE/2);i++){ 
        shmem_->PutData(0xA5);
        shmem_->PutData(0x5A); 
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 8);
}

TEST_F(TestApp, MultiSequence_5) {
    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=2;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 1);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0x0);
    shmem_->PutData(0x5A);
    shmem_->PutData(0x0);
    shmem_->PutData(0x0);
    for(int i=6;i<SHARED_MEM_SIZE-2;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 3);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0x0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A);
    shmem_->PutData(0x0);
    for(int i=6;i<SHARED_MEM_SIZE-2;i++){ 
        shmem_->PutData(0x0);
    }
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 

    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 6);

    /* Fill the shared memory buffer */
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    shmem_->PutData(0);
    shmem_->PutData(0);
    shmem_->PutData(0xA5);
    shmem_->PutData(0x5A); 
    for(int i=14;i<SHARED_MEM_SIZE;i++){ 
        shmem_->PutData(0x0);
    }
    /* Signal and test conditions */
    TestApp_SignalAndTest(true, true, 10);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
