#ifndef PMM_H
#define PMM_H
#include "types.h"
#include "boot_struct.h"

//定义块大小的枚举
typedef 
enum page_c_t{
	_1=0,
	_2=1,
	_4=2,
	_8=3,
	_16=4,
	_32=5,
	_64=6,
	_128=7,
	_256=8,
	_512=9,
	_1024=10,
	_erro=11
} page_c_t;


typedef 
struct pm_page_t{
	uint32_t page_no;
	struct pm_page_t * next;
	uint8_t state;	// 1--空闲可用  0--不可用 实际上由于我采用是否在链表中来判断块状态 所以此字段无用（但是我懒得改）
} pm_page_t;


//链表集合
typedef
struct pm_multi_link_t{
	pm_page_t * _1;
	pm_page_t * _2;
	pm_page_t * _4;
	pm_page_t * _8;
	pm_page_t * _16;
	pm_page_t * _32;
	pm_page_t * _64;
	pm_page_t * _128;
	pm_page_t * _256;
	pm_page_t * _512;
	pm_page_t * _1024;
} pm_multi_link_t;

//分配页时返回的结构体 注意free页时也必须用此结构体 这样可以帮助MUTI链表找到free块后插入哪个子链表
typedef 
struct pm_alloc_t{
	uint32_t addr;     //分配页的地址
	page_c_t size;     //分配的页数量
	uint8_t state;     //为1 分配成功 为0 分配失败  分配后必须先判断此字段
} pm_alloc_t;

void pmm_init();
pm_alloc_t pmm_alloc_pages(uint32_t ); //分配NULL则为出错了 由于NULL=0为内核物理页
pm_alloc_t pmm_alloc_one_page();
bool pmm_free_page(pm_alloc_t );

extern multiboot_t * mboot_ptr;      //GRUB启动结构体指针 定义在boot.s中 
extern uint8_t kern_start[];
extern uint8_t timer;
extern uint8_t kern_end[];

#endif