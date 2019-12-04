#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rbtree.h>
#include <linux/slab.h> // for kmalloc
#include <linux/time.h>
#include <linux/timer.h>

#define FALSE 0
#define TRUE 1

struct my_type {
        struct rb_node node;
        int key;
        int value;
};


int rb_insert(struct rb_root *root, struct my_type *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	/* Figure out "where" to put new node */
	while (*new) {
		struct my_type *this = container_of(*new, struct my_type, node);
		parent = *new;
		if (this->key > data->key)
			new = &((*new)->rb_left);
		else if (this->key < data->key)
			new = &((*new)->rb_right);
		else
			return FALSE;
	}
	rb_link_node(&data->node, parent, new); /*relinking*/
	rb_insert_color(&data->node, root);/*recoloring & rebalancing*/
	return TRUE;
}

struct my_type *rb_search(struct rb_root *root, int key)
{
	struct rb_node *node = root->rb_node;
	while (node) {
			struct my_type *data = container_of(node, struct my_type, node);
		if(data->key > key)
			node = node->rb_left;
		else if(data->key < key)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

int rb_delete(struct rb_root *mytree, int key)
{
	struct my_type *data = rb_search(mytree, key);
	if (data) {
		rb_erase(&data->node, mytree);
		kfree(data);
	}
}
void struct_exmaple(void)
{
	struct rb_root my_tree = RB_ROOT;
	int i = 0, ret;
	int n;
	struct timespec ts1, ts2;
	long long temp;

	n = 1000;
	/* rb_node create and insert */
	getnstimeofday(&ts1);
	for(; i < n;i++){
		struct my_type *new =kmalloc(sizeof(struct my_type),GFP_KERNEL);
		if(!new)
			return NULL;
		new->value = i*10;
		new->key = i;
		ret = rb_insert(&my_tree,new);
	}
	getnstimeofday(&ts2);
	temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
	printk("insert %d elements  : ", n);
	printk("%16lld nanosec\n", temp);
	/* rb_tree find node */
	getnstimeofday(&ts1);
	struct my_type *find_node = NULL;
	for(i = 0; i < n; i++){
		find_node = rb_search(&my_tree, i);
		if(!find_node){
			printk("ERROR AT FIND %d", n);
			return NULL;
		}
	}
	//printk("find : (key,value)=(%d.%d)\n", find_node->key,find_node->value);
	getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp);

	/* rb_tree delete node */
	getnstimeofday(&ts1);
	for(i = n; i > 0; i--){
		rb_delete(&my_tree,0);
	}
	getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp);

	n = 10000;
        /* rb_node create and insert */
        getnstimeofday(&ts1);
        for(; i < n;i++){
                struct my_type *new =kmalloc(sizeof(struct my_type),GFP_KERNEL);
                if(!new)
                        return NULL;
                new->value = i*10;
                new->key = i;
                ret = rb_insert(&my_tree,new);
        }
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("insert %d elements  : ", n);
        printk("%16lld nanosec\n", temp);
        /* rb_tree find node */
        getnstimeofday(&ts1);
        find_node = NULL;
        for(i = 0; i < n; i++){
                find_node = rb_search(&my_tree, i);
                if(!find_node){
                        printk("ERROR AT FIND %d", n);
                        return NULL;
                }
        }
        //printk("find : (key,value)=(%d.%d)\n", find_node->key,find_node->value);
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp);

        /* rb_tree delete node */
        getnstimeofday(&ts1);
        for(i = n; i > 0; i--){
                rb_delete(&my_tree,0);
        }
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp);

	n = 100000;
        /* rb_node create and insert */
        getnstimeofday(&ts1);
        for(; i < n;i++){
                struct my_type *new =kmalloc(sizeof(struct my_type),GFP_KERNEL);
                if(!new)
                        return NULL;
                new->value = i*10;
                new->key = i;
                ret = rb_insert(&my_tree,new);
        }
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("insert %d elements  : ", n);
        printk("%16lld nanosec\n", temp);
        /* rb_tree find node */
        getnstimeofday(&ts1);
        find_node = NULL;
        for(i = 0; i < n; i++){
                find_node = rb_search(&my_tree, i);
                if(!find_node){
                        printk("ERROR AT FIND %d", n);
                        return NULL;
                }
        }
        //printk("find : (key,value)=(%d.%d)\n", find_node->key,find_node->value);
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("search %d elements  : ", n);
        printk("%16lld nanosec\n", temp);

        /* rb_tree delete node */
        getnstimeofday(&ts1);
        for(i = n; i > 0; i--){
                rb_delete(&my_tree,0);
        }
        getnstimeofday(&ts2);
        temp = (ts2.tv_sec - ts1.tv_sec) * 1000000000 + (ts2.tv_nsec - ts1.tv_nsec);
        printk("delete %d elements  : ", n);
        printk("%16lld nanosec\n", temp);


}

int __init hello_module_init(void)
{
	printk("RBT time measurement start!\n");
        struct_exmaple();
        return 0;
}
void __exit hello_module_cleanup(void)
{
        printk("Bye Module\n");
}
module_init(hello_module_init);
module_exit(hello_module_cleanup);

