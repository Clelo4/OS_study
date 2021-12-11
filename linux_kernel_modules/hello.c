#include <linux/module.h>
#include <linux/printk.h>

/* This function is called when the module is loaded. */
int hello(void) 
{
        printk(KERN_INFO "Loading module... Hello World!\n");

        return 0;
}

/* This function is called when the module is removed. */
void goodbye(void) 
{
        printk(KERN_INFO "Removing module... Goodbye World!\n");
}

/* Macros for registering module entry and exit points */
module_init( hello );
module_exit( goodbye );

/* Macros for declaring module metadata */
MODULE_DESCRIPTION("A basic Hello World module");
MODULE_AUTHOR("Jack");
MODULE_LICENSE("GPL");
