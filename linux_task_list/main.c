/**
 * @file main.c
 * @author Jack
 * @mail chengjunjie.jack@gmail.com
 * @date 2021-12-12
 * @version 0.1
 *
 * @copyright Copyright (c) 2021
 */

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/list.h>

extern struct task_struct init_task;

void print_task(struct task_struct *, int);

void print_task(struct task_struct *task, int offset) {
	struct list_head *bro = &task->children;
	struct list_head *posi;
	list_for_each(posi, bro) {
		typeof(struct task_struct *) entry = list_entry(posi, struct task_struct, sibling);
		printk("%*s\\_ [%d] %s\n", offset, " ", entry->pid, entry->comm);
		print_task(entry, offset + 4);
	}
}

int m_init(void) {
	printk("Loading Module\n");
	printk("-- [%d] %s", init_task.pid, init_task.comm);
	print_task(&init_task, 4); 
	return 0;
}

void m_exit(void) {
}

module_init(m_init);
module_exit(m_exit);

MODULE_DESCRIPTION("A basic module for study");
MODULE_AUTHOR("Jack");
MODULE_LICENSE("GPL");

