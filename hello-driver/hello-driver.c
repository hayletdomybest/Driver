#include <linux/init.h>
#include <linux/module.h>

//文件系統開頭檔
#include <linux/fs.h>

//kernel space with user space communication
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

//定義檔案操作與與file_operationn資料結構：
static int example_open(struct inode *inode,struct file *filp)
{
	printk("my modules oepn\n");
	return 0;
}
static int example_close(struct inode *inode,struct file *filp)
{
	printk("my modules close\n");
	return 0;
}
static ssize_t example_read(struct file *filp, char *buf,\
			    size_t size, loff_t *f_pos) 
{
    printk("my modules read  (size=%zu)\n", size);
    return 0;
}
static ssize_t example_write(struct file *filp,char *buf,\
		             size_t size, loff_t *f_pos)
{
    size_t pos;
    char byte[100];
    char *point=byte;
    for(pos=0;pos<size;pos++)
    {
       if(copy_from_user(point,buf+pos,1))
       {
         break;
       }
       printk("%c ",*point++);
    }
    printk("\n my modules write %s (size=%zu)\n",byte,size);
    return (ssize_t)size;
}
//檔案操作函式
static struct file_operations example_fops = {
    .open = example_open,
    .release = example_close,
    .read = example_read,
    .write = example_write,
};


#define EXAMPLE_MAJOR 60
#define EXAMPLE_NAME "my modules"
static int example_init(void)
{
	int result;
	printk("my modules Start\n");

	/*註冊 Character device*/
	result=register_chrdev(EXAMPLE_MAJOR,EXAMPLE_NAME,&example_fops);
	if(result<0)
	{
		printk("My modules register failed");
		return result;
	}
	return 0;
}

static void example_exit(void)
{
	printk("my modules Stop\n");
	 /* Unregister character device */
    unregister_chrdev(EXAMPLE_MAJOR, EXAMPLE_NAME);
}

module_init(example_init);
module_exit(example_exit);



