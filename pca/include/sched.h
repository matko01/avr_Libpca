#ifndef __SCHED_H__
#define __SCHED_H__

#ifdef OFFLINE_TESTING
#include <stdint.h>
#else
#include "config.h"
#include "common.h"
#include "timer_common.h"
#endif

/**
 * @brief time resolution type
 */
typedef SCHED_TIME_RESOLUTION t_timeres;


/**
 * @brief callback handler format
 *
 * @param a_data custom data
 *
 * @return none
 */
typedef void (*t_fh)(void *a_data);


/**
 * @brief task status bitmasks
 */
#define SCHED_TS_NOT_EXECUTABLE 0x00
#define SCHED_TS_EXECUTABLE 0x02
#define SCHED_TS_PAUZED 0x00
#define SCHED_TS_RUNNING 0x01

/**
 * @brief create a compound variable
 */
#define SCHED_COMBINE_TS_ID(__ts, __id) ((__ts << 6) | (__id & 0x3f))

/**
 * @brief check if task is executable
 */
#define SCHED_TASK_IS_EXECUTABLE(__id) ( __id & (SCHED_TS_EXECUTABLE << 6))

/**
 * @brief is task marked as running (not pauzed)
 */
#define SCHED_TASK_IS_RUNNING(__id) ( __id & (SCHED_TS_RUNNING << 6))

/**
 * @brief task struct
 */
typedef struct _t_task {
	uint8_t ts_id; // uppder bit holds the state, lower 7 bits hold the task id

	t_timeres interval_static;
	volatile t_timeres interval_dynamic;
	
	t_fh handler;
	void *data;
	struct _t_task *prv, *nxt;
} t_task;


/**
 * @brief initialize scheduler
 *
 * @param a_freq scheduler tick frequency
 */
void sched_init(uint32_t a_freq);

/**
 * @brief create new task
 *
 * @param a_handler task callback
 * @param a_data data for the callback
 * @param a_interval interval (number of ticks)
 *
 * @return task id or SCHED_MAX_TASKS if unable to create task
 */
uint8_t sched_task_new(t_fh a_handler, void *a_data, t_timeres a_interval);

/**
 * @brief delete task of given id
 *
 * @param a_id task id
 */
void sched_task_delete(uint8_t a_id);

/**
 * @brief pause task (stop it from being scheduled)
 *
 * @param a_id task id
 */
void sched_task_pauze(uint8_t a_id);

/**
 * @brief resume previously paused task
 *
 * @param a_id task id
 */
void sched_task_resume(uint8_t a_id);

/**
 * @brief run scheduler
 */
void sched_run();

/**
 * @brief how many task there is 
 *
 * @return 
 */
uint8_t sched_task_count();

#endif /* __SCHED_H__ */
