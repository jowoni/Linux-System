#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h> // for kmalloc
#include <linux/time.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/delay.h>

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_second_entry(ptr, type, member) \
	list_entry((list_entry((ptr)->next, type, member))->member.next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)

#define list_second_last_entry(ptr, type, member) \
	list_entry((list_entry((ptr)->prev, type, member))->member.prev, type, member)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_twice_next_entry(pos, member) \
	list_entry((list_entry((pos)->member.next, typeof(*(pos)), member))->member.next, typeof(*(list_entry((pos)->member.next, typeof(*(pos)), member))), member) 

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, typeof(*(pos)), member)

#define list_twice_prev_entry(pos, member) \
	list_entry((list_entry((pos)->member.prev, typeof(*(pos)), member))->member.prev, typeof(*(list_entry((pos)->member.prev, typeof(*(pos)), member))), member) 

#define list_for_each_entry_latecomer(pos, head, member)    \
 for (pos = list_first_entry(head, typeof(*pos), member); \
      &pos->member != (head);     \
      pos = list_twice_next_entry(pos, member))

#define list_for_each_entry_vanguard(pos, head, member)	\
 for (pos = list_second_entry(head, typeof(*pos), member); \
      &pos->member != (head);     \
      pos = list_twice_next_entry(pos, member))

#define list_for_each_entry_backward_latecomer(pos, head, member)    \
 for (pos = list_last_entry(head, typeof(*pos), member); \
      &pos->member != (head);     \
      pos = list_twice_prev_entry(pos, member))

#define list_for_each_entry_backward_vanguard(pos, head, member)	\
 for (pos = list_second_last_entry(head, typeof(*pos), member); \
      &pos->member != (head);     \
      pos = list_twice_prev_entry(pos, member))

static spinlock_t splock_search;

struct my_node {
	struct list_head list;
	int data;
};

static struct list_head my_list;
static struct timespec ts1, ts2, ts3, ts4, ts5, ts6, ts7, ts8, ts9, ts10, ts11, ts12;
static struct task_struct *thread1, *thread2, *thread3, *thread4;
static int inserts = 0;
static int searches = 0;
static int deletes = 0;
static int n = 0;
static struct my_node *current_node_vanguard;
static struct my_node *current_node_latecomer;
static struct my_node *current_node_backward_vanguard;
static struct my_node *current_node_backward_latecomer;
static int search_type = 0;

int list_insert(void)
{	
	int i;
	for(i = 0; i<n; i++){
		struct my_node *new = kmalloc(sizeof(struct my_node),GFP_KERNEL);
		new->data = i;
		list_add(&new->list, &my_list);
		printk("insert %d", inserts);
		inserts++;
	}     
	getnstimeofday(&ts2);
	return 0;
}

static int list_search_vanguard(void *data)
{
	list_for_each_entry_vanguard(current_node_vanguard, &my_list, list){
                if(current_node_vanguard->data == searches){
			kthread_stop(thread2);
			if(search_type == 2){
				kthread_stop(thread3);
				kthread_stop(thread4);
			}
			kthread_stop(thread1);
                        break;
                }
        }
        return 0;	
}

static int list_search_latecomer(void *data)
{
        list_for_each_entry_latecomer(current_node_latecomer, &my_list, list){
		if(current_node_latecomer->data == searches){
			kthread_stop(thread1);
			if(search_type == 2){
				kthread_stop(thread3);
				kthread_stop(thread4);
			}
			kthread_stop(thread2);
			break;
		}
	}
        return 0;	
}

static int list_search_backward_vanguard(void *data)
{
	list_for_each_entry_backward_vanguard(current_node_backward_vanguard, &my_list, list){
                if(current_node_backward_vanguard->data == searches){
			kthread_stop(thread4);
			if(search_type == 2){
				kthread_stop(thread1);
				kthread_stop(thread2);
			}
			kthread_stop(thread3);
                        break;
                }
        }
        return 0;	
}

static int list_search_backward_latecomer(void *data)
{
        list_for_each_entry_backward_latecomer(current_node_backward_latecomer, &my_list, list){
		if(current_node_backward_latecomer->data == searches){
			kthread_stop(thread3);
			if(search_type == 2){
				kthread_stop(thread1);
				kthread_stop(thread2);
			}
			kthread_stop(thread4);
			break;
		}
	}
        return 0;	
}

