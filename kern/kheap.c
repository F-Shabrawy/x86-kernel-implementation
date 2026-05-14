#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
static int ksizeBefore[(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE]={0};
static int ksizeAfter[(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE]={0};
static bool karr[(KERNEL_HEAP_MAX-KERNEL_HEAP_START)/PAGE_SIZE]={0};


void* kmalloc(unsigned int size)
{
	// Write your code here, remove the panic and write your code

	//NOTE: Allocation is based on FIRST FIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	int needed=(size+PAGE_SIZE-1)/PAGE_SIZE;
	int start=-1;
	int count=0;
	int total= (KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE;
	//placing pages using first find
	for(int i=0;i<total;i++)
	{
		if(!karr[i])
		{
			if(!count)
				start=i;
			count++;
			if(count==needed)
				break;
		}
		else{
			count=0;
			start=-1;
		}
	}
	//if no space is found
	if (count < needed)
	    return NULL;
	if(start==-1)
		return NULL;
	//if virtual space is found
	void* ret=(void*)(KERNEL_HEAP_START+start*PAGE_SIZE);
	for(int i=0;i<needed;i++){
		//allocating from free frame list
		int va = (int)ret + i * PAGE_SIZE;
		struct Frame_Info *ptr_frame_info;
		int r1=allocate_frame(&ptr_frame_info);
		if(r1!=0)
			return NULL;

		int r2=map_frame(ptr_page_directory,ptr_frame_info,(void*)va,PERM_PRESENT|PERM_WRITEABLE);
		if(r2!=0)
			return NULL;
		karr[start+i]=1;
		ksizeBefore[start+i]=i;
		ksizeAfter[start+i]=count-1-i;
	}
	return ret;
}

void kfree(void* virtual_address)
{
	uint32 va = ROUNDDOWN((uint32)virtual_address,PAGE_SIZE);
	if(virtual_address == NULL || va < KERNEL_HEAP_START || va >=KERNEL_HEAP_MAX){
		return;
	}
	uint32 index = (va - KERNEL_HEAP_START) /PAGE_SIZE;
	//check if it is out of bound
	if(index >= (KERNEL_HEAP_MAX - KERNEL_HEAP_START) /PAGE_SIZE){
		return;
	}
	//check if the va is allocated
	if(!karr[index]){
		return;
	}
	uint32 sindex = index - ksizeBefore[index];
	uint32 numOfPages = ksizeBefore[index] + ksizeAfter[index]+1;

	if(numOfPages == 0){
		return;
	}
	//get the start va
	uint32 sva = KERNEL_HEAP_START + sindex * PAGE_SIZE;
	for(uint32 i = 0; i <numOfPages;i++){
		uint32 cva = sva + i* PAGE_SIZE;
		uint32 cindex = sindex + i;
		if(cindex >= (KERNEL_HEAP_MAX - KERNEL_HEAP_START)/PAGE_SIZE){
			break;
		}
		//skip in case it isn't allocated
		if(!karr[cindex]){
			continue;
		}
		karr[cindex] = 0;
		ksizeBefore[cindex] = 0;
		ksizeAfter[cindex] = 0;
		unmap_frame(ptr_page_directory, (void*)cva);
	}
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	// Write your code here, remove the panic and write your code
	int frameBase= (physical_address >> 12) << 12;
	int offset = physical_address - frameBase;
	uint32 va =0 ;
	for (uint32 i = KERNEL_HEAP_START; i < KERNEL_HEAP_MAX; i+=PAGE_SIZE ){
		uint32 * ptr= NULL;
		struct Frame_Info *frame_info_ptr=get_frame_info(ptr_page_directory,(void*)i,&ptr);
		if (frame_info_ptr!=NULL && (ptr[PTX(i)]&PERM_PRESENT)){
			int pa = to_physical_address(frame_info_ptr);
			if (pa == frameBase){
				va= i + offset;
				break;}
		}
	}
	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details
	//change this "return" according to your answer
	return va;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
    unsigned int page_directory_index = PDX(virtual_address);
    unsigned int page_table_index = PTX(virtual_address);
    unsigned int offset = virtual_address & 0x00000FFF;
    uint32 page_directory_entry = ptr_page_directory[page_directory_index];
    if ((page_directory_entry & PERM_PRESENT) == 0)
        return 0;
    uint32* ptr_page_table;
    get_page_table(ptr_page_directory, (void*)virtual_address, &ptr_page_table);
    uint32 page_table_entry = ptr_page_table[page_table_index];
    if ((page_table_entry & PERM_PRESENT) == 0)
        return 0;
    return (page_table_entry & 0xFFFFF000) | offset;
}

void *krealloc(void *virtual_address, uint32 new_size)
{
	panic("krealloc() is not required...!!");
	return NULL;

}
