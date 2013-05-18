//main.c
//zzz
//

#include <stdio.h>
//#include <file.h>
#include <string.h>

#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define TESTSTR	"把海弄干的鱼不在"
//#define TESTSTR	"北京邮电大学"
//#define TESTSTR	"房"
#define TESTSTR	{0xd6,0xf7,0xb2,0xbb,0xd4,0xda,0xba,0xf5,'\0'}	//主不在乎

//#define BACK_COLOR 0xf800
#define BACK_COLOR 60

int printchr16(int top, int left, unsigned short* fb, unsigned char c[16][2]);

int main()
{
	unsigned char chr[16][2];
	unsigned char teststr[] = TESTSTR;
	int len = strlen(teststr);
	FILE* HzK = fopen("HZK16", "rb"); //Hzk file open
	if(HzK == NULL)
	{
		perror("fopen error\n");
		exit(1);
	}
	
	int fd_fb = open("/dev/fb0", O_RDWR);	//get handle of LCD screen
	unsigned short* fb = (unsigned short *)mmap(0, 240*320*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);	//map
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
		fseek(HzK, (94 * (x-1) + (y-1)) * 32, SEEK_SET);	//get offset
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
				if(c[i][j] & (0x80>>k))	//16 bit per character
				{
					//tmp = (i + top)*240 + j*8 + left + k;
					//*(fb + tmp) = FONT_COLOR;
					//printf(" ");
				} else
				{
					tmp = (i + top)*240 + j*8 + left + k;
					*(fb + (unsigned short)tmp) = BACK_COLOR;
					//printf("z");
				}
			}
		}
		printf("\n");
	}
	return 0;
}
