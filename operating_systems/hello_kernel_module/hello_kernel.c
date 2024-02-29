#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");

static int __init my_init_Function(void) {

    printk(KERN_ALERT "Initializing my first kernel module.\n");
    return 0;

}

static void __exit my_exit_function(void) {

    printk(KERN_ALERT "Exiting from my first kernel module.\n");
}

module_init(my_init_Function);
module_exit(my_exit_function);