#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/vmalloc.h>
#include <asm/current.h>
#include <asm/tlbflush.h>
#include <asm/pgtable.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

static int __init test_init(void)
{
	unsigned char *vmallocmem;
	unsigned int level;
	pte_t* pte;
	unsigned long flags;
	unsigned long phys_addr = 0xfffe100c0000;
	unsigned int *virt_addr = 0;
	unsigned int data = 0;

	printk("%s", __FUNCTION__);

	// get virtual address
	vmallocmem = (unsigned char*)vmalloc(1000000);
	vmallocmem[0x3fc] = 0x12;
	printk("vmallocmem addr=0x%p", vmallocmem);
	// get pte of this virtual address
	pte = lookup_address((unsigned long)vmallocmem, &level);
	// keep the flags of this page
	flags = pte->pte & 0xfff0000000000fff;
	// disable cache
	flags = flags | 0x10;
	printk("pte addr=0x%lx", pte->pte);
	printk("flags=0x%lx", flags);
	// change pte to our physical address
	pte->pte = phys_addr | flags;
	printk("pte addr=0x%lx", pte->pte);
	// add offset to access 0xfffe100c03fc
	virt_addr = (unsigned int*)((unsigned long)vmallocmem + 0x3fc);
	printk("virt addr=0x%p", virt_addr);
	// confirm the pte physical address
	pte = lookup_address((unsigned long)virt_addr, &level);
	printk("pte addr=0x%lx", pte->pte);
	// flush cache
	__flush_tlb_all();
	// read from 0xfffe100c03fc
	data = *virt_addr;
	printk("data=0x%x", data);

	vfree(vmallocmem);
	return 0;
}

static void __exit test_exit(void)
{
	printk("%s", __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);
