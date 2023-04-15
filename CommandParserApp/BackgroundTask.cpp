/**
 * @file  BackgroundTask.cpp
 * @brief Class to handle background task start, stop and signaling
 * @note
 *
 */
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include "BackgroundTask.h"
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
 * @brief Initialize reference to commanad sequence processing obj
 *
 * @param  CmdSeqParser* reference to the object
 * @return None
 */
BackgroundTask::BackgroundTask(CmdSeqParser* parser)
{
    assert(parser != NULL);
    /* Initialize the members */
    parser_= parser;
    isStopped_ = false;
    sem_init(&sem_, 0, 0);
}

/**
 * @brief Run the background task that invokes command procesisng routine
 *
 * @param  None
 * @return None
 */
void BackgroundTask::run()
{
    /*
     * This task runs a while loop waiting for the signal from the
     * data available routine(simulated isr) and calls the cmd process
     * routine to identify and count valid command sequences
     */
    while (true) {
        sem_wait(&sem_);
        if(isStopped_.load(std::memory_order_acquire)){
            break;
        }
        parser_->parser();
    }
}

/**
 * @brief Notify that data is available from the test harness
 *
 * @param  None
 * @return None
 */
void BackgroundTask::notifyDataAvailable() 
{
    /* Signal that data is available */
    sem_post(&sem_);
}

/**
 * @brief Stop the background task 
 *
 * @param  None
 * @return None
 */
void BackgroundTask::stop()
{
    /* Set the stop flag to come out of while loop */
    isStopped_.store(true, std::memory_order_release);

    /* Signal to come out of wait condition */
    sem_post(&sem_);
}
