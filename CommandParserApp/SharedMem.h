/**
 * @file  SharedMem.h
 * @brief Shared Memory class for handling the incoming data
 * @note
 *
 */
#ifndef __SHARED_MEM_H__
#define __SHARED_MEM_H__
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include <cstdint>
#include <cstring>
#include <cassert>
#include <mutex>
/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/
/*
 * The shared memory size between application and test is 16 bytes
 */
#define SHARED_MEM_SIZE (16)

/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
class SharedMem{
    public: 
        SharedMem();        /**< Allocate shared memory */
        ~SharedMem();       /**< Release the shared memory */
        void PutData(uint8_t data); /**< Put the data in shared memory */
        uint8_t GetData();   /**< Get the data from shared memory */
        bool IsEmpty();      /**< Check empty condition */
        bool IsFull();       /**< Check Full condition */
    private:
        uint8_t* shMemAddr_; /**< Pointer to shared memory */
        uint8_t get_index_;  /**< Index in the memory for the get data */ 
        uint8_t put_index_;  /**< Index in the memory for the put data */
        uint8_t count_;      /**< Total count of data */
        std::mutex mutex_;   /**< Mutex for safe access */
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
#endif /* __SHARED_MEM_H */
