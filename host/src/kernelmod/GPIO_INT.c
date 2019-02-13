#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/poll.h>

#define GPIO_MAJOR 123
#define DEV_NAME "GPIO_INT"


#define INT_PIN 16

volatile char INT_flag = 0;
DECLARE_WAIT_QUEUE_HEAD(INT_waitq);    //declear wait queue head for poll interface

/********************file system operation**********************/
static ssize_t gpio_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int status;
    if(INT_flag)
    {
        // copy data from kernel space to user space
        status = copy_to_user(buf, (char *)&INT_flag, 1);
        INT_flag = 0;
        if(status == -1)
        {
            printk(KERN_INFO "error when copying GPIO INT status to user space\n");
            return -1;
        }
        return 1;
    }
    return 0;
}

static ssize_t gpio_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    //printk(KERN_INFO "inside gpio write\n");
    return 0;
}

static unsigned int gpio_poll(struct file *file, poll_table *wait)
{
    unsigned int ret = 0;
    poll_wait(file, &INT_waitq, wait);

    if(INT_flag)
    {
        // mind to use POLLPRI in userspace
        ret |=POLLPRI;
    }
    return ret;
}

static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //printk(KERN_INFO "inside gpio ioctl\n");
    return 0;
}

static const struct file_operations gpio_fops =
{
    .owner = THIS_MODULE,
    .read  = gpio_read,
    .write = gpio_write,
    .poll  = gpio_poll,
    .unlocked_ioctl = gpio_ioctl,
};
/**********************end of fsop******************************/
/**********************IRQ handler******************************/
static irqreturn_t irq_handler(int irq, void *dev)
{
    INT_flag = 1;
    wake_up_interruptible(&INT_waitq);
    return IRQ_HANDLED;
}
/**********************end of IRQ handler***********************/
static struct class *GPIO_class;
static struct device *GPIO_device;
static __init int gpio_init(void)
{
    int err;
    int status;

    printk(KERN_INFO "GPIO_INT init\n");
    // request a GPIO in gpio bus
    err = gpio_request_one(INT_PIN, GPIOF_IN, "GPIO_INT");
    if (err)
    {
        return err;
    }
    // enable interrupt& registera a callback function
    enable_irq(gpio_to_irq(INT_PIN));
    err = request_irq(gpio_to_irq(INT_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("GPIO_INT irq_request setting failed!\n");
        return err;
    }
    // register as a character device
    status = register_chrdev(GPIO_MAJOR, DEV_NAME, &gpio_fops);
    if (status < 0)
    {
        return status;
    }
    // create a device class
    GPIO_class = class_create(THIS_MODULE, "GPIO_INT");
    // create a device of the class
    GPIO_device = device_create(GPIO_class, NULL, MKDEV(GPIO_MAJOR,0), NULL, "GPIO_INT");

    return 0;
}

static __exit void gpio_exit(void)
{
    printk(KERN_INFO "GPIO_INT exit\n");
    // free the interrupt
    free_irq(gpio_to_irq(INT_PIN), NULL);
    // release the pin
    gpio_free(INT_PIN);
    // unregister device, class & character deivce
    device_unregister(GPIO_device);
    class_destroy(GPIO_class);
    unregister_chrdev(GPIO_MAJOR, DEV_NAME);
}


module_init(gpio_init);
module_exit(gpio_exit);

MODULE_AUTHOR("M.CHENG");
MODULE_LICENSE("Dual BSD/GPL");
