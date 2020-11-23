#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/ioctl.h>

#define DRIVER_AUTHOR "CAUSW Byeonggon Lee"
#define DRIVER_DESC "driver for motor"
#define MOTOR_NAME "motor"
#define MOTOR_MODULE_VERSION "motor V1.0"
#define MOTOR_ACTION_ADDR 0x00C
#define MOTOR_DIRECTION_ADDR 0x00E
#define MOTOR_SPEED_ADDR 0x010

#define MOTOR_MAGIC 0xDB
#define MOTOR_SET_ACTION _IOW(MOTOR_MAGIC, 0, int)
#define MOTOR_SET_DIRECTION _IOW(MOTOR_MAGIC, 0, int)
#define MOTOR_SET_SPEED _IOW(MOTOR_MAGIC, 0, int)

extern ssize_t iom_fpga_itf_read(unsigned int addr);
extern ssize_t iom_fpga_itf_write(unsigned int addr, unsigned short int value);

static int motor_in_use = 0;

int motor_open(struct inode *pinode, struct file *pfile)
{
	if (motor_in_use != 0) {
		return -EBUSY;
	}
	motor_in_use = 1;
	return 0;
}

int motor_release(struct inode *pinode, struct file *pfile)
{
	motor_in_use = 0;
	return 0;
}

static long motor_ioctl(struct file * pinode, unsigned int cmd, unsigned long data)
{
	int ret, param;

	printk(KERN_ERR "MOTOR_SET_ACTION");
	switch (cmd) {
	case MOTOR_SET_ACTION:
		ret = copy_from_user(&param, (void*) data, _IOC_SIZE(MOTOR_SET_ACTION));
		if (ret < 0) {
			printk(KERN_ERR "data copy from userspace failed\n");
			return -EFAULT;
		}
        iom_fpga_itf_write(MOTOR_ACTION_ADDR, param);
		break;
    case MOTOR_SET_DIRECTION:
		ret = copy_from_user(&param, (void*) data, _IOC_SIZE(MOTOR_SET_DIRECTION));
		if (ret < 0) {
			printk(KERN_ERR "data copy from userspace failed\n");
			return -EFAULT;
		}
        iom_fpga_itf_write(MOTOR_DIRECTION_ADDR, param);
        break;
    case MOTOR_SET_SPEED:
		ret = copy_from_user(&param, (void*) data, _IOC_SIZE(MOTOR_SET_SPEED));
		if (ret < 0) {
			printk(KERN_ERR "data copy from userspace failed\n");
			return -EFAULT;
		}
        iom_fpga_itf_write(MOTOR_SPEED_ADDR, param);
        break;
	default:
		printk(KERN_ERR "SOMETHING WRONG!");
	}
	return 0;
}

static struct file_operations motor_fops = {
	.owner = THIS_MODULE,
	.open = motor_open,
	.unlocked_ioctl = motor_ioctl,
	.release = motor_release
};

static struct miscdevice motor_driver = {
	.fops = &motor_fops,
	.name = MOTOR_NAME,
	.minor = MISC_DYNAMIC_MINOR
};

int motor_init(void)
{
	misc_register(&motor_driver);
	printk(KERN_INFO "driver: %s driver init\n", MOTOR_NAME);
	return 0;
}

void motor_exit(void)
{
	misc_deregister(&motor_driver);
	printk(KERN_INFO "driver: %s driver exit\n", MOTOR_NAME);
}

module_init(motor_init);
module_exit(motor_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");
