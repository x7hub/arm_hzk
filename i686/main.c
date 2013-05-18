//main.c
//zzz
//

#include <stdio.h>
//#include <file.h>
#include <string.h>

//#define TESTSTR	"把海弄干的鱼不在"
//#define TESTSTR	"北京邮电大学"
//#define TESTSTR	"主不在乎"
#define TESTSTR	{0xd6,0xf7,0xb2,0xbb,0xd4,0xda,0xba,0xf5,'\0'}

int printchr16(unsigned char c[16][2]);

int main()
{
	unsigned char chr[16][2];
	FILE* HzK = fopen("HZK16", "rb");
	unsigned char teststr[] = TESTSTR;
	//int zzz;
	//for(zzz=0; zzz<8; zzz++)
	//	printf("%x\n", *(teststr+zzz));
	int len = strlen(teststr);
	printf("%d\n", len);
	
	int i;
	for(i=0; i<len; i+=2)
	{
		int x = teststr[i] - 0xa0;
		int y = teststr[i+1] - 0xa0;
		fseek(HzK, (94 * (x-1) + (y-1)) * 32, SEEK_SET);	//get offset
		fread(chr, 32, 1, HzK);	//read hzk16 code
		printchr16(chr);	//print to screen
	}
	fclose(HzK);
	return 0;
}

int printchr16(unsigned char c[16][2])
{
	int i, j, k;
	for(i=0; i<16; i++)	//16 lines
	{
		for(j=0; j<2; j++)
		{
			for(k=0; k<8; k++)
			{
				if(c[i][j] & (0x80>>k))	//16 bit per character
				{
					printf(" ");
				} else
				{
					printf("z");
				}
			}
		}
		printf("\n");
	}
	return 0;
}
