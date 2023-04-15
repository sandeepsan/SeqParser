/**
 * @file  BackgroundTask.h
 * @brief Class to handle background task start, stop and signaling
 * @note
 *
 */
#ifndef __BACKGROUND_TASK_H__
#define __BACKGROUND_TASK_H__
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include <atomic>
#include <thread>
#include <semaphore.h>
#include "CmdSeqParser.h"
/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
class BackgroundTask {
    public:
        BackgroundTask(CmdSeqParser* parser); /**< Initialize command process obj */
        void run();                  /**< Run the background task */
        void notifyDataAvailable();  /**< Notify that data is available */
        void stop();                 /**< Stop the background task */
    private:
        CmdSeqParser* parser_;       /**< Command process obj reference */
        std::atomic<bool> isStopped_; /**< variable to control task stop */
        sem_t sem_; /**< semaphore to signal that data is available */
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
#endif /* __BACKGROUND_TASK_H */
