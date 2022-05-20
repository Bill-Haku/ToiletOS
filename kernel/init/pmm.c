//注意 此模块只适用于至少8M可用空间的机器 因为必须需要至少2048个页来填充分配主链表
#include "pmm.h"
#include "types.h"
#include "printk.h"
#include "boot_struct.h"
#define ERRO_POP_BLOCK 0xFFFFFFFF    //无法寻找页的返回
// extern multiboot_t * mboot_ptr;      //GRUB启动结构体指针 定义在boot.s中 
// extern uint8_t kern_start[];
// extern uint8_t timer;
// extern uint8_t kern_end[];

multiboot_t * mboot_ptr;      //GRUB启动结构体指针 定义在boot.s中 
uint8_t kern_start[];
uint8_t timer;
uint8_t kern_end[];

uint32_t pmm_page_start;
uint32_t pmm_page_end;
uint32_t pmm_max_page_no;    //最大块编号 不能达到的上限(或者我叫做开区间？)
uint32_t singel_page_first_no;
//page_array用来存放页描述结构体的数组 使用动态分配的方式可以节约200KB软盘空间(对我1.44M的可怜空间来说 这已经很多了)
//注意：不要放在0x0地址 不然会与NULL冲突（他喵的 这bug我找了几个小时）并且选择一个合适的对齐 便于
//由于page_array放置地址问题 最大支持不到4GB 过大的数组会扩展到破坏内核空间(可修改存放在1M以上)
pm_page_t *page_array = (pm_page_t*)0x00000100; 
pm_multi_link_t * MULTI_LINK;   //用大写来表示很重要 并且定义为结构体指针，用->更加美观了～
pm_multi_link_t multi_link_struct={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
pm_page_t * SINGLE_LINK = NULL ;
//获取page编号对应的addr
static uint32_t pmm_page_no_to_addr(uint32_t page_no){
	return (page_no<<12)+pmm_page_start;
}

//addr转为page_no
static uint32_t addr_to_pmm_page_no(uint32_t addr){
	return (addr-pmm_page_start)>>12;
}

//将 page_c_t枚举类型转化为对应的块大小 如 (page_c_t)_256——> (uint32_t)256
static uint32_t c_to_uint32(page_c_t ph){
	uint32_t re = 1;
	re = re<<ph;
	return re;
}

static uint32_t get_partner_page_no(uint32_t page_no,page_c_t type){
	//--|--|--|--|--|--| 如图 必须整数倍或者0
	uint32_t v1 = c_to_uint32(type);
	uint32_t v2 = 2*v1;
	if((page_no - v1)%v2 == 0)
		return page_no-v1;
	else
		return page_no+v1;
}


//向链表添加块（用于初始化链表以及free后添加块）
//此处可以使用宏（但是我偏不）
static void append_block(int page_no,page_c_t c){
	page_array[page_no].next = NULL ;     //一定要设置 新加入块的下个指针为NULL
	pm_page_t * header;
	switch(c){
		case _1:
			header = MULTI_LINK ->_1;
			if(header==NULL)
				MULTI_LINK ->_1 = &(page_array[page_no]);
			break;
		case _2:
			header = MULTI_LINK ->_2;
			if(header==NULL)
				MULTI_LINK ->_2 = &(page_array[page_no]);
			break;
		case _4:
			header = MULTI_LINK ->_4;
			if(header==NULL)
				MULTI_LINK ->_4 = &(page_array[page_no]);
			break;
		case _8:
			header = MULTI_LINK ->_8;
			if(header==NULL)
				MULTI_LINK ->_8 = &(page_array[page_no]);
			break;
		case _16:
			header = MULTI_LINK ->_16;
			if(header==NULL)
				MULTI_LINK ->_16 = &(page_array[page_no]);
			break;
		case _32:
			header = MULTI_LINK ->_32;
			if(header==NULL)
				MULTI_LINK ->_32 = &(page_array[page_no]);
			break;
		case _64:
			header = MULTI_LINK ->_64;
			if(header==NULL)
				MULTI_LINK ->_64 = &(page_array[page_no]);
			break;
		case _128:
			header = MULTI_LINK ->_128;
			if(header==NULL)
				MULTI_LINK ->_128 = &(page_array[page_no]);
			break;
		case _256:
			header = MULTI_LINK ->_256;
			if(header==NULL)
				MULTI_LINK ->_256 = &(page_array[page_no]);
			break;
		case _512:
			header = MULTI_LINK ->_512;
			if(header==NULL)
				MULTI_LINK ->_512 = &(page_array[page_no]);
			break;
		case _1024:
			header = MULTI_LINK ->_1024;
			if(header==NULL)
				MULTI_LINK ->_1024 = &(page_array[page_no]);
			break;
		case _erro:
			break;
	}
	if(header!=NULL){
		pm_page_t * probe = header;
		for(;(probe->next)!=NULL;probe = probe->next)
			;
		(probe -> next)=&(page_array[page_no]);
	}
}

//从链表中取出一个块 链表为空返回ERRO_POP_BLOCK
static uint32_t pop_block(page_c_t c){
	pm_page_t * header;
	switch(c){
		case _1:
			header = MULTI_LINK ->_1;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_1 =NULL;
				return header->page_no;
			}
			break;
		case _2:
			header = MULTI_LINK ->_2;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_2 = NULL;
				return header->page_no;
			}
			break;
		case _4:
			header = MULTI_LINK ->_4;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_4 = NULL;
				return header->page_no;
			}
			break;
		case _8:
			header = MULTI_LINK ->_8;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_8 = NULL;
				return header->page_no;
			}
			break;
		case _16:
			header = MULTI_LINK ->_16;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_16 = NULL;
				return header->page_no;
			}
			break;
		case _32:
			header = MULTI_LINK ->_32;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_32 = NULL;
				return header->page_no;
			}
			break;
		case _64:
			header = MULTI_LINK ->_64;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_64 = NULL;
				return header->page_no;
			}
			break;
		case _128:
			header = MULTI_LINK ->_128;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_128 = NULL;
				return header->page_no;
			}
			break;
		case _256:
			header = MULTI_LINK ->_256;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_256 = NULL;
				return header->page_no;
			}
			break;
		case _512:
			header = MULTI_LINK ->_512;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_512 = NULL;
				return header->page_no;
			}
			break;
		case _1024:
			header = MULTI_LINK ->_1024;
			if(header==NULL)
				return ERRO_POP_BLOCK;
			if(header->next==NULL){
				MULTI_LINK ->_1024 = NULL;
				return header->page_no;
			}
			break;
		case _erro:
			return ERRO_POP_BLOCK; 
			break;
	}
	pm_page_t * probe = header;
	for(;probe->next->next!=NULL;probe = probe->next)
		;
	uint32_t return_page_no = probe->next->page_no;
	probe->next=NULL;
	return return_page_no;
}

