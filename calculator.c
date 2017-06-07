#include <linux/module.h>	// included for all kernel modules
#include <linux/kernel.h>	// included for KERN_INFO
#include <linux/init.h>	  // included for __init and __exit macros
#include <linux/fs.h>
#include <asm/uaccess.h>

#include <linux/device.h>
#include <linux/version.h>


#include <linux/cdev.h>

#include <linux/parport.h>
#include <linux/pci.h>

//#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mml");
MODULE_DESCRIPTION("A Simple kernel calc World module");

static int calc_result = 0;
static char *hello_str = "Hello, world!\n";
static int arguments[2] = { 0, 0};
static char operation = '+';

// buffer!
static ssize_t dev_read(struct file *file, char *buf,
						size_t count, loff_t *ppos)
{
	char buff[255];
	int len;
	

	switch (operation)
	{
	case '+':
		calc_result = arguments[0] + arguments[1];
		break;
	case '-':
		calc_result = arguments[0] - arguments[1];
		break;		
	}
	//plus
	//arguments
	int res = snprintf( buff, 255, "Result: %d %c %d = %d .%s", arguments[0] ,
		operation, arguments[1], calc_result, hello_str);
	if (res <= 0)
		return -EOVERFLOW;
	len = MIN(res, 255);
	//strlen(hello_str);
	printk(KERN_INFO "=== read : %d\n", count);
	if (count < len)
		return -EINVAL;
	if (*ppos != 0)
	{
		printk(KERN_INFO "=== read return : 0\n"); // EOF
		return 0;
	}

	if (copy_to_user(buf, buff, len))
		return -EINVAL;
	*ppos = len;
	printk(KERN_INFO "=== read return : %d\n", len);
	return len;
}
enum {max_mess_size = 255};
static char   message[max_mess_size + 1] = {0};           ///< Memory for the string that is passed from userspace
//static short  size_of_message;              ///< Used to remember the size of the string stored

//i.e. operation
static ssize_t operand_write (struct file *file, char const *data1,
						size_t count, loff_t *ppos)
{
	//long result;
	//int ret;// char *pos;
	char const* cpos;
	//char format[] = KERN_INFO "=== pre write : %zu , str '%0s'\n";
	//cpos = data1;
	copy_from_user(message, data1, MIN(count, max_mess_size-1));
	message[max_mess_size-1] = 0;
	//snprintf(message, max_mess_size, "%s(%zu letters)", data1, count);   // appending received string with its length
	//size_of_message = strlen(message);                 // store the length of the stored message
	printk(KERN_INFO " kerncalc: Received %zu characters from the user\n", count);

	printk(KERN_INFO "=== pre write 0:count %zu , \n", count);
	printk(KERN_INFO "=== pre write 1: str %s\n", message);
	switch (*message) {
	case '+':
	case '-':
		operation = *message;
		printk(KERN_INFO "=== kerncalc: operation %c is set\n", operation);
		break;
	default:
	}
	
	// /home/unencr/Prog_projects/kernel_calcf/kernel_calc/target_bin
	//printk(format, count, data);
	//if (count > 0)
	//	data[count - 1] = 0;

	//include <errno.h>
//EINVAL
	//calc_result = result;
	//printk(KERN_INFO "=== write : %zu , str %s, parsed: %ld\n", count, data, result);
	return count;
	
}
//----------------------------------------
#define LEN_MSG 160
static char buf_msg[ LEN_MSG + 1 ] = "Hello from module!\n";
/* <linux/device.h>
LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
struct class_attribute {
struct attribute attr;
ssize_t (*show)(struct class *class, struct class_attribute *attr, char *buf);
ssize_t (*store)(struct class *class, struct class_attribute *attr,
const char *buf, size_t count);
};
LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,32)
struct class_attribute {
struct attribute attr;
ssize_t (*show)(struct class *class, char *buf);
ssize_t (*store)(struct class *class, const char *buf, size_t count);
};
 */
/* sysfs show() method. Calls the show() method corresponding to the individual sysfs file */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)

static ssize_t x_show(struct class *class, struct class_attribute *attr, char *buf)
{
#else

static ssize_t x_show(struct class *class, char *buf)
{
#endif
	strcpy(buf, buf_msg);
	printk("read %d\n", strlen(buf));
	return strlen(buf);
}

static ssize_t argument_store(int opnum, const char *buf, size_t count)
{
#endif
	printk("write %d\n", count);
	strncpy(buf_msg, buf, count);
	buf_msg[ count ] = '\0';	
	
	ret = kstrtol(buf_msg, 10, &arguments[opnum]);// sscanf atoi
	if (ret < 0) {
		pr_err( "=== kstrtol : %d \n", ret);
		return 0;
	}
	pr_info("=== parsed %d", &arguments[opnum]);
	return count;
}

/* sysfs store() method. Calls the store() method corresponding to the individual sysfs file */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)

