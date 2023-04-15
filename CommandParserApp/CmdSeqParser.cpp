/**
 * @file  CmdSeqParser.cpp
 * @brief Class to identify and count the valid command sequences
 * @note
 *
 */
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include "CmdSeqParser.h"
/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* Prototype                                                             */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Variable                                                              */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Function                                                              */
/*-----------------------------------------------------------------------*/
/**
 * @brief Initialize reference to shared memory object
 *
 * @param  SharedMem* reference to the object
 * @return None
 */
CmdSeqParser:: CmdSeqParser(SharedMem* shmem){
    assert(shmem != NULL);
    shmem_ = shmem;
}

/**
 * @brief Process the data in buffer for valid commands and udpate count
 *
 * @param  None
 * @return None
 */
void CmdSeqParser::parser()
{
    int i = 0;
    uint8_t data;

    /* Check that shared memory is full at this point */
    assert(shmem_->IsFull() == true);

    /* 
     * Go through the shared memory data and count the sequence
     */
    for (i = 0; i < SHARED_MEM_SIZE; i++) {
        /* Fetch the data byte from shared memory */
        data = shmem_->GetData();

        /*
	 * Based on state and received data, go to different state
	 * DEFAULT: State in which the search for the sequence begins
	 * FOUND_A5: State in which 0xA5 is received
	 * FOUND_5A: State in which 0x5A is received
	 */
        if(state_ == State::DEFAULT){
            if (data == 0x5A) {
                state_ = State::FOUND_5A;
	    }else if(data == 0xA5){
                state_ = State::FOUND_A5;
	    }
        }else if (state_ == State::FOUND_5A) {
            if (data == 0xA5) {
                state_ = State::FOUND_A5;
	    } else if(data != 0x5A){
                state_ = State::DEFAULT;
            }
	} else if (state_ == State::FOUND_A5) {
	    if (data == 0x5A) {
                state_ = State::FOUND_5A;
                counter_++;
            } else if(data != 0xA5){
                state_ = State::DEFAULT;
            }
        } else {
            if (data == 0x5A) {
                state_ = State::FOUND_5A;
            }else if(data == 0xA5){
                state_ = State::FOUND_A5;
            }else{
                state_ = State::DEFAULT;
            }
        }
    }
}

/**
 * @brief Get the count value
 *
 * @param  None
 * @return counter_ count value
 */
uint64_t CmdSeqParser::getCount(){
    /* Count of valid command sequence */
    return counter_;
}

