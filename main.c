#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define cpu_read8(d)			(d)
#define cpu_read16(d)			((((d)&0x00FF)<<8) | (((d)&0xFF00)>>8))
#define cpu_read32(d)			((((d)&0x000000FF)<<24) | (((d)&0x0000FF00)<<8) | (((d)&0x00FF0000)>>8) | (((d)&0xFF000000)>>24))

typedef unsigned char		BYTE;
typedef	unsigned short		WORD;
typedef unsigned int		DWORD;
typedef   signed int		LONG;

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{ // bmfh
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{ // bmih
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;


typedef struct tagBITMAP
{
	BITMAPFILEHEADER* fileHeader;
	BITMAPINFOHEADER* infoHeader;
	BYTE* color;
} BITMAP;


BITMAP* readBitmap(const char* fileName)
{
	int fd;
	BITMAPFILEHEADER* header;
	BITMAPINFOHEADER* info;

	BITMAP* bitmap;


	printf("file name : %s\n", fileName);

	bitmap = (BITMAP *)malloc(sizeof(BITMAP));

	if (bitmap != NULL) {
		bitmap->fileHeader = (BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
		bitmap->infoHeader = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
	}
	if (bitmap == NULL || bitmap->fileHeader == NULL || bitmap->infoHeader == NULL) {
		printf("malloc error.\n");
		return NULL;
	}


	fd = open(fileName, O_RDONLY);
	if (fd == -1) {
		printf("open error.\n");
		return NULL;
	}

	read(fd, bitmap->fileHeader, sizeof(*header));
	printf("file size   = %d\n", bitmap->fileHeader->bfSize);
	printf("offset = %d\n", bitmap->fileHeader->bfOffBits);

	read(fd, bitmap->infoHeader, sizeof(*info));
	printf("width  = %d\n", bitmap->infoHeader->biWidth);
	printf("height = %d\n", bitmap->infoHeader->biHeight);
	printf("bit    = %d\n", bitmap->infoHeader->biBitCount);

	bitmap->color = (BYTE *)malloc(bitmap->fileHeader->bfSize);
	if (bitmap->color == NULL)
		return NULL;

	lseek(fd, bitmap->fileHeader->bfOffBits, SEEK_SET);
	read(fd, bitmap->color, bitmap->fileHeader->bfSize);

	return bitmap;
}


int main(int argc, char *argv[])
{
	char *name;
	BITMAP* bitmapInfo;

	if (argc != 2) {
		name = "test.bmp";
	} else {
		name = argv[1];
	}

	bitmapInfo = readBitmap(name);
	if (bitmapInfo == NULL) {
		printf("bitmap error\n");
		return -1;
	}









	return 0;
}
