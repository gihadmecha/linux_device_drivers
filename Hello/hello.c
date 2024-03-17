
#include <linux/module.h>
#include <linux/init.h>

//for module parameter
#include <linux/moduleparam.h>

//module informations
//-----------------------------------------------------------------------
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gihad Mohamed");
MODULE_DESCRIPTION("A hello world Psuedo device driver");
//-----------------------------------------------------------------------

//Module parameter
//-----------------------------------------------------------------------
// cnt initial value
int cnt = 0;

//let parameter folder is invisible
//module_param (cnt, int, 0);
//s: stat
//I: inode as as a linux header
//RUSR:read user
//WUSR:write user
//RGRP:read group
//WGRP:write group
module_param (cnt, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

//descripe cnt parameter in modinfo
MODULE_PARM_DESC (cnt, "An integer");
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
static int __init driver_hello_init (void)
{
    int i = 0;
    for (i = 0; i < cnt; i++)
        printk ("Hello kernel !!\n");
    return 0;
}

static void __exit driver_hello_exit (void)
{
    int i = 0;
    for (i = 0; i < cnt; i++)
        printk ("Bye :)\n");
}

module_init (driver_hello_init);

module_exit (driver_hello_exit);
