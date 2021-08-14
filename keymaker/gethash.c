#include "keygen.h"
#include <omp.h>

int main(int argc, char* argv[])
{
	char wolf[128] = "wolfram";
	char cmd[128] = "wolfram -version";
	switch (argc)
	{
	case 1:
		if (system("wolfram -version") != 0)
		{
			printf("Can't Find wolfram.\n");
			PAUSE;
			return 0;
		}
		break;
	case 2:
		strcpy(cmd, argv[1]);
		strcat(cmd, " -version");
		if (system(cmd) == 0)
			strcpy(wolf, argv[1]);
		else
		{
			printf("Can't Find wolfram.\n");
			PAUSE;
			return 0;
		}
		break;
	default:
		printf("Too many arguments.\n");
		PAUSE;
		return 0;
	}

	char* mathid;
	char* key;

	mathid = getMathID(wolf);
	key = randKey();

	for (int i = 0; i < 256; i++)
	{
		printf("Current block: %d/256\n", i+1);

		char passwd[256][16] = { {'0'} };
#pragma omp parallel for
		for (int j = 0; j < 256; j++)
			strcpy(passwd[j], genPasswd(mathid, key, 256 * i + j));
#pragma omp parallel for
		for (int j = 0; j < 256; j++)
		{
			char act_cmd[128];
			sprintf(act_cmd, "%s -activate \"%s %s\"", wolf, key, passwd[j]);
			FILE* p = NULL;
			if ((p = POPEN(act_cmd, "r")) == NULL)
				continue;
			if (PCLOSE(p) != 0)
				;
			else {
				printf("0x%X\n", 256 * i + j);
				//return 0;
			}
		}
	}

	return 0;
}