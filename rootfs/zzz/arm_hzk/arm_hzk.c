#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define TESTSTR	{0xb8,0xdf,0xcf,0xe8,0xb5,0xcb,0xd0,0xc0,0xc2,0xc0,0xea,0xbf,'\0'} //高翔邓欣吕昕 
#define BACK_COLOR 0xf800

int printchr16(int top, int left, unsigned short* fb, unsigned char c[16][2]);

int main()
{
	unsigned char chr[16][2];
	unsigned char teststr[] = TESTSTR;
	int len = strlen(teststr);
	FILE* HzK = fopen("HZK16", "rb"); //open HZK file
	if(HzK == NULL)
	{
		perror("fopen error\n");
		exit(1);
	}
	
	int fd_fb = open("/dev/fb0", O_RDWR);	//get a handle of the screen
	unsigned short* fb = (unsigned short *)mmap(0, 240*320*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);	//memery map
	if(fb == MAP_FAILED)
	{
		perror("mmap error\n");
		exit(1);
	}
	
	int i;
	for(i=0; i<len; i+=2)
	{
		int x = teststr[i] - 0xa0;  
		int y = teststr[i+1] - 0xa0;
		fseek(HzK, (94 * (x-1) + (y-1)) * 32, SEEK_SET);	//locate character
		fread(chr, 32, 1, HzK);	//read hzk16 code
		printchr16(90, 5+8*i, fb, chr);	//print to screen
	}
	fclose(HzK);
	return 0;
}

int printchr16(int top, int left, unsigned short* fb, unsigned char c[16][2])
{
	int i, j, k;
	unsigned short tmp;
	for(i=0; i<16; i++)	//16 lines
	{
		for(j=0; j<2; j++)
		{
			for(k=0; k<8; k++)
			{
				if(c[i][j] & (0x80>>k))	//16 bit per line
				{
					//tmp = (i + top)*240 + j*8 + left + k;
					//*(fb + tmp) = FONT_COLOR;
					//printf(" ");
				} else
				{
					tmp = (i + top)*240 + j*8 + left + k;
					*(fb + (unsigned short)tmp) = BACK_COLOR; //show red background colour
					printf("z");
				}
			}
		}
		printf("\n");
	}
	return 0;
}
