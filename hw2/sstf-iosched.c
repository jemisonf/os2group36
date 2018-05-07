#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

// Forwards and backwards pre-processor statements to determine direction of elevator
#define Head_Forward 1
#define Head_Backward 0


struct sstf_Data {
	//struct that contains the pointer to the queue
	struct list_head queue;
	//
	sector_t head_Position;
	// Direction that the eleveor is moving
	unsigned char Direction;
};

//
static void FIFO_Merge(struct request_queue *q, struct request *rq, struct request *next)
{
	list_del_init(&next->queuelist);
}

static int sstf_dispatch(struct request_queue *q, int force) 
{
	struct sstf_Data *sd = q->elevator->elevator_data;

	if (!list_empty(&sd->queue)) {

		struct request *req_Next;
		struct request *req_Previous;
		struct request *rq;
		
		req_Next = list_entry(sd->queue.next, struct request, queuelist);
		req_Previous = list_entry(sd->queue.prev, struct request, queuelist);

		// Check if the next element wrapped is the first element
		// This means that there is only one element in the array.
		if (req_Next == req_Previous) {
			rq = req_Next;
		} else {
			if (sd->Direction == Head_Forward) {
				if (req_Next->__sector > sd->head_Position) {
					rq = req_Next;
				} else {
					sd->Direction = Head_Backward;
					rq = req_Previous;
				}
			} else { // if we are going backwards, chance position to previous.
				if (req_Previous->__sector < sd->head_Position) {
					rq = req_Previous;
				} else {
					sd->Direction = Head_Forward;
					rq = req_Next;
				}
			}
		}

		list_del_init(&rq->queuelist);
		/* Calculate the position where the head will end up */
		sd->head_Position = blk_rq_pos(rq) + blk_rq_sectors(rq);
		elv_dispatch_add_tail(q, rq);

		/* For debugging */
		if(rq_data_dir(rq) == 0){
			printk("[SSTF] dsp READ %llu\n",rq->__sector);
		} else {
			printk("[SSTF] dsp WRITE %llu\n",rq->__sector);
		}
		return 1;
	}
	return 0;
}

static void sstf_add_request(struct request_queue *q, struct request *rq)
{
	struct sstf_Data *sd = q->elevator->elevator_data;

	struct request *rnext, *rprev;
	sector_t next, rq_sector;

	/*
	If the list is empty, just add the request.
	*/
	if(list_empty(&sd->queue)){
		list_add(&rq->queuelist,&sd->queue);
	} else {
		rnext = list_entry(sd->queue.next, struct request, queuelist);
		rprev = list_entry(sd->queue.prev, struct request, queuelist);

		next = blk_rq_pos(rnext);
		rq_sector = blk_rq_pos(rq);

		while(rq_sector > next){
			rnext = list_entry(sd->queue.next, struct request, queuelist);
			rprev = list_entry(sd->queue.prev, struct request, queuelist);
			next = blk_rq_pos(rnext);
		}

		/* __list_add() adds between 2 consecutive entries */
		__list_add(&rq->queuelist, &rprev->queuelist, &rnext->queuelist);
	}
	/* For debugging: */
	printk(KERN_INFO "[SSTF] add %s %ld\n",rq->cmd,(long) rq->__sector);
}

static struct request *sstf_former_request(struct request_queue *q, struct request *rq)
{
	struct sstf_Data *sd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &sd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *sstf_latter_request(struct request_queue *q, struct request *rq)
{
	struct sstf_Data *sd = q->elevator->elevator_data;

	if (rq->queuelist.next == &sd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

static int sstf_init_queue(struct request_queue *q, struct elevator_type *e)
{

	struct sstf_Data *sd;
	struct elevator_queue *eq;

	eq = elevator_alloc(q, e);
	if(!eq)
		return -ENOMEM;

	sd = kmalloc_node(sizeof(*sd), GFP_KERNEL, q->node);
	if(!sd)
	{
		kobject_put(&eq->kobj);
		return -ENOMEM;
	}
	
	sd->head_Position = 0;
	eq->elevator_data = sd;

	INIT_LIST_HEAD(&sd->queue);
	spin_lock_irq(q->queue_lock);
	q->elevator = eq;
	spin_unlock_irq(q->queue_lock);
	return 0;
}

static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_Data *sd = e->elevator_data;

	BUG_ON(!list_empty(&sd->queue));
	kfree(sd);
}

static struct elevator_type elevator_sstf = {
	.ops = {
		.elevator_merge_req_fn		= FIFO_Merge,
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_former_req_fn		= sstf_former_request,
		.elevator_latter_req_fn		= sstf_latter_request,
		.elevator_init_fn			= sstf_init_queue,
		.elevator_exit_fn			= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

static int __init sstf_init(void)
{
	elv_register(&elevator_sstf);
	return 0;
}

static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);


MODULE_AUTHOR("Scott Russell, Arya Asgari, Fischer Jemison, Group 36");
