
//for alloc_chrdev_region function, unregister_chrdev_region function, Major macro and Minor macro
#include <linux/fs.h>

//define driver as character, block or network
#include <linux/cdev.h>

#include <linux/module.h>
#include <linux/init.h>

//for module parameter
#include <linux/moduleparam.h>

//create device driver Number automatically
//-----------------------------------------------------------------------
//variable to set Major and Minor numbers in it.
dev_t device_number;
//-----------------------------------------------------------------------

static int driver_open (struct inode* device_file, struct file* instance)
{
    printk ("device file was opened !!");
    return 0;
}

static int driver_close (struct inode* device_file, struct file* instance)
{
    printk ("device file was closed !!");
    return 0;
}

struct file_operations file_operations =
{
    //this file operation struct for this module
    .owner = THIS_MODULE,

    .open = driver_open,
    .release = driver_close
};

//add it as character device
struct cdev strut_characterDevice;

//to create file in the user space
struct device* pointer_struct_device;
struct class* pointer_struct_class;

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

    //1-create device driver numbers automatically
    //-----------------------------------------------------------------------
    //set the Minor number you choose in the least 20 bits in the "device number" variable
    //set the Major number you choose in the rest bits in the "device number" variable
    retval = alloc_chrdev_region ( &device_number, 0, 1, "Gihad_SUDO_DRIVER");
    if (retval == 0)
    {
        //print the function name "driver_hello_init".

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
        printk ("could not create a number for the device !!\n");
        return -1;
    }
    //-----------------------------------------------------------------------

    //2-define driver as character, block or network
    //-----------------------------------------------------------------------
    //add it as a character device
    cdev_init(&strut_characterDevice, &file_operations);
    retval = cdev_add (&strut_characterDevice, device_number, 1);
    if (retval != 0)
    {
        //unregister_chrdev_region (device_number, 1);

        printk ("couldn't add the Hello module as a character module !!\n");
        
        //return -1;

        goto CHARACTER_ERROR;
    }
    //-----------------------------------------------------------------------

    //3-create file in the user space (/dev)
    //to handle this module from workspace
    //-----------------------------------------------------------------------
    //create class in /sys/class/ for file attributes 
    pointer_struct_class = class_create("GIHAD_class");
    if (pointer_struct_class == NULL)
    {

        //unregister_chrdev_region (device_number, 1);
        //cdev_del (&strut_characterDevice);

        printk ("couldn't create class !!\n");

        goto CLASS_ERROR;

        //return -1;
    }

    pointer_struct_device = device_create(pointer_struct_class, NULL, device_number, NULL, "GIHAD_file");
    if (pointer_struct_device == NULL)
    {

        //unregister_chrdev_region (device_number, 1);
        //cdev_del (&strut_characterDevice);
        //class_destroy(pointer_struct_class);

        printk ("couldn't create file !!\n");

        //return -1;

        goto FILE_ERROR;
    }
    //-----------------------------------------------------------------------

    for (i = 0; i < cnt; i++)
        printk ("Hello kernel !!\n");


    printk ("driver is created :)\n");

    return 0;

    

    FILE_ERROR:
    class_destroy(pointer_struct_class);
    CLASS_ERROR:
    cdev_del (&strut_characterDevice);
    CHARACTER_ERROR:
    unregister_chrdev_region (device_number, 1);

    return -1;
}

static void __exit driver_hello_exit (void)
{
    /**
     * cdev_del() - remove a cdev from the system
     * @p: the cdev structure to be removed
     *
     * cdev_del() removes @p from the system, possibly freeing the structure
     * itself.
     *
     * NOTE: This guarantees that cdev device will no longer be able to be
     * opened, however any cdevs already open will remain and their fops will
     * still be callable even after cdev_del returns.
     */
    cdev_del (&strut_characterDevice);

    //destroy file
    device_destroy (pointer_struct_class, device_number);

    //distroy device driver Number automatically function
    //-----------------------------------------------------------------------
    unregister_chrdev_region (device_number, 1);
    //-----------------------------------------------------------------------

    //destroy class
    class_destroy(pointer_struct_class);

    int i = 0;
    for (i = 0; i < cnt; i++)
        printk ("Bye :)\n");
}

module_init (driver_hello_init);

module_exit (driver_hello_exit);
