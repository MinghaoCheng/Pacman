#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/poll.h>

#define GPIO_MAJOR 123
#define DEV_NAME "GPIO_INT"


#define BUTTON_PIN 16

volatile char INT_flag = 0;
DECLARE_WAIT_QUEUE_HEAD(INT_waitq);

/********************file system operation**********************/
static ssize_t gpio_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    int status;
    if(INT_flag)
    {
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
        ret |=POLLPRI;
    }
    return ret;
}

static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //printk(KERN_INFO "inside gpio ioctl\n");
    return 0;
}


static const struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
    .read  = gpio_read,
	.write = gpio_write,
    .poll  = gpio_poll, 
	.unlocked_ioctl = gpio_ioctl,
};
/**********************end of fsop******************************/
static irqreturn_t irq_handler(int irq, void *dev)
{
    INT_flag = 1;
    wake_up_interruptible(&INT_waitq);
    return IRQ_HANDLED;
}

static __init int gpio_init(void)
{
    int err;
    int status;
    
    printk(KERN_INFO "GPIO_INT init\n");
    
    err = gpio_request_one(BUTTON_PIN, GPIOF_IN, "Button");
    if (err) return err;

    enable_irq(gpio_to_irq(BUTTON_PIN));
    err = request_irq(gpio_to_irq(BUTTON_PIN), irq_handler, 
            IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0) {
        printk("GPIO_INT irq_request setting failed!\n");
        return err;
    }
    
    status = register_chrdev(GPIO_MAJOR, DEV_NAME, &gpio_fops);
	if (status < 0)
		return status;

    
    return 0;
}

static __exit void gpio_exit(void)
{
    printk(KERN_INFO "GPIO_INT exit\n");
    free_irq(gpio_to_irq(BUTTON_PIN), NULL);
    gpio_free(BUTTON_PIN);
    unregister_chrdev(GPIO_MAJOR, DEV_NAME);
}


module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("Dual BSD/GPL");
