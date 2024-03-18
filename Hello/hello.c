
#include <linux/fs.h>
#include <linux/cdev.h>

#include <linux/module.h>
#include <linux/init.h>

//for module parameter
#include <linux/moduleparam.h>

//create device driver Number automatically
//-----------------------------------------------------------------------
dev_t device_number;
//-----------------------------------------------------------------------

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
    int retval;

    ///create device driver Number automatically
    //-----------------------------------------------------------------------
    //set the Minor number you choose in the least 20 bits in the "device number" variable
    //set the Major number you choose in the rest bits in the "device number" variable
    retval = alloc_chrdev_region ( &device_number, 0, 1, "Gihad_SUDO_DRIVER");
    if (retval == 0)
    {
        //print the function name.

        //print the Major Number from "device number" variable.
        //print the Minor Number from the least 20 bits in the "device number" variable.
        /*from the Major and Minor Macroes header file:
           #define MINORBITS	20
           #define MINORMASK	((1U << MINORBITS) - 1)

           #define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
           #define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
        */
        
        printk ("%s retval = 0 - registered Device number Major: %d, Minor: %d\n" , __FUNCTION__, MAJOR(device_number), MINOR(device_number));
    }
    else
    {
        printk ("could not create a number for the device !!");
        return -1;
    }
    //-----------------------------------------------------------------------

    for (i = 0; i < cnt; i++)
        printk ("Hello kernel !!\n");
        
    return 0;
}

static void __exit driver_hello_exit (void)
{
    //distroy device driver Number automatically function
    //-----------------------------------------------------------------------
    unregister_chrdev_region (device_number, 1);
    //-----------------------------------------------------------------------

    int i = 0;
    for (i = 0; i < cnt; i++)
        printk ("Bye :)\n");
}

module_init (driver_hello_init);

module_exit (driver_hello_exit);
