#include "sched.h"
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>

/* ================================================================================ */

static volatile t_task g_tasks[SCHED_MAX_TASKS];
static volatile t_task *g_head = NULL,
			  *g_tail = NULL;

static uint8_t _sched_find_task_slot() {
	uint8_t i = SCHED_MAX_TASKS;
	
	/**
	 * @brief loop through slots
	 */
	while (i--) {
		if (!SCHED_TASK_IS_EXECUTABLE(g_tasks[i].ts_id)) {
			return i;
		}
	}

	return SCHED_MAX_TASKS;
}

/* ================================================================================ */

#if SCHED_IMPLEMENT_TIMER_INT == 1

#if SCHED_TIMER == E_TIMER0
ISR(TIMER0_COMPA_vect) 
#elif SCHED_TIMER == E_TIMER1
ISR(TIMER1_COMPA_vect) 
#elif SCHED_TIMER == E_TIMER2
ISR(TIMER2_COMPA_vect) 
#else
#error UNKNOWN TIMER REQUESTED
#endif
{
	volatile t_task *tsk = (t_task *)g_head;

	// decrement the interval for all scheduled tasks
	while (tsk) {
		if (tsk->interval_dynamic && SCHED_TASK_IS_RUNNING(tsk->ts_id)) {
			tsk->interval_dynamic--;
		}
		tsk = tsk->nxt;
	}
}
#endif

/* ================================================================================ */

void sched_init(uint32_t a_freq) {
	uint32_t pocr;

	memset((void *)g_tasks, 0x00, sizeof(g_tasks));
	_timer_init_ctc(SCHED_TIMER);

	if (E_TIMER1 == SCHED_TIMER) {
		pocr = _timer_freq_prescale(E_TIMER1, 2*a_freq, 65535);
	}
	else {
		pocr = _timer_freq_prescale(SCHED_TIMER, 2*a_freq, 255);
	}

	_timer_setup_ctc(SCHED_TIMER, pocr);
	_timer_en_compa_int(SCHED_TIMER);

	g_head = NULL;
	g_tail = NULL;
}


uint8_t sched_task_new(t_fh a_handler, void *a_data, t_timeres a_interval) {
	uint8_t idx = _sched_find_task_slot(); 
	
	//_sched_find_task_slot();

	if (SCHED_MAX_TASKS != idx) {
		memset((void *)&g_tasks[idx], 0x00, sizeof(g_tasks[idx]));

		g_tasks[idx].ts_id = SCHED_COMBINE_TS_ID((SCHED_TS_EXECUTABLE | SCHED_TS_RUNNING), idx);
		g_tasks[idx].interval_static = a_interval;
		g_tasks[idx].interval_dynamic = a_interval;
		g_tasks[idx].handler = a_handler;
		g_tasks[idx].data = a_data;

		// empty list
		if ((g_head == g_tail) && (g_head == NULL)) {
			g_head = &g_tasks[idx];
		}
		else {
			g_tasks[idx].prv = (t_task *)g_tail;
			g_tail->nxt = (t_task *)&g_tasks[idx];
			g_tasks[idx].nxt = NULL;
		}

		g_tail = &g_tasks[idx]; 
	}

	return idx;
}


void sched_task_delete(uint8_t a_id) {

	/// maintain the head
	if (NULL == g_tasks[a_id].prv) {
		g_head = g_tasks[a_id].nxt;
	}
	else {
		g_tasks[a_id].prv->nxt = g_tasks[a_id].nxt;
	}

	/// maintain the tail
	if (NULL == g_tasks[a_id].nxt) {
		g_tail = g_tasks[a_id].prv;
	}
	else {
		g_tasks[a_id].nxt->prv = g_tasks[a_id].prv;
	}

	memset((void *)&g_tasks[a_id], 0x00, sizeof(g_tasks[a_id]));
}


void sched_task_pauze(uint8_t a_id) {
	g_tasks[a_id].ts_id &= 0xbf;
	g_tasks[a_id].ts_id |= (SCHED_TS_PAUZED << 6);
}


void sched_task_resume(uint8_t a_id) {
	g_tasks[a_id].ts_id &= 0xbf;
	g_tasks[a_id].ts_id |= (SCHED_TS_RUNNING << 6);
}


uint8_t sched_task_count() {
	t_task *next = (t_task *)g_head;
	uint8_t cnt = 0;

	while (next) {
		cnt++;	
		next = next->nxt;
	}
	return cnt;
}


void sched_run() {

	t_task *tsk = (t_task *)g_head;
	while (tsk) {
		if (!tsk->interval_dynamic && tsk->handler) {
			tsk->handler(tsk->data);
			tsk->interval_dynamic = tsk->interval_static;
		}
		tsk = tsk->nxt;
	}
}

