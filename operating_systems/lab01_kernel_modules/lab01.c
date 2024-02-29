#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Syed Naqvi:100590852");
MODULE_DESCRIPTION("100590852:This is Syed's first Kernel module.");

static int __init my_init_Function(void) {

    printk(KERN_INFO "Welcome to System Programming.\n");
    return 0;

}

static void __exit my_exit_function(void) {

    printk(KERN_INFO "Cleaning up the Kernel Module.\n");
}

module_init(my_init_Function);
module_exit(my_exit_function);