static ssize_t argument1_store(struct class *class, struct class_attribute *attr,
	const char *buf, size_t count)
{
#else

static ssize_t argument1_store(struct class *class, const char *buf, size_t count)
{
#endif
	

	return argument_store(1, buf, count);
}

/* <linux/device.h>
#define CLASS_ATTR(_name, _mode, _show, _store) \
struct class_attribute class_attr_##_name = __ATTR(_name, _mode, _show, _store) */
CLASS_ATTR(argument1, (S_IWUSR | S_IRUGO), &x_show, &argument1_store);
CLASS_ATTR(argument2, (S_IWUSR | S_IRUGO), &x_show, &argument2_store);
static struct class *sysfs_class;

int sysfs_init(void)
{
	int res;
	sysfs_class = class_create(THIS_MODULE, "arg-class");
	if (IS_ERR(sysfs_class)) printk("bad class create\n");
	res = class_create_file(sysfs_class, &class_attr_argument1);
	if (res < 0)
		return ret;
	res = class_create_file(sysfs_class, &class_attr_argument2);

	/* <linux/device.h>
	extern int __must_check class_create_file(struct class *class, const struct class_attribute
	 *attr); */
	printk("kerncalc 'sysfs'  initialized\n");
	return ret;
}

void sysfs_cleanup(void)
{
	/* <linux/device.h>
	extern void class_remove_file(struct class *class, const struct class_attribute *attr); */
	class_remove_file(sysfs_class, &class_attr_argument1);
	class_remove_file(sysfs_class, &class_attr_argument2);

	class_destroy(sysfs_class);
	return;
}


static int major = 0;
module_param(major, int, S_IRUGO);
#define EOK 0
static int device_open = 0;
static int dev_open(struct inode *n, struct file *f)
{
	if (device_open)
		return -EBUSY;
	device_open++;
	return EOK;
}
static int dev_release(struct inode *n, struct file *f)
{
	device_open--;
	return EOK;
}
static const struct file_operations result_fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
};
static const struct file_operations operand_fops = {
	.owner = THIS_MODULE,
	.open = dev_open,
	.release = dev_release,
	.write = operand_write,
};
#define DEVICE_FIRST 0
#define DEVICE_COUNT 2
#define MODNAME "my_kernel_calc_dev"
static struct cdev hcdev_result;
static struct cdev hcdev_operand;
static struct class *devclass[2];
static char const* devnames[2] = { "operand", "result" };
static int __init kernel_calc_init(void)
{
	int ret, i;
	dev_t dev;
	if (major != 0)
	{
		dev = MKDEV(major, DEVICE_FIRST);
		ret = register_chrdev_region(dev, DEVICE_COUNT, MODNAME);
	}
	else
	{
		ret = alloc_chrdev_region(&dev, DEVICE_FIRST, DEVICE_COUNT, MODNAME);
		major = MAJOR(dev); // не забыть зафиксировать!
	}
	if (ret < 0)
	{
		printk(KERN_ERR "=== Can not register char device region\n");
		goto err;
	}
	cdev_init(&hcdev_operand, &operand_fops);
	hcdev_operand.owner = THIS_MODULE;
	ret = cdev_add(&hcdev_operand, dev, 1);
	if (! (ret < 0)) {
		cdev_init(&hcdev_result, &result_fops);
		hcdev_result.owner = THIS_MODULE;
		ret = cdev_add(&hcdev_result, MKDEV(major, DEVICE_FIRST + 1 ), 1);
	} else 
	{
		unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), 1);
		printk(KERN_ERR "=== Can not add char device\n");
		goto err;
	}
	if (ret < 0)
	{
		unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
		printk(KERN_ERR "=== Can not add char device\n");
		goto err;
	}
	//operand_class 
	devclass[0] = class_create(THIS_MODULE, "kerncalc_operand_class"); /* struct class* */
	devclass[1] = class_create(THIS_MODULE, "kerncalc_result_class"); /* struct class* */
	for (i = 0; i < DEVICE_COUNT; i++)
	{

		dev = MKDEV(major, DEVICE_FIRST + i);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 26)
		/* struct device *device_create( struct class *cls, struct device *parent,
dev_t devt, const char *fmt, ...); */
		device_create(devclass[i], NULL, dev, "%s", devnames[i], i);
#else
		// прототип device_create() изменился!
		/* struct device *device_create( struct class *cls, struct device *parent,
  dev_t devt, void *drvdata, const char *fmt, ...); */
		device_create(devclass[i], NULL, dev, NULL, "%s", devnames[i], i);
#endif
	}
	ret = sysfs_init();
	if (ret < 0) {
		goto err;
	}
	printk(KERN_INFO "======== kerncalc module installed %d:[%d-%d],===========\n",
		   MAJOR(dev), DEVICE_FIRST, MINOR(dev));
err:
	return ret;// Non-zero return means that the module couldn't be loaded.

}

static void __exit kernel_calc_cleanup(void)
{
	//if (ret != 0)
		//pr_err("cannot unregister \n");
	//pr_debug(" module unloaded\n");
	dev_t dev;
	int i;
	for (i = 0; i < DEVICE_COUNT; i++)
	{
		dev = MKDEV(major, DEVICE_FIRST + i);
		device_destroy(devclass[i], dev);
	class_destroy(devclass[i]);
	}
	cdev_del(&hcdev_operand);
	cdev_del(&hcdev_result);

	unregister_chrdev_region(MKDEV(major, DEVICE_FIRST), DEVICE_COUNT);
	sysfs_cleanup()
	printk(KERN_INFO "=============== module removed ==================\n");
}

module_init(kernel_calc_init);
module_exit(kernel_calc_cleanup);