int list_search(void)
{
	spin_lock(&splock_search);
	for(searches = 0; searches<n; searches++){
		thread1 = kthread_run(list_search_vanguard, NULL, "list_search_vanguard");
		thread2 = kthread_run(list_search_latecomer, NULL, "list_search_latecomer");
		printk("search %d", searches);
	}
	getnstimeofday(&ts4);
	spin_unlock(&splock_search);
	return 0;
}

int list_search_backward(void)
{
	spin_lock(&splock_search);
	for(searches = 0; searches<n; searches++){
		thread3 = kthread_run(list_search_backward_vanguard, NULL, "list_search_backward_vanguard");
		thread4 = kthread_run(list_search_backward_latecomer, NULL, "list_search_backward_latecomer");
		printk("search %d", searches);
	}
	getnstimeofday(&ts6);
	spin_unlock(&splock_search);
	return 0;
}

int list_search_all(void)
{
	spin_lock(&splock_search);
	for(searches = 0; searches<n; searches++){
		thread1 = kthread_run(list_search_vanguard, NULL, "list_search_vanguard");
		thread2 = kthread_run(list_search_latecomer, NULL, "list_search_latecomer");
		thread3 = kthread_run(list_search_backward_vanguard, NULL, "list_search_backward_vanguard");
		thread4 = kthread_run(list_search_backward_latecomer, NULL, "list_search_backward_latecomer");
		printk("search %d", searches);
	}
	getnstimeofday(&ts8);
	spin_unlock(&splock_search);
	return 0;
}

int list_search_past(void)
{
	spin_lock(&splock_search);
	struct my_node *current_node;
	for(searches = 0; searches<n; searches++){
		list_for_each_entry(current_node, &my_list, list){
			if(current_node->data == searches){
				printk("search %d", searches);
				break;
			}
		}
		printk("search %d", searches);
	}
	getnstimeofday(&ts10);
	spin_unlock(&splock_search);
	return 0;
}

int list_delete(void)
{
	spin_lock(&splock_search);
        struct my_node *tmp;
	struct my_node *current_node;
        list_for_each_entry_safe(current_node, tmp, &my_list, list){
                list_del(&current_node->list);
                kfree(current_node);
		deletes++;
                continue;
        }
	getnstimeofday(&ts12);
        printk("deleted %d", deletes);
	return 0;
	spin_unlock(&splock_search);
}

int __init hello_module_init(void)
{
	/* initialize list */
	INIT_LIST_HEAD(&my_list);
	// spin_lock_init(&splock_insert);
	spin_lock_init(&splock_search);
	// spin_lock_init(&splock_delete);

	inserts = 0;
	searches = 0;
	deletes = 0;

	printk("Linked List time measurement start!\n");
	
	printk("insert start");
	n = 100000;
	getnstimeofday(&ts1);
        list_insert();

	printk("search 1 start");
	search_type = 0;
	getnstimeofday(&ts3);
	list_search();

	printk("search 2 start");
	search_type = 1;
	getnstimeofday(&ts5);
	list_search_backward();

	printk("search 3 start");
	search_type = 2;
	getnstimeofday(&ts7);
        list_search_all();

	printk("search 4 start");
	getnstimeofday(&ts9);
        list_search_past();

	printk("delete start");
	getnstimeofday(&ts11);
        list_delete();
	return 0;
}

void __exit hello_module_cleanup(void)
{
	long long temp1, temp2, temp3, temp4, temp5 , temp6;//, temp7, temp8, temp9;
	temp1 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
	temp2 = (ts4.tv_sec - ts3.tv_sec) * 1000000000 + (ts4.tv_nsec - ts3.tv_nsec);
	temp3 = (ts6.tv_sec - ts5.tv_sec) * 1000000000 + (ts6.tv_nsec - ts5.tv_nsec);
	temp4 = (ts8.tv_sec - ts7.tv_sec) * 1000000000 + (ts8.tv_nsec - ts7.tv_nsec);
	temp5 = (ts10.tv_sec - ts9.tv_sec) * 1000000000 + (ts10.tv_nsec - ts9.tv_nsec);
	temp6 = (ts12.tv_sec - ts11.tv_sec) * 1000000000 + (ts12.tv_nsec - ts11.tv_nsec);

        printk("insert time        : %16lld nanosec\n", temp1);
	printk("search time front  : %16lld nanosec\n", temp2);
        printk("search time back   : %16lld nanosec\n", temp3);
	printk("search time all    : %16lld nanosec\n", temp4);
	printk("search time past   : %16lld nanosec\n", temp5);
        printk("delete time        : %16lld nanosec\n", temp6);
}
module_init(hello_module_init);
module_exit(hello_module_cleanup);
MODULE_LICENSE("GPL");
