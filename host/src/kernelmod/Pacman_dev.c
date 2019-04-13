/*
 * GNU GENERAL PUBLIC LICENSE
 * Version 1, Mar 2019
 *
 * (C) 2019, Minghao Cheng <chengminghao92@gmail.com>
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/poll.h>

#define Pacman_major 123
#define DEV_NAME "Pacman_dev"

#define RESET_PIN               2
#define INTA_PIN                4
#define INTB_PIN                3

#define KEY_LEFT_PIN            5
#define KEY_UP_PIN              6
#define KEY_RIGHT_PIN           13
#define KEY_DOWN_PIN            19
#define KEY_RESET_PIN           26

volatile char INT_flag = 0;
volatile char KEY_flag = 0;
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
    if(KEY_flag)
    {
        // copy data from kernel space to user space
        status = copy_to_user(buf, (char *)&KEY_flag, 1);
        KEY_flag = 0;
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
    uint8_t val;
    if(0 == copy_from_user(&val, buf, 1))
    {
        //printk(KERN_INFO "writing to GPIO2 = %x\n", val);
        gpio_set_value(RESET_PIN, val);
    }
    return 0;
}

static unsigned int gpio_poll(struct file *file, poll_table *wait)
{
    unsigned int ret = 0;
    poll_wait(file, &INT_waitq, wait);

    if(INT_flag)
    {
        // mind to use POLLPRI in userspace
        ret |= POLLPRI;
    }
    if(KEY_flag)
    {
        ret |= POLLRDBAND;
    }
    return ret;
}

static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    //printk(KERN_INFO "inside gpio ioctl\n");
    return 0;
}

static const struct file_operations Pacman_fops =
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
    //////////////////
    if(irq == gpio_to_irq(INTA_PIN))
    {
        INT_flag |= 0x01;
    }
    else if(irq == gpio_to_irq(INTB_PIN))
    {
        INT_flag |= 0x02;
    }
    else if(irq == gpio_to_irq(KEY_UP_PIN))
    {
        KEY_flag |= 0x01;
    }
    else if(irq == gpio_to_irq(KEY_DOWN_PIN))
    {
        KEY_flag |= 0x02;
    }
    else if(irq == gpio_to_irq(KEY_LEFT_PIN))
    {
        KEY_flag |= 0x03;
    }
    else if(irq == gpio_to_irq(KEY_RIGHT_PIN))
    {
        KEY_flag |= 0x04;
    }
    else if(irq == gpio_to_irq(KEY_RESET_PIN))
    {
        KEY_flag |= 0x05;
    }
    printk(KERN_INFO "INT KEY_flag = %x\n", KEY_flag);
    wake_up_interruptible(&INT_waitq);
    return IRQ_HANDLED;
}
/**********************end of IRQ handler***********************/
static struct class *Pacman_dev_class;
static struct device *Pacman_dev_device;
static __init int Pacman_dev_init(void)
{
    int err;
    int status;

    // request GPIO pins
        // Request INT pins
    err = gpio_request_one(INTA_PIN, GPIOF_IN, "Pacman_dev_INTA");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev PIN INTA request failed\n");
        return err;
    }
    err = gpio_request_one(INTB_PIN, GPIOF_IN, "Pacman_dev_INTB");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev PIN INTB request failed\n");
        return err;
    }
    err = gpio_request_one(RESET_PIN, GPIOF_OUT_INIT_LOW, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev PIN RESET request failed\n");
        return err;
    }
        // Request KEYboard pins
    err = gpio_request_one(KEY_LEFT_PIN, GPIOF_IN, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev KEY_LEFT_PIN request failed\n");
        return err;
    }
    err = gpio_request_one(KEY_UP_PIN, GPIOF_IN, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev KEY_UP_PIN request failed\n");
        return err;
    }
    err = gpio_request_one(KEY_RIGHT_PIN, GPIOF_IN, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev KEY_RIGHT_PIN request failed\n");
        return err;
    }
    err = gpio_request_one(KEY_DOWN_PIN, GPIOF_IN, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev KEY_DOWN_PIN request failed\n");
        return err;
    }
    err = gpio_request_one(KEY_RESET_PIN, GPIOF_IN, "Pacman_dev_RESET");
    if (err)
    {
        printk(KERN_INFO "Pacman_dev KEY_RESET_PIN request failed\n");
        return err;
    }
    // enable interrupt& register a callback function
    enable_irq(gpio_to_irq(INTA_PIN));
    err = request_irq(gpio_to_irq(INTA_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev irq_A_request setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(INTB_PIN));
    err = request_irq(gpio_to_irq(INTB_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev irq_B_request setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(KEY_UP_PIN));
    err = request_irq(gpio_to_irq(KEY_UP_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev KEY_UP_PIN IRQ request setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(KEY_DOWN_PIN));
    err = request_irq(gpio_to_irq(KEY_DOWN_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev KEY_DOWN_PIN IRQ setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(KEY_LEFT_PIN));
    err = request_irq(gpio_to_irq(KEY_LEFT_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev KEY_LEFT_PIN IRQ setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(KEY_RIGHT_PIN));
    err = request_irq(gpio_to_irq(KEY_RIGHT_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev KEY_RIGHT_PIN IRQ setting failed!\n");
        return err;
    }
    enable_irq(gpio_to_irq(KEY_RESET_PIN));
    err = request_irq(gpio_to_irq(KEY_RESET_PIN), irq_handler, IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
    if (err < 0)
    {
        printk("Pacman_dev KEY_RESET_PIN IRQ setting failed!\n");
        return err;
    }
    // register as a character device
    status = register_chrdev(Pacman_major, DEV_NAME, &Pacman_fops);
    if (status < 0)
    {
        return status;
    }
    // create a device class
    Pacman_dev_class = class_create(THIS_MODULE, "Pacman_dev");
    // create a device of the class
    Pacman_dev_device = device_create(Pacman_dev_class, NULL, MKDEV(Pacman_major,0), NULL, "Pacman_dev");
    printk(KERN_INFO "Pacman_dev init successful\n");
    return 0;
}

static __exit void Pacman_dev_exit(void)
{
    // free the interrupt
    free_irq(gpio_to_irq(INTA_PIN), NULL);
    free_irq(gpio_to_irq(INTB_PIN), NULL);
    free_irq(gpio_to_irq(KEY_UP_PIN), NULL);
    free_irq(gpio_to_irq(KEY_DOWN_PIN), NULL);
    free_irq(gpio_to_irq(KEY_LEFT_PIN), NULL);
    free_irq(gpio_to_irq(KEY_RIGHT_PIN), NULL);
    free_irq(gpio_to_irq(KEY_RESET_PIN), NULL);
    // release the pin
    gpio_free(INTA_PIN);
    gpio_free(INTB_PIN);
    gpio_free(RESET_PIN);
    gpio_free(KEY_UP_PIN);
    gpio_free(KEY_DOWN_PIN);
    gpio_free(KEY_LEFT_PIN);
    gpio_free(KEY_RIGHT_PIN);
    gpio_free(KEY_RESET_PIN);
    // unregister device, class & character deivce
    device_unregister(Pacman_dev_device);
    class_destroy(Pacman_dev_class);
    unregister_chrdev(Pacman_major, DEV_NAME);
    printk(KERN_INFO "Pacman_dev exit successful\n");
}


module_init(Pacman_dev_init);
module_exit(Pacman_dev_exit);

MODULE_AUTHOR("M.CHENG");
MODULE_LICENSE("Dual BSD/GPL");
