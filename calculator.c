#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mml");
MODULE_DESCRIPTION("A Simple kernel calc World module");

static int __init kernel_calc_init(void)
{
    printk(KERN_INFO "Hello kernel_calc world!\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit kernel_calc_cleanup(void)
{
	//if (ret != 0)
		//pr_err("cannot unregister \n");
    //pr_debug("hellofs module unloaded\n");
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(kernel_calc_init);
module_exit(kernel_calc_cleanup);
