/**
 * @file  CmdSeqParser.h
 * @brief Class to identify and count the valid command sequences
 * @note
 *
 */
#ifndef __CMD_SEQ_PARSER_H__
#define __CMD_SEQ_PARSER_H__
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include "SharedMem.h"
#include <cassert>
/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
class CmdSeqParser{
    public:
        CmdSeqParser(SharedMem* shmem); /**< Initialize reference to shared mem obj */
        void parser();                  /**< Process the data in shared buffer */
        uint64_t getCount();            /**< Get the valid command count */
    private:
        enum class State { DEFAULT, FOUND_5A, FOUND_A5 }; /**< State of processing data */
        State state_ = State::DEFAULT; /**< Current state of the processing */
        SharedMem* shmem_;             /**< Reference to shared memory obj */
        uint64_t counter_ = 0;         /**< Counter to keep track of valid sequences */
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
#endif /*__CMD_SEQ_PARSER_H_*/
