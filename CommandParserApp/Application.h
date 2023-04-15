/**
 * @file  Application.h
 * @brief Application class to facilitate action like start, stop and signal
 * @note
 *
 */
#ifndef __APPLICATION_H__
#define __APPLICATION_H__
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include <iostream>
#include <thread>
#include <atomic>
#include <stdint.h>
#include <mutex>
#include <cassert>
#include "SharedMem.h"
#include "CmdSeqParser.h"
#include "BackgroundTask.h"
/*-----------------------------------------------------------------------*/
/* Macro                                                                 */
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/* DataType                                                              */
/*-----------------------------------------------------------------------*/
class Application {
    public:
        Application(BackgroundTask* task);
        void start(void);            /**< Start the application */
        void stop(void);             /**< Stop the application */
        void dataAvailable(void);    /**< Signal about data availability */
    private:
        BackgroundTask* task_;   /**< Reference to background task obj */
        std::thread taskThread_; /**< Thread in the application  */
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
#endif /*__APPLICATION_H__*/
