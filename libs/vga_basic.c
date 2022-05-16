#include "vga_basic.h"
#include "port.h"
static uint8_t tab_length=4;            //定义tab空格数
// VGA 的字符显示缓冲的起点是 0xB8000
static uint8_t *video_memory = (uint8_t *)0xB8000;
//光标位置
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static void move_cursor()
{
// 屏幕是 80 字节宽
    uint16_t cursorLocation = cursor_y * 80 + cursor_x;
    // 在这里用到的两个内部寄存器的编号为14与15，分别表示光标位置
    // 的高8位与低8位。
    outb(0x3D4, 14);                    // 告诉 VGA 我们要设置光标的高字节
    outb(0x3D5, cursorLocation >> 8);   // 发送高 8 位
    outb(0x3D4, 15);                    // 告诉 VGA 我们要设置光标的低字节
    outb(0x3D5, cursorLocation);        // 发送低 8 位
}

void move_cursor_by_XY(uint8_t x,uint8_t y){
	if(x>79||y>24)
		return ;
	cursor_x=x;
	cursor_y=y;
	move_cursor();
}

void clear_screen(){
	cursor_x=0;
	cursor_y=0;
	for(int j=0;j<25;j++){
		for(int i=0;i<80;i++)
			vga_putc('\0',black,white);
	}
	cursor_x=0;
	cursor_y=0;
	move_cursor();
}

void screen_uproll_once(){     //光标是会变化的
	for(int j=1;j<25;j++){
		for(int i=0;i<80;i++){
			uint16_t cursorLocation = j * 80 + i;
			uint16_t targetCursorLocation = (j-1) * 80 + i;
			*((uint16_t *)video_memory+targetCursorLocation)=*((uint16_t *)video_memory+cursorLocation);										
		}
	}
	for(int i=0;i<80;i++){
		uint16_t lastRowCur=24*80+i;
		*((char *)video_memory+2*lastRowCur)='\0';
		*((char *)video_memory+2*lastRowCur+1)=0x0F;									

	}
	if(cursor_y!=0){
		cursor_y--;
		move_cursor();
	}
}
void vga_putc(char input,vga_color_t back,vga_color_t fore){
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;
	*((char *)video_memory+2*cursorLocation)=input;
	*((char *)video_memory+2*cursorLocation+1)=(back<<4)|(fore&0x0F);
	if(cursor_x==79){
		if(cursor_y==24){
			screen_uproll_once();
			cursor_x=0;
			cursor_y=24;
		}
		else{
			cursor_x=0;
			cursor_y++;
		}
	}
	else{
		cursor_x++;
	}
	move_cursor();
}
void kputc_color(char input,vga_color_t back,vga_color_t fore){
	switch (input)
	{
		case '\t':{
			for(int i=0;i<tab_length;i++)
				vga_putc('\0',back,fore);
			break;
		}
		case '\n':{
			uint8_t temp_num = 80-cursor_x;
			if(temp_num==0)
				temp_num=80;
			for(int i=0;i<temp_num;i++)
			vga_putc('\0',back,fore);
			break;
		}
		default:
			vga_putc(input,back,fore);	
	}
}

void kputc(char input){
	kputc_color(input,black,white);
}

void kputs_color(char * input_str,vga_color_t back,vga_color_t fore){
	char * probe=input_str;
	while(*probe!='\0')
		kputc_color(*probe++,back,fore);
}

void kputs(char *input_str){
	kputs_color(input_str,black,white);
}

void vga_init(){
	clear_screen();
}