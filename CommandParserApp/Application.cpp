/**
 * @file  Application.h
 * @brief Application class to facilitate action like start, stop and signal
 * @note
 *
 */
/*-----------------------------------------------------------------------*/
/* Header                                                                */
/*-----------------------------------------------------------------------*/
#include "Application.h"
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
 * @brief Initialize reference to background task obj
 *
 * @param  task reference to the object
 * @return None
 */
Application::Application(BackgroundTask* task){
    assert(task != NULL);
    task_ = task;
}

/**
 * @brief Start the application which runs background task in a thread
 *
 * @param  None
 * @return None
 */
void Application::start(void)
{
    /* Run the background task in a thread */
    taskThread_ = std::thread(&BackgroundTask::run, task_);
}

/**
 * @brief Stop the background task and wait for the thread to finish
 *
 * @param  None
 * @return None
 */
void Application::stop(void) {
    /* Stop the background task */
    task_->stop();

    /* Wait for the thread to join */
    taskThread_.join();
}

/**
 * @brief Indicate that data is available when the shared memory is full
 *
 * @param  None
 * @return None
 */
void Application:: dataAvailable(void) {
     /* Signal the background task that data is available */
     task_->notifyDataAvailable();
}