//从multi_boot结构体中取出需要管理的地址空间大小 
static uint32_t get_max_pm_addr(){          //qemu默认为128M
	uint32_t max_addr=0;
	printk("mmap_length: ");
	printk("%d\n", mboot_ptr->mmap_length); //4026586176 = 3840MB
	pm_entry_t *pm_entry_cur = (pm_entry_t *) mboot_ptr->mmap_addr;
	if ((unsigned long) pm_entry_cur < mboot_ptr->mmap_addr + mboot_ptr->mmap_length) {
		printk("yes\n");
	} else {
		printk_color("[ERROR]Init pmm fail", black, red);
		while(1);
	}
	for(;(unsigned long) pm_entry_cur < mboot_ptr->mmap_addr + mboot_ptr->mmap_length; pm_entry_cur++) {
		printk("0x%h-0x%h-0x%h-%d\n",pm_entry_cur->base_addr_low,pm_entry_cur->length_low,pm_entry_cur->base_addr_low+pm_entry_cur->length_low,pm_entry_cur->type);
		if(pm_entry_cur->type==1&&max_addr<pm_entry_cur->base_addr_low+pm_entry_cur->length_low)
			max_addr=pm_entry_cur->base_addr_low+pm_entry_cur->length_low;		
	}
	return max_addr;
}

//初始化页描述结构体以及装载链表
static void pmm_page_init(){  //初始化链表结构体并且填充链表
	MULTI_LINK=&multi_link_struct;
	MULTI_LINK->_1=NULL;
	MULTI_LINK->_2=NULL;
	MULTI_LINK->_4=NULL;
	MULTI_LINK->_8=NULL;
	MULTI_LINK->_16=NULL;
	MULTI_LINK->_32=NULL;
	MULTI_LINK->_64=NULL;
	MULTI_LINK->_128=NULL;
	MULTI_LINK->_256=NULL;
	MULTI_LINK->_512=NULL;
	MULTI_LINK->_1024=NULL;
	printk("pmm_max_page_no: %d", pmm_max_page_no);
	//我们将要余出至多1023页来作为单页分配的一个缓冲区 这个缓冲区是与伙伴算法独立的 便于快速分配单页
	for(int i=0;i<pmm_max_page_no;i++){
		page_array[i].page_no = i;
		page_array[i].state = 1;
		page_array[i].next = NULL;
	}
	//装载链表
	append_block(0,_1);    //1页链表
	append_block(1,_1);
	append_block(2,_2);    //2页链表
	append_block(4,_4);
	append_block(8,_8);
	append_block(16,_16);
	append_block(32,_32);
	append_block(64,_64);
	append_block(128,_128);
	append_block(256,_256);
	append_block(512,_512);
	append_block(1024,_1024);


	printk("pmm_max_page_no: %d", pmm_max_page_no);
	int temp_page_no=2048;
	for(;temp_page_no<(pmm_max_page_no-1024);temp_page_no+=1024){
		append_block(temp_page_no,_1024);
	}
	singel_page_first_no = temp_page_no;   //将第一个单页缓冲区的编号存放好

	printk("We have %d pages for singel page alloc!\n",pmm_max_page_no-temp_page_no);
	pm_page_t * temp_single_probe = NULL;
	for(;temp_page_no<pmm_max_page_no;temp_page_no++){
		if(SINGLE_LINK == NULL){
			SINGLE_LINK=&(page_array[temp_page_no]);
			temp_single_probe = SINGLE_LINK ;
		}
		else{
			temp_single_probe->next = &(page_array[temp_page_no]);
			temp_single_probe = temp_single_probe -> next;
		}
	}
}


