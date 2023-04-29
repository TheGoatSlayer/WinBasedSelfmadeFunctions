#define _CRT_SECURE_NO_WARNINGS
#include "ReadFileASCIIArtHeader.h"

void main()
{
	printf("                    ");
	FILE* fp = fopen("ASCIIArts.txt", "r");
	if (fp == NULL)
		return;
	PrintASCIIArt(fp);
	fclose(fp);
}