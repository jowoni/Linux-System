#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h> // for kmalloc
#include <linux/time.h>
#include <linux/timer.h>

struct my_node {
	struct list_head list;
	int data;
};

void struct_example(void)
{
	struct list_head my_list;
	/* initialize list */
	INIT_LIST_HEAD(&my_list);
	int i;
	struct my_node *tmp; /* need for delete */
	struct my_node *current_node; 
	/* This will point on the actual data structures during the iteration */
	struct timespec ts1, ts2;
	long long temp;

	int n = 1000;
	/* list element add */
	getnstimeofday(&ts1);
	for(i = 0; i<n;i++){
		struct my_node *new = kmalloc(sizeof(struct my_node),GFP_KERNEL);
		new->data = i;
		list_add(&new->list, &my_list);
	}
	getnstimeofday(&ts2);
	long long temp1 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
	//printk("insert %d elements  : ", n);
	//printk("%16lld nanosec\n", temp);

	/* list element search */
	getnstimeofday(&ts1);
	for(i = 0; i<n; i++){
		list_for_each_entry(current_node, &my_list, list){
			if(current_node->data == i){
				//printk("current value : %d\n", current_node->data);
				break;
			}
        	}
	}
	getnstimeofday(&ts2);
        long long temp2 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("search %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);

	/* list element delete */
	getnstimeofday(&ts1);
        list_for_each_entry_safe(current_node, tmp, &my_list, list){	
                //printk("current node value : %d\n", current_node->data);
                list_del(&current_node->list);
                kfree(current_node);
		continue;                	
        }	
	getnstimeofday(&ts2);
        long long temp3 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("delete %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);
	
	n = 10000;
/* list element add */
        getnstimeofday(&ts1);
        for(i = 0; i<n;i++){
                struct my_node *new = kmalloc(sizeof(struct my_node),GFP_KERNEL);
                new->data = i;
                list_add(&new->list, &my_list);
        }
        getnstimeofday(&ts2);
        long long temp4 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("insert %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);

        /* list element search */
        getnstimeofday(&ts1);
        for(i = 0; i<n; i++){
                list_for_each_entry(current_node, &my_list, list){
                        if(current_node->data == i){
                                //printk("current value : %d\n", current_node->data);
                                break;
                        }
                }
        }
        getnstimeofday(&ts2);
        long long temp5 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("search %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);

        /* list element delete */
        getnstimeofday(&ts1);
        list_for_each_entry_safe(current_node, tmp, &my_list, list){
                //printk("current node value : %d\n", current_node->data);
                list_del(&current_node->list);
                kfree(current_node);
                continue;
        }
        getnstimeofday(&ts2);
        long long temp6 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("delete %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);	

	n = 100000;
	/* list element add */
        getnstimeofday(&ts1);
        for(i = 0; i<n;i++){
                struct my_node *new = kmalloc(sizeof(struct my_node),GFP_KERNEL);
                new->data = i;
                list_add(&new->list, &my_list);
        }
        getnstimeofday(&ts2);
        long long temp7 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("insert %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);

        /* list element search */
        getnstimeofday(&ts1);
        for(i = 0; i<n; i++){
                list_for_each_entry(current_node, &my_list, list){
                        if(current_node->data == i){
                                //printk("current value : %d\n", current_node->data);
                                break;
                        }
                }
        }
        getnstimeofday(&ts2);
        long long temp8 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        //printk("search %d elements  : ", n);
        //printk("%16lld nanosec\n", temp);

        /* list element delete */
        getnstimeofday(&ts1);
        list_for_each_entry_safe(current_node, tmp, &my_list, list){
                //printk("current node value : %d\n", current_node->data);
                list_del(&current_node->list);
                kfree(current_node);
                continue;
        }
        getnstimeofday(&ts2);
        long long temp9 = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
	
	n = 1000;
        printk("insert %d elements  : ", n);
        printk("%16lld nanosec\n", temp1);
	printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp2);   
	printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp3);   
		
	n = 10000;
	printk("insert %d elements  : ", n);
        printk("%16lld nanosec\n", temp4);       
        printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp5);
        printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp6);

	n = 100000;
	printk("insert %d elements  : ", n);
        printk("%16lld nanosec\n", temp7);       
        printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp8);
        printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp9);

}

int __init hello_module_init(void)
{
	printk("Linked List time measurement start!\n");
        struct_example();
        return 0;
}

void __exit hello_module_cleanup(void)
{
        printk("Bye Module\n");
}
module_init(hello_module_init);
module_exit(hello_module_cleanup);
