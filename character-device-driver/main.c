#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>//kmalloc
#include <linux/vmalloc.h>//vmalloc()
#include <linux/types.h>//ssize_t
#include <linux/fs.h>//file_operaiotns
#include <linux/uaccess.h>//copy_from_user

#define MEM_MALLOC_SIZE 4096 //�w�İϤj�p
#define MEM_MAJOR    240 //�D�]�ƽs��
#define MEM_MINOR    0

char *mem_spvm = NULL; //�w�İϫ��w�A���VRAM
struct cdev *mem_cdev = NULL; //�]�ƹ�H
struct class *mem_class = NULL; //�]�����O���w

static int   mem_init(void);
static void  mem_exit(void);
static int mem_open(struct inode *inode,struct file *filp);
static int mem_release(struct inode *inode, struct file *filp);
static ssize_t mem_read(struct file *filp,char __user *buf,size_t count,loff_t *fpos);
static ssize_t mem_write(struct file *filp, char __user *buf,size_t count ,loff_t *fops);


//�ɮ׾ާ@������O
static const struct file_operations mem_fops={
    .owner = THIS_MODULE,
    .open = mem_open,
    .release = mem_release,
    .read = mem_read,
    .write = mem_write,
};


//��l��
static int  mem_init(void)
{
    int ret;
    //�Ыس]�Ƹ�       �D���X     �����X
    int devno = MKDEV(MEM_MAJOR,MEM_MINOR);
    printk("mem_init initial...\n");

    //�t�m�w�İϪŶ�
    mem_spvm = (char *)vmalloc(MEM_MALLOC_SIZE);
    if(mem_spvm == NULL)
    {
        printk("vmalloc mem_spvm error\n");
        return -ENOMEM;//
    }
    
    //malloc�]�ƹ�H
    mem_cdev = cdev_alloc();
    if(mem_cdev == NULL)
    {
        printk("cdev_alloc error\n");
        return -ENOMEM;
    }
	//�]�ƹ�H��l��
    cdev_init(mem_cdev,&mem_fops);
    mem_cdev->owner = THIS_MODULE;
    ret = cdev_add(mem_cdev,devno,1);//�N�]�Ƹ��Jkernel

	//kernel���J���� �����]�� ���L ���}
    if(ret)
    {
        cdev_del(mem_cdev);
        mem_cdev = NULL;
        printk("cdev_add error\n");
        return -1;
    }

    //
    mem_class = class_create(THIS_MODULE,"fuck you");
    if(IS_ERR(mem_class))
    {
        printk("class_create error..\n");
        return -1;
    }
    device_create(mem_class,NULL,MKDEV(MEM_MAJOR,MEM_MINOR),NULL,"fuck you");
    
    printk("init finished..\n");
    return 0;
}

static void  mem_exit(void)
{
    printk("mem_exit starting..\n");
    if(mem_cdev != NULL)
        cdev_del(mem_cdev);
    printk("cdev_del ok\n");

    device_destroy(mem_class,MKDEV(MEM_MAJOR,MEM_MINOR));
    class_destroy(mem_class);

    if(mem_spvm != NULL)
        vfree(mem_spvm);

    printk("vfree ok\n");
    printk("mem_exit finished..\n");
}

static int mem_open(struct inode *inode,struct file *filp)
{
    printk("open vmalloc space..\n");
    try_module_get(THIS_MODULE);//kernel module�ƶq + 1
    printk("open vamlloc space ok..\n");
    return 0;
}
static int mem_release(struct inode *inode, struct file *filp)
{
    printk("close vmalloc space..\n");
    module_put(THIS_MODULE);//kernel module�ƶq - 1
    return 0;
}
static ssize_t mem_read(struct file *filp,char __user *buf,size_t count,loff_t *fpos)
{
    int ret = -1;
    char *tmp;
    printk("copy data to the user space\n");
    tmp = mem_spvm;
    if(count > MEM_MALLOC_SIZE)
        count = MEM_MALLOC_SIZE;
    if(tmp != NULL)//write user space from kernel
        ret = copy_to_user(buf,tmp,count);
    if(ret == 0)//linux���U�禡 ���榨�\ ���^�� 0
    {
        printk("read copy data success\n");
        return count;
    }
    else
    {
        printk("read copy data error\n");
        return 0;
    }
}
static ssize_t mem_write(struct file *filp, char __user *buf,size_t count ,loff_t *fops)
{
    int ret = -1;
    char *tmp;
    printk("read data from the user space.\n");
    tmp = mem_spvm;
    if(count > MEM_MALLOC_SIZE)
        count = MEM_MALLOC_SIZE;
    if(tmp != NULL)
        ret = copy_from_user(tmp,buf,count);
    if(ret == 0)
    {
        printk("write copy data success.\n");
        return count;
    }
    else
    {
        printk("write copy data error.\n");
        return 0;    
    }
}

MODULE_LICENSE("GPL");
module_init(mem_init);
module_exit(mem_exit);