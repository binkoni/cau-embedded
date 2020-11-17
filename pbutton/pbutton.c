#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/ioctl.h>

#define DRIVER_AUTHOR "CAUSW Byeonggon Lee"
#define DRIVER_DESC "driver for pbutton"
#define PBUTTON_NAME "pbutton"
#define PBUTTON_MODULE_VERSION "pbutton V1.0"
#define PBUTTON_ADDR 0x050
#define PBUTTON_CNT 9

extern ssize_t iom_fpga_itf_read(unsigned int addr);
extern ssize_t iom_fpga_itf_write(unsigned int addr, unsigned short int value);

static int pbutton_in_use = 0;

int pbutton_open(struct inode *pinode, struct file *pfile)
{
	if (pbutton_in_use != 0) {
		return -EBUSY;
	}
	pbutton_in_use = 1;
	return 0;
}

int pbutton_release(struct inode *pinode, struct file *pfile)
{
	pbutton_in_use = 0;
	return 0;
}

ssize_t pbutton_read(struct file *pinode, char *gdata, size_t len, loff_t *off_what)
{
	unsigned char clicked[PBUTTON_CNT];
	int i;

	for (i = 0; i < PBUTTON_CNT; ++i) {
		clicked[i] = iom_fpga_itf_read(PBUTTON_ADDR + i * 2);
	}

	if (copy_to_user(gdata, clicked, PBUTTON_CNT * sizeof(unsigned char))) {
		return -EFAULT;
	}

	return PBUTTON_CNT * sizeof(unsigned char);
}

static struct file_operations pbutton_fops = {
	.owner = THIS_MODULE,
	.open = pbutton_open,
	.read = pbutton_read,
	.release = pbutton_release
};

static struct miscdevice pbutton_driver = {
	.fops = &pbutton_fops,
	.name = PBUTTON_NAME,
	.minor = MISC_DYNAMIC_MINOR
};

int pbutton_init(void)
{
	misc_register(&pbutton_driver);
	printk(KERN_INFO "driver: %s driver init\n", PBUTTON_NAME);
	return 0;
}

void pbutton_exit(void)
{
	misc_deregister(&pbutton_driver);
	printk(KERN_INFO "driver: %s driver exit\n", PBUTTON_NAME);
}

module_init(pbutton_init);
module_exit(pbutton_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");
