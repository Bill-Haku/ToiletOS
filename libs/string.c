#include "string.h"

//进制转换的辅助函数
static int func(char *s,int n,int i)          //n表示输入的数 i表示转化的进制
{
    char bit[]={"0123456789ABCDEF"};
    int len;
    if(n==0)
    {
        strcpy(s,"");
        return 0;
    }
    func(s,n/i,i);
    len=strlen(s);
    s[len]=bit[n%i];
    s[len+1]='\0';
    return 1;
}

//32位的16进制数 使用0x+8位字符表示+\0   共11位字符
inline char *num2hexstr(int number,int need0x)
{	if (number!=0)
	{
	static char static_hexstr[11]={0};
	static_hexstr[0]='0';
	static_hexstr[1]='x';
	char temp_arry[9]={0};
	char *temp_arry_ptr=temp_arry;
	func(temp_arry_ptr,number,16);
	char temp_arry2[9]={0};
	char *temp_arry2_ptr=temp_arry2;
	strcpy(temp_arry2_ptr,temp_arry_ptr);
	int offset=8-strlen(temp_arry);
	strcpy(temp_arry+offset,temp_arry2);
	for(int i=0;i<offset;i++)
		*(temp_arry+i)='0';
	if(need0x)
		strcpy(static_hexstr+2,temp_arry);
	else
		strcpy(static_hexstr,temp_arry);
	return static_hexstr;
	}
	else 
	{
		if(need0x)
			return "0x00000000";
		else
			return "00000000";
	}
}


inline void memcpy(uint8_t *dest ,const uint8_t *src,uint32_t len)
{
	for(;len!=0;len--)
	{
		*dest=*src;
		dest++;
		src++;
	}
}

inline void memset(void *dest,uint8_t val ,uint32_t len)
{
	uint8_t *dst = (uint8_t *)dest;

    for ( ; len != 0; len--) {
        *dst++ = val;
    }
}

inline void bzero(void *dest, uint32_t len)
{
    memset(dest, 0, len);
}

inline int strcmp(const char *str1,const char *str2)
{
	while(1){
		if (*str1=='\0'&&*str2=='\0')
			return 0;
		else if ((int)*str1>(int)*str2){
			return 1;
		}
		else if((int)*str1>(int)*str2){
			return -1;
		}
		else{
			str1++;
			str2++;
		}
	}
}

inline int strlen(const char *src)
{
	int i=0;
	while(*(src+i)!='\0')
		i++;
	return i;
}

inline char *strcpy(char *dest, const char *src)
{
	char *dest_head=dest;
	while(*(src)!='\0')
	{
		*dest=*src;
		dest++;
		src++;
	}
	*dest='\0';
	dest=dest_head;
	return dest;
}

inline char *strcat(char *dest, const char *src)
{
	char *pointer=dest;
	for(;*pointer!='\0';pointer++);
	strcpy(pointer,src);
	return dest;		
}

inline char *uintTostring(uint32_t num)
{
	char i2stable[10]={'0','1','2','3','4','5','6','7','8','9'};
	static char m[32]={0};
	char *str_ptr=m;
	char *head=str_ptr;
	int i=0;
	for(;num>=10;i++)
	{
		uint32_t num_=num/10;
		uint32_t index=num-(num_*10);
		num=num_;
		*str_ptr=i2stable[index];
		str_ptr++;
	}
	*str_ptr=i2stable[num];
	*(str_ptr+1)='\0';
	return strrevers(head);
}

inline  char *strrevers(char *str)
{
	char *str_head=str;
	int lenth=strlen(str);
	int ptr_h=0;
	int ptr_t=--lenth;
	for(;ptr_h<ptr_t;)
	{
		char temp=*(str_head+ptr_t);
		*(str_head+ptr_t)=*(str_head+ptr_h);
		*(str_head+ptr_h)=temp;
		ptr_h++;
		ptr_t--;
	}
	return str_head;
}