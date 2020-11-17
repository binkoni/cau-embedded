#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/ioctl.h>

#define DRIVER_AUTHOR "CAUSW Byeonggon Lee"
#define DRIVER_DESC "driver for dotmatrix2"
#define DOTM_NAME "dotmatrix2"
#define DOTM_MODULE_VERSION "dotmatrix2 V1.0"
#define DOTM_ADDR 0x210
#define DOTM_COL_SIZE 7

#define DOTM_MAGIC 0xBC
#define DOTM_SET_ALL _IOW(DOTM_MAGIC, 0, int)
#define DOTM_SET_CLEAR _IOW(DOTM_MAGIC, 1, int)

extern ssize_t iom_fpag_itf_read(unsigned int addr);
extern ssize_t iom_fpga_itf_write(unsigned int addr, unsigned short int value);

static int dotm_in_use = 0;

unsigned char dotm_fontmap_letter[4][10] = {
	{0x00, 0x19, 0x25, 0x25, 0x25, 0x25, 0x25, 0x25, 0x19, 0x00},
	{0x00, 0x25, 0x3f, 0x25, 0x3f, 0x01, 0x0e, 0x11, 0x0e, 0x00},
	{0x00, 0x00, 0x1e, 0x02, 0x0a, 0x08, 0x3f, 0x10, 0x1e, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

unsigned char dotm_fontmap_full[10] = {
	0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
};

unsigned char dotm_fontmap_empty[10] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int dotm_open(struct inode *pinode, struct file *pfile)
{
	if (dotm_in_use != 0) {
		return -EBUSY;
	}
	dotm_in_use = 1;
	return 0;
}

int dotm_release(struct inode *pinode, struct file *pfile)
{
	dotm_in_use = 0;
	return 0;
}

ssize_t dotm_write(struct file *pinode, const char *gdata, size_t len, loff_t *off_what) {
	int ret, line, i;
	unsigned char args[3];
	int mode, offset;
	unsigned short int letterline;
	if (len != 3) {
		printk(KERN_INFO "data should be 3 bytes\n");
		return -EFAULT;
	}
	ret = copy_from_user(args, gdata, 3);
	if (ret < 0) {
		printk(KERN_INFO "Failed to copy from user");
		return -EFAULT;
	}
	mode = args[0] - '0';
	offset = (args[1] - '0') * 10 + (args[2] - '0');
	if (mode < 3) {
		for (i = 0; i < 10; ++i) {
			iom_fpga_itf_write(DOTM_ADDR + (i * 2), dotm_fontmap_letter[mode][i] & 0x7f);
		}
	} else if (mode == 3) { // vertical scrolling
		for (i = offset, line = 0; i < 30 && line < 10; ++i, ++line) {
			iom_fpga_itf_write(DOTM_ADDR + (line * 2), dotm_fontmap_letter[i / 10][i % 10] & 0x7f);
		}
	} else { // horizontal scrolling
		for (line = 0; line < 10; ++line) {
               		letterline = (dotm_fontmap_letter[offset / DOTM_COL_SIZE][line] << (offset % DOTM_COL_SIZE))
				| (dotm_fontmap_letter[offset / DOTM_COL_SIZE + 1][line] >> (DOTM_COL_SIZE - (offset % DOTM_COL_SIZE)))
				& 0x7f;
			iom_fpga_itf_write(DOTM_ADDR + (line * 2), letterline);
		}
	}
	return len;
}

static long dotm_ioctl(struct file *pinode, unsigned int cmd, unsigned long data) {
	int i;
	unsigned short wordvalue;
	switch (cmd) {
		case DOTM_SET_ALL:
			for (i = 0; i < 10; i++) {
				wordvalue = dotm_fontmap_full[i] & 0x7f;
				iom_fpga_itf_write((unsigned int) DOTM_ADDR + (i * 2), wordvalue);
			}
			break;
		case DOTM_SET_CLEAR:
			for (i = 0 ; i < 10; i++) {
				wordvalue = dotm_fontmap_empty[i] & 0x7f;
				iom_fpga_itf_write((unsigned int) DOTM_ADDR + (i * 2), wordvalue);
			}
			break;
	}
	return 0;
}

static struct file_operations dotm_fops = {
	.owner = THIS_MODULE,
	.open = dotm_open,
	.write = dotm_write,
	.unlocked_ioctl = dotm_ioctl,
	.release = dotm_release,
};

static struct miscdevice dotm_driver = {
	.fops = &dotm_fops,
	.name = DOTM_NAME,
	.minor = MISC_DYNAMIC_MINOR,
};

int dotm_init(void)
{
	misc_register(&dotm_driver);
	printk(KERN_INFO "driver: %s driver init\n", DOTM_NAME);
	return 0;
}

void dotm_exit(void)
{
	misc_deregister(&dotm_driver);
	printk(KERN_INFO "driver: %s driver exit\n", DOTM_NAME);
}

module_init(dotm_init);
module_exit(dotm_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");
