
#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Moatasem ");
MODULE_DESCRIPTION("A hello world Psuedo device driver");

static int __init driver_hello_init (void)
{
    printk ("Hello kernel !!\n");
    return 0;
}

static void __exit driver_hello_exit (void)
{
    printk ("Bye :)\n");
}

module_init (driver_hello_init);

module_exit (driver_hello_exit);
