/**
 * @file  SharedMem.cpp
 * @brief Shared Memory class for handling the incoming data
 * @note
 *
 */
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include "SharedMem.h"
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
 * @brief Allocates shared memory of size 16 bytes
 *
 * @param  None
 * @return None
 */
SharedMem::SharedMem() 
{
    shMemAddr_= new uint8_t[SHARED_MEM_SIZE];
    memset(shMemAddr_, 0, SHARED_MEM_SIZE);
    get_index_ = 0;
    put_index_ = 0;
    count_ = 0;
}

/**
 * @brief releases the shared memory
 *
 * @param  None
 * @return None
 */
SharedMem::~SharedMem()
{
    delete[] shMemAddr_;
}

/**
 * @brief Put the data to shared memory
 *
 * @param  data  data to be written
 * @return None
 */
void SharedMem::PutData(uint8_t data) {
    assert(count_ < SHARED_MEM_SIZE);
    std::lock_guard<std::mutex> lock(mutex_);
    shMemAddr_[put_index_] = data;
    put_index_ = (put_index_ + 1) % SHARED_MEM_SIZE;
    ++count_;
}

/**
 * @brief Get the data from shared memory
 *
 * @param  None
 * @return data read data from the memory
 */
uint8_t SharedMem::GetData() {
    uint8_t data;
    assert(count_ > 0);

    std::lock_guard<std::mutex> lock(mutex_);
    data = shMemAddr_[get_index_];
    get_index_ = (get_index_ + 1) % SHARED_MEM_SIZE;
    --count_;
    return data;
}

/**
 * @brief check if the memory is empty
 *
 * @param  None
 * @return true/false memory is empty or not
 */
bool SharedMem::IsEmpty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return count_ == 0;
}

/**
 * @brief check if the memory is full
 *
 * @param  None
 * @return true/false memory is full or not
 */
bool SharedMem::IsFull() {
    std::lock_guard<std::mutex> lock(mutex_);
    return count_ == SHARED_MEM_SIZE;
}
