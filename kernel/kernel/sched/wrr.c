/*
 * Weighted Round Robin Scheduling Class (mapped to the SCHED_WRR policy)
 */

#include "sched.h"
#include <linux/printk.h>

int wrr_weight = MAX_WRR_WEIGHT - 1;

void init_wrr_rq(struct wrr_rq *wrr_rq, struct rq *rq)
{
	INIT_LIST_HEAD(&wrr_rq->queue);
}

static int should_boost(struct task_struct *p)
{
	return (p->cred->uid >= 10000);
}

static void
enqueue_task_wrr_internal(struct rq *rq, struct task_struct *p, int flags,
	int weight)
{
	struct list_head wrr_q;

	p->wrr.weight = weight;
	p->wrr.time_slice = WRR_TIMESLICE * weight * 10;
	wrr_q = rq->wrr.queue;
	list_add_tail(&p->wrr.run_list, &wrr_q);
	inc_nr_running(rq);
}


static void timeslice_end(struct rq *rq, struct task_struct *p, int queued)
{
	list_del(&(p->wrr.run_list));
	if (p->wrr.weight > 0)
		--p->wrr.weight;
	p->wrr.time_slice = WRR_TIMESLICE * p->wrr.weight;
	enqueue_task_wrr_internal(rq, p, 0, p->wrr.weight);
	set_tsk_need_resched(p);
}

static void
enqueue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	int weight = wrr_weight;

	if (!should_boost(p))
		weight = 0;
	enqueue_task_wrr_internal(rq, p, weight, flags);
}


struct task_struct *_find_container(struct list_head *cursor)
{
	return container_of(list_entry(cursor, struct sched_wrr_entity,
					run_list),
			struct task_struct, wrr);
}

static void dequeue_task_wrr(struct rq *rq, struct task_struct *p, int flags)
{
	list_del(&(p->wrr.run_list));
	dec_nr_running(rq);
}

static void yield_task_wrr(struct rq *rq)
{
	struct task_struct *curr = rq->curr;
	dequeue_task_wrr(rq, curr, 0);
	enqueue_task_wrr(rq, curr, 0);
}

static bool yield_to_task_wrr(
	struct rq *rq,
	struct task_struct *p,
	bool preempt
)
{
	return 0;
}

static void check_preempt_curr_wrr(
	struct rq *rq,
	struct task_struct *p,
	int flags
)
{

}

int xxx;
static struct task_struct *pick_next_task_wrr(struct rq *rq)
{
	struct task_struct *next = NULL;
	int i;
	if (xxx)
		return NULL;
	for (i = MAX_WRR_WEIGHT; i >= 0; i--) {
		if (!list_empty(&(rq->wrr.queue))) {
			next = _find_container(rq->wrr.queue.next);
			break;
		}
	}
	if (!xxx)
		xxx = !xxx;
	return next;
}

static void put_prev_task_wrr(struct rq *rq, struct task_struct *p)
{

}

static void set_curr_task_wrr(struct rq *rq)
{

}

static void task_tick_wrr(struct rq *rq, struct task_struct *p, int queued)
{
	if (--p->wrr.time_slice)
		return;
	timeslice_end(rq, p, queued);
}

static void task_fork_wrr(struct task_struct *p)
{

}

static void switched_from_wrr(struct rq *this_rq, struct task_struct *task)
{

}

static void switched_to_wrr(struct rq *this_rq, struct task_struct *task)
{

}

static void prio_changed_wrr(
	struct rq *this_rq,
	struct task_struct *task,
	int oldprio
)
{

}

static unsigned int get_rr_interval_wrr(struct rq *rq, struct task_struct *task)
{
	return 0;
}


const struct sched_class sched_wrr_class = {
	.next                   = &fair_sched_class,
	.enqueue_task           = enqueue_task_wrr,
	.dequeue_task           = dequeue_task_wrr,
	.yield_task             = yield_task_wrr,
	.yield_to_task          = yield_to_task_wrr,
	.check_preempt_curr     = check_preempt_curr_wrr,
	.pick_next_task         = pick_next_task_wrr,
	.put_prev_task          = put_prev_task_wrr,
	.set_curr_task          = set_curr_task_wrr,
	.task_tick              = task_tick_wrr,
	.task_fork              = task_fork_wrr,
	.prio_changed           = prio_changed_wrr,
	.switched_from          = switched_from_wrr,
	.switched_to            = switched_to_wrr,
	.get_rr_interval        = get_rr_interval_wrr,
};
