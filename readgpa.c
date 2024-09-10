#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/kfifo.h>

MODULE_LICENSE("GPL");

#define DUMP_SIZE 1024
#define RESERVED_PHYS_ADDR 0x40a1837000 
static void __iomem *mapped_page = NULL;

int kernel_pa2va(void){
    mapped_page = ioremap(RESERVED_PHYS_ADDR, DUMP_SIZE);
    if(!mapped_page){
        printk(KERN_ERR "failed to map pa %lx\n", RESERVED_PHYS_ADDR);
	return -ENOMEM;
    }
    printk(KERN_INFO "pa %lx to va %lx\n", RESERVED_PHYS_ADDR, mapped_page);
    
    return 0;
}
void read_func(void){
    for(int i = 0; i < DUMP_SIZE; i++){
       printk(KERN_CONT "%u ", ioread8(mapped_page+i));
    }
}

static int __init readgpa_init(void)
{
    kernel_pa2va();
    read_func();
    return 0;
}

static void __exit readgpa_exit(void)
{
    iounmap(mapped_page);
}
EXPORT_SYMBOL(read_func);
module_init(readgpa_init);
module_exit(readgpa_exit);

