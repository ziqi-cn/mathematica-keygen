// keymaker.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "config.h"

#ifdef WIN32_ENV
#define PAUSE system("pause ")
#define POPEN _popen
#define PCLOSE _pclose
#endif

#ifdef LINUX_ENV
#define PAUSE 
#define POPEN popen
#define PCLOSE pclose
#endif

const int HASH11[4] = { 0x25DB, 0x42DD, 0x6A91, 0xA439 };
const int HASH12[15] = { 0x29C2, 0x2FDB, 0x44F1, 0x60F0, 0x8250,
                          0x8C68, 0x8E3C, 0xA439, 0xA68B, 0xABEB,
                          0xD227, 0xDB75, 0xE4A8, 0xE756, 0xEE71
                        };

char* randKey()
{
	char* key;
	key = (char*)malloc(17);
	if (key == NULL)
		exit(-1);
	strcpy(key, "1234-4321-123456");
	srand((unsigned)time(NULL));
	for (int i = 0; i < 16; i++)
	{
		if (i != 4 && i != 9)
			key[i] = rand() % 10 + 48;
	}
	return key;
}

char* getMathID(char* wolf)
{
	char cmd[128];
	strcpy(cmd, wolf);
	strcat(cmd, " -machineid");
	FILE* pf = NULL;
	if ((pf = POPEN(cmd, "r")) == NULL)
		return "Error";
	char* mathid;
	mathid = (char*)malloc(256);
	if (fscanf(pf, "%s", mathid) != 1)
		exit(-1);
	PCLOSE(pf);
	return mathid;
}

bool checkMathID(char* id)
{
	if (strlen(id) != 16)
		return false;
	if (id[4] != '-' || id[10] != '-')
		return false;
	for (int i = 0; i < 16; i++)
		if (i != 4 && i != 10)
			if (id[i] > '9' || id[i] < '0')
				return false;
	return true;
}

int testSalt(int a, int b, int c)
{
	for (int i = 0; i < 8; i++)
	{
		int t = (b >> i) & 1;
		if (t + ((a - t) & ~1) == a)
			a = (a - t) >> 1;
		else
			a = ((c - t) ^ a) >> 1;
	}
	return a;
}

char* genPasswd(char* mathid, char* key, int salt)
{
	char* passwd;
	passwd = (char*)malloc(256);
	if (passwd == NULL)
		exit(-1);
	strcpy(passwd, "xxxx-xxx-xxx::1");
	char str[36] = { '\0' };
	int uuid[35] = {0};
	strcat(str, mathid);
	strcat(str, "$1&");
	strcat(str, key);
	for (int i = 0; i < 35; i++)
		uuid[i] = (int)str[i];

	int salt1 = salt;
	for (int i = 34; i >= 0; i--)
		salt1 = testSalt(salt1, uuid[i], 0x105C3);

	int offset1 = 0;
	while (testSalt(testSalt(salt1, offset1 & 0xFF, 0x105C3), offset1 >> 8, 0x105C3) != 0xA5B6)
	{
		offset1++;
		if (offset1 >= 0xFFFF)
			return "Error";
	}

	offset1 = (int)(((offset1 + 0x72FA) & 0xFFFF) * 99999.0 / 0xFFFF);
	int salt2;
	salt2 = (int)(offset1 / 1000 * 1000 + offset1 % 100 * 10 + offset1 / 100 % 10);
	salt2 = (int)((salt2 / 99999.0) * 0xFFFF) + 1;
	salt2 = testSalt(testSalt(0, salt2 & 0xFF, 0x1064B), salt2 >> 8, 0x1064B);
	for (int i = 34; i >= 0; i--)
		salt2 = testSalt(salt2, uuid[i], 0x1064B);

	int offset2 = 0;
	while (testSalt(testSalt(salt2, offset2 & 0xFF, 0x1064B), offset2 >> 8, 0x1064B) != 0xA5B6)
	{
		offset2++;
		if (offset2 >= 0xFFFF)
			return "Error";
	}

	offset2 = (int)((offset2 & 0xFFFF) * 99999.0 / 0xFFFF);

	passwd[0] = offset2 / 10 % 10 + '0';
	passwd[1] = offset1 / 10 % 10 + '0';
	passwd[2] = offset1 / 1000 % 10 + '0';
	passwd[3] = offset1 / 10000 % 10 + '0';
	passwd[4] = '-';
	passwd[5] = offset2 % 10 + '0';
	passwd[6] = offset1 / 100 % 10 + '0';
	passwd[7] = offset2 / 10000 % 10 + '0';
	passwd[8] = '-';
	passwd[9] = offset2 / 100 % 10 + '0';
	passwd[10] = offset1 % 10 + '0';
	passwd[11] = offset2 / 1000 % 10 + '0';
	return passwd;
}