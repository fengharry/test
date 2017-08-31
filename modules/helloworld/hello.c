#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/vmalloc.h>
#include <asm/current.h>
#include <asm/pgtable.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static int __init hello_init(void)
{
	unsigned char *vmallocmem;
	unsigned int level;
	pte_t* pte;
	unsigned long flags;
	unsigned long phys_addr = 0xfffe100c0000;
	unsigned int *virt_addr = 0;
	unsigned int data = 0;

	printk(KERN_INFO "Hello\n");
	vmallocmem = (unsigned char*)vmalloc(1000000);

	printk("vmallocmem addr=0x%p", vmallocmem);
	pte = lookup_address((unsigned long)vmallocmem, &level);
	flags = pte->pte & 0xfff0000000000fff;
	printk("pte addr=0x%lx", pte->pte);
	printk("flags=0x%lx", flags);
	pte->pte = phys_addr | flags;
	printk("pte addr=0x%lx", pte->pte);
	virt_addr = (unsigned int*)((unsigned long)vmallocmem + 0x3fc);
	printk("virt addr=0x%p", virt_addr);
	pte = lookup_address((unsigned long)virt_addr, &level);
	printk("pte addr=0x%lx", pte->pte);
	data = *virt_addr;
	printk("data=0x%x", data);

	vfree(vmallocmem);
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