static uint32_t alloc_helper(page_c_t target_ph,page_c_t pop_ph,uint32_t pop_page_no){
	for(;pop_ph!=target_ph;pop_ph--){
		uint32_t append_page_no = pop_page_no+(c_to_uint32(pop_ph)/2);
		append_block(append_page_no,pop_page_no-1);
	}
	return pop_page_no;
}



//伙伴算法多页分配
pm_alloc_t pmm_alloc_pages(uint32_t page_count){
	pm_alloc_t return_struct = {0,_erro,0}; 
	if (page_count>1024||page_count==0)
		return return_struct;    //分配失败
	//来点骚操作
	//获取需要分配的页数
	uint32_t page_count_probe=page_count;
	uint32_t counter1 = 0;
	for(;page_count_probe!=0 ;counter1++,page_count_probe=page_count_probe>>1)
		;
	uint32_t counter2= counter1-1;
	if(page_count==(1<<counter2))
		counter1--;
	//此时的counter1即为pm_c_t枚举的对应值
	page_c_t ph = counter1;
	page_c_t origin_ph = ph;    //存放原始ph
	uint32_t pop_page_no = ERRO_POP_BLOCK;
	for(;ph<_erro;ph++){
		pop_page_no = pop_block(ph);
		if(pop_page_no!=ERRO_POP_BLOCK)
			break;
	}
	if(pop_page_no==ERRO_POP_BLOCK){
		return return_struct;    //分配失败
	}
	else{
		uint32_t target_page_no = alloc_helper(origin_ph,ph,pop_page_no);
		return_struct.addr = pmm_page_no_to_addr(target_page_no);
		return_struct.state = 1;
		return_struct.size = origin_ph;
		return return_struct;
	}
}

//缓冲区及伙伴算法单页分配
//当缓冲区没有页的时候会调用伙伴算法分配一页
pm_alloc_t pmm_alloc_one_page(){
	pm_alloc_t return_struct = {0,_erro,0};
	if(SINGLE_LINK!=NULL){
		pm_page_t * header = SINGLE_LINK;
		if(header->next==NULL){
			SINGLE_LINK = NULL;
			return_struct.state = 1;
			return_struct.size = _1;
			return_struct.addr =pmm_page_no_to_addr(header->page_no);
			return return_struct;
		}
		else{
			pm_page_t * probe = header;
			for(;probe->next->next!=NULL;probe=probe->next){
				//printk("%d\n",probe->page_no);
				//if(probe->page_no==0){
				//	while(1);
				//}
			}
			return_struct.state = 1;
			return_struct.size = _1;
			return_struct.addr =pmm_page_no_to_addr(probe->next->page_no);
			probe->next = NULL;
		}
		return return_struct;
	}
	else{
		return pmm_alloc_pages(1);
	}
}

//free页 返回bool型（定义在typs.h中） True-成功free False-失败
bool pmm_free_page(pm_alloc_t block_disc){
	//检查页是否在页编号范围内
	uint32_t page_no = addr_to_pmm_page_no(block_disc.addr);
	if(page_no<pmm_max_page_no){
		page_c_t size = block_disc.size;
		uint32_t partner_page_no = get_partner_page_no(page_no,size);
		return True;
	}
	else
		return False;
}


//为内核entry使用的pmm管理模块初始化函数
void pmm_init(){
	printk("Kernel Start: 0x%h\n",kern_start);
	printk("Kernel End:   0x%h\n",kern_end);

	//一定要注意 由于分页必须4k对齐 所以此处的物理页管理必须与虚拟页相同 都要4K对齐
	pmm_page_start = ((((uint32_t)kern_end >> 12))+1)<<12;
	if (!CHECK_FLAG(mboot_ptr->flags, 6)) {
		printk("flags[6] not set");
		while(1);
	}
	pmm_page_end = (((get_max_pm_addr() >> 12)))<<12;
	pmm_max_page_no = ((pmm_page_end - pmm_page_start)>>12);
	printk("Pmm page start:  0x%h\n",pmm_page_start);
	printk("Pmm page end:    0x%h\n",pmm_page_end);
	printk("Pmm max page no: %d\n",pmm_max_page_no);
	printk("\n");
	printk("page0:0x%h\n",pmm_page_no_to_addr(32000));
	pmm_page_init();
	printk("page_no:%d\n",SINGLE_LINK->next->page_no);
	printk("***%d***",get_partner_page_no(4,_2));
